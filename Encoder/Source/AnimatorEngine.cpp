#include "AnimatorEngine.h"

AnimatorEngine::AnimatorEngine()
{
}

void AnimatorEngine::reset(juce::OwnedArray<TimelineModel>* timelines, AmbiSourceSet* sourceSet, double sampleRate_)
{
    // Store references
    sampleRate = sampleRate_;
    pSourceSet = sourceSet;
    
    // Copy only valid timelines (up to groupCount)
    copyTimelines(timelines);
    
    // Reset state
    activeMovements.clear();
    clipSchedule.clear();
    nextScheduledClipIndex = 0;
    lastPositionMs = 0;
    
    // Build clip schedule only for valid timelines
    int maxValidTimelines = pSourceSet ? pSourceSet->groupCount() : 0;
    for (int timelineIdx = 0; timelineIdx < copiedTimelines.size() && timelineIdx < maxValidTimelines; ++timelineIdx)
    {
        auto* timeline = copiedTimelines[timelineIdx];
        
        // Schedule movement clips
        for (int clipIdx = 0; clipIdx < timeline->movement.clips.size(); ++clipIdx)
        {
            const auto& clip = timeline->movement.clips[clipIdx];
            ClipSchedule schedule{timelineIdx, clipIdx, clip.start, clip.end(), true};
            clipSchedule.add(schedule);
        }
    }
    
    // Sort clips by start time
    std::sort(clipSchedule.begin(), clipSchedule.end());
    
    // Mark that we need to pre-render on first process call
    needsPreRender = true;
}

void AnimatorEngine::processAnimationAt(ms_t positionMs)
{
    // Handle pre-rendering on first call after reset
    if (needsPreRender)
    {
        preRenderMovements();
        needsPreRender = false;
    }
    
    // Update active movements based on current position
    updateActiveMovements(positionMs);
    
    // Process all active movements
    processActiveMovements(positionMs);
    
    lastPositionMs = positionMs;
}

void AnimatorEngine::copyTimelines(juce::OwnedArray<TimelineModel>* sourceTimelines)
{
    copiedTimelines.clear();
    
    // Only copy timelines up to the number of valid groups
    int maxValidTimelines = pSourceSet ? pSourceSet->groupCount() : 0;
    
    for (int i = 0; i < sourceTimelines->size() && i < maxValidTimelines; ++i)
    {
        auto* timeline = (*sourceTimelines)[i];
        auto* newTimeline = new TimelineModel();
        
        // Copy movement clips
        for (const auto& clip : timeline->movement.clips)
            newTimeline->movement.clips.add(clip);
        
        // Copy action clips
        for (const auto& clip : timeline->actions.clips)
            newTimeline->actions.clips.add(clip);
            
        copiedTimelines.add(newTimeline);
    }
}


void AnimatorEngine::preRenderMovements()
{
    // Pre-render all movement clips that have defined start points
    for (int timelineIdx = 0; timelineIdx < copiedTimelines.size(); ++timelineIdx)
    {
        auto* timeline = copiedTimelines[timelineIdx];
        
        for (const auto& clip : timeline->movement.clips)
        {
            if (clip.useStartPoint)
            {
                preRenderMovementClip(timelineIdx, clip);
            }
        }
    }
}

void AnimatorEngine::preRenderMovementClip(int timelineIndex, const MovementClip& clip)
{
    // Calculate number of frames at audio block resolution
    const double clipDurationSeconds = clip.length / 1000.0;
    const int totalFrames = static_cast<int>(std::ceil(clipDurationSeconds * sampleRate / 512.0));
    
    juce::Array<juce::Vector3D<double>> frames;
    
    for (int frame = 0; frame < totalFrames; ++frame)
    {
        const double progress = static_cast<double>(frame) / (totalFrames - 1);
        
        // Create a temporary ActiveMovement object
        ActiveMovement tempMovement(timelineIndex, clip, 0);
        auto position = calculateMovementPosition(tempMovement, progress);
        frames.add(position);
    }
}

void AnimatorEngine::updateActiveMovements(ms_t currentTimeMs)
{
    // Check for new clips starting
    while (nextScheduledClipIndex < clipSchedule.size() &&
           clipSchedule[nextScheduledClipIndex].start <= currentTimeMs)
    {
        const auto& schedule = clipSchedule[nextScheduledClipIndex];
        
        // Only start movement if timeline index is valid and group exists
        if (schedule.isMovement &&
            schedule.timelineIndex < (pSourceSet ? pSourceSet->groupCount() : 0) &&
            pSourceSet->getActiveGroup(schedule.timelineIndex))
        {
            auto* timeline = copiedTimelines[schedule.timelineIndex];
            const auto& clip = timeline->movement.clips[schedule.clipIndex];
            startMovementClip(schedule.timelineIndex, clip, currentTimeMs);
        }
        
        nextScheduledClipIndex++;
    }
    
    // Remove finished movements and movements for invalid groups
    for (int i = activeMovements.size() - 1; i >= 0; --i)
    {
        const auto& movement = activeMovements[i];
        bool shouldRemove = false;
        
        // Remove if movement is finished
        if (currentTimeMs >= movement.actualStartTime + movement.clip.length)
        {
            shouldRemove = true;
        }
        // Remove if group no longer exists
        else if (!pSourceSet ||
                 movement.timelineIndex >= pSourceSet->groupCount() ||
                 !pSourceSet->getActiveGroup(movement.timelineIndex))
        {
            shouldRemove = true;
        }
        
        if (shouldRemove)
        {
            activeMovements.remove(i);
        }
    }
}

void AnimatorEngine::startMovementClip(int timelineIndex, const MovementClip& clip, ms_t currentTimeMs)
{
    // Remove any existing movement for this timeline
    for (int i = activeMovements.size() - 1; i >= 0; --i)
    {
        if (activeMovements[i].timelineIndex == timelineIndex)
        {
            activeMovements.remove(i);
        }
    }
    
    // Start new movement
    ActiveMovement newMovement(timelineIndex, clip, currentTimeMs);
    
    // Store the initial position for MoveTo operations
    if (clip.movementType == MovementType::MoveToCartesian || clip.movementType == MovementType::MoveToPolar)
    {
        if (clip.useStartPoint)
        {
            newMovement.initialPosition = juce::Vector3D<double>(
                clip.startPointGroup.getX(),
                clip.startPointGroup.getY(),
                clip.startPointGroup.getZ());
        }
        else
        {
            newMovement.initialPosition = getGroupPosition(timelineIndex);
        }
        DBG("MoveTo - Stored initial position: (" << newMovement.initialPosition.x
            << ", " << newMovement.initialPosition.y << ", " << newMovement.initialPosition.z << ")");
    }
    
    // Store start angle for circle/spiral (existing code)
    if (clip.movementType == MovementType::Circle || clip.movementType == MovementType::Spiral)
    {
        auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
        juce::Vector3D<double> startPos = clip.useStartPoint ?
            juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ()) :
            getGroupPosition(timelineIndex);
        
        double relX = startPos.x - centerPos.x;
        double relY = startPos.y - centerPos.y;
        newMovement.startAngle = std::atan2(relY, relX);
    }
    
    activeMovements.add(newMovement);
}

void AnimatorEngine::processActiveMovements(ms_t currentTimeMs)
{
    if (!pSourceSet) return;
    
    for (auto& movement : activeMovements)
    {
        // Skip if timeline index is invalid or group doesn't exist
        if (movement.timelineIndex >= pSourceSet->groupCount() ||
            !pSourceSet->getActiveGroup(movement.timelineIndex))
        {
            continue;
        }
        
        ms_t elapsed = currentTimeMs - movement.actualStartTime;
        double progress = movement.getProgress(currentTimeMs);
        
        // Debug logging for MoveTo movements
        if (movement.clip.movementType == MovementType::MoveToCartesian ||
            movement.clip.movementType == MovementType::MoveToPolar)
        {
            static ms_t lastLogTime = 0;
            if (currentTimeMs - lastLogTime > 100) // Log every 100ms to avoid spam
            {
                DBG("MoveTo Progress - elapsed: " << elapsed << "ms/" << movement.clip.length
                    << "ms, progress: " << progress);
                lastLogTime = currentTimeMs;
            }
        }
        
        auto position = calculateMovementPosition(movement, progress);
        pSourceSet->setGroupXyz(movement.timelineIndex, position.x, position.y, position.z, true);
    }
}

juce::Vector3D<double> AnimatorEngine::getGroupPosition(int timelineIndex)
{
    if (pSourceSet &&
        timelineIndex < pSourceSet->groupCount() &&
        pSourceSet->getActiveGroup(timelineIndex))
    {
        return pSourceSet->getActiveGroup(timelineIndex)->getVector3D();
    }
    
    // Return default position if group doesn't exist
    return juce::Vector3D<double>(0.0, 0.0, 0.0);
}

juce::Vector3D<double> AnimatorEngine::calculateMovementPosition(const ActiveMovement& activeMovement, double progress)
{
    const auto& clip = activeMovement.clip;
    
    switch (clip.movementType)
    {
        case MovementType::MoveToCartesian:
            return calculateLinearCartesian(clip, progress, activeMovement.timelineIndex);
            
        case MovementType::MoveToPolar:
            return calculateLinearPolar(clip, progress, activeMovement.timelineIndex);
            
        case MovementType::Circle:
            return calculateCircle(clip, progress, activeMovement.timelineIndex);
            
        case MovementType::Spiral:
            return calculateSpiral(clip, progress, activeMovement.timelineIndex);
            
        default:
            return juce::Vector3D<double>(0.0, 0.0, 0.0);
    }
}

juce::Vector3D<double> AnimatorEngine::calculateLinearCartesian(const MovementClip& clip, double progress, int timelineIndex)
{
    juce::Vector3D<double> startPos;
    
    // Find the active movement to get the stored initial position
    for (auto& movement : activeMovements)
    {
        if (movement.timelineIndex == timelineIndex &&
            (movement.clip.movementType == MovementType::MoveToCartesian || movement.clip.movementType == MovementType::MoveToPolar))
        {
            startPos = movement.initialPosition; // Use the stored initial position
            break;
        }
    }
    
    // Fallback if no active movement found (shouldn't happen)
    if (startPos.x == 0 && startPos.y == 0 && startPos.z == 0)
    {
        if (clip.useStartPoint)
        {
            startPos = juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ());
        }
        else
        {
            startPos = getGroupPosition(timelineIndex);
        }
    }
    
    auto targetPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
    
    DBG("Linear Cartesian - progress: " << progress
        << ", using FIXED start: (" << startPos.x << ", " << startPos.y << ", " << startPos.z << ")"
        << ", target: (" << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << ")");
    
    return startPos + (targetPos - startPos) * progress;
}

juce::Vector3D<double> AnimatorEngine::calculateLinearPolar(const MovementClip& clip, double progress, int timelineIndex)
{
    juce::Vector3D<double> startPos;
    
    // Find the active movement to get the stored initial position
    for (auto& movement : activeMovements)
    {
        if (movement.timelineIndex == timelineIndex &&
            (movement.clip.movementType == MovementType::MoveToCartesian || movement.clip.movementType == MovementType::MoveToPolar))
        {
            startPos = movement.initialPosition; // Use the stored initial position
            break;
        }
    }
    
    // Fallback if no active movement found (shouldn't happen)
    if (startPos.x == 0 && startPos.y == 0 && startPos.z == 0)
    {
        if (clip.useStartPoint)
        {
            startPos = juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ());
        }
        else
        {
            startPos = getGroupPosition(timelineIndex);
        }
    }
    
    auto targetPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
    
    DBG("Linear Polar - progress: " << progress
        << ", using FIXED start: (" << startPos.x << ", " << startPos.y << ", " << startPos.z << ")"
        << ", target: (" << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << ")");
    
    // Convert to spherical coordinates
    auto startSpherical = cartesianToSpherical(startPos);
    auto targetSpherical = cartesianToSpherical(targetPos);
    
    // Handle azimuth wrapping for shortest path
    double startAzimuth = startSpherical.x;
    double targetAzimuth = targetSpherical.x;
    double angularDist = targetAzimuth - startAzimuth;
    
    if (angularDist > juce::MathConstants<double>::pi)
        angularDist -= 2.0 * juce::MathConstants<double>::pi;
    else if (angularDist < -juce::MathConstants<double>::pi)
        angularDist += 2.0 * juce::MathConstants<double>::pi;
    
    // Linear interpolation in spherical space
    double interpAzimuth = startAzimuth + angularDist * progress;
    double interpElevation = startSpherical.y + (targetSpherical.y - startSpherical.y) * progress;
    double interpDistance = startSpherical.z + (targetSpherical.z - startSpherical.z) * progress;
    
    return sphericalToCartesian(juce::Vector3D<double>(interpAzimuth, interpElevation, interpDistance));
}

juce::Vector3D<double> AnimatorEngine::calculateCircle(const MovementClip& clip, double progress, int timelineIndex)
{
    auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
    
    double radius;
    if (clip.useStartPoint)
    {
        auto startPos = juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ());
        radius = calculateDistance(startPos, centerPos);
    }
    else
    {
        auto currentPos = getGroupPosition(timelineIndex);
        radius = calculateDistance(currentPos, centerPos);
    }
    
    // Find the active movement to get the stored start angle
    double startAngle = 0.0;
    for (auto& movement : activeMovements)
    {
        if (movement.timelineIndex == timelineIndex && movement.clip.movementType == MovementType::Circle)
        {
            startAngle = movement.startAngle;
            break;
        }
    }
    
    // Calculate the motion: positive count = clockwise, so we subtract from start angle
    double angle = startAngle - (2.0 * juce::MathConstants<double>::pi * clip.count * progress);
    
    DBG("Circle - count: " << clip.count << ", progress: " << progress
        << ", startAngle: " << startAngle << " rad (" << (startAngle * 180.0 / juce::MathConstants<double>::pi) << "°)"
        << ", final angle: " << angle << " rad (" << (angle * 180.0 / juce::MathConstants<double>::pi) << "°)");
    
    // Circular motion in XY plane around center
    return juce::Vector3D<double>(
        centerPos.x + radius * std::cos(angle),
        centerPos.y + radius * std::sin(angle),
        centerPos.z
    );
}

juce::Vector3D<double> AnimatorEngine::calculateSpiral(const MovementClip& clip, double progress, int timelineIndex)
{
    auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
    
    double startRadius;
    if (clip.useStartPoint)
    {
        auto startPos = juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ());
        startRadius = calculateDistance(startPos, centerPos);
    }
    else
    {
        auto currentPos = getGroupPosition(timelineIndex);
        startRadius = calculateDistance(currentPos, centerPos);
    }
    
    // Find the active movement to get the stored start angle
    double startAngle = 0.0;
    for (auto& movement : activeMovements)
    {
        if (movement.timelineIndex == timelineIndex && movement.clip.movementType == MovementType::Spiral)
        {
            startAngle = movement.startAngle;
            break;
        }
    }
    
    // Calculate the motion: positive count = clockwise, so we subtract from start angle
    double angle = startAngle - (2.0 * juce::MathConstants<double>::pi * clip.count * progress);
    
    // Calculate current radius with spiral change
    double completedRounds = clip.count * progress;
    double currentRadius = startRadius + (clip.radiusChange * completedRounds);
    
    DBG("Spiral - count: " << clip.count << ", progress: " << progress
        << ", startAngle: " << startAngle << " rad (" << (startAngle * 180.0 / juce::MathConstants<double>::pi) << "°)"
        << ", final angle: " << angle << " rad (" << (angle * 180.0 / juce::MathConstants<double>::pi) << "°)"
        << ", startRadius: " << startRadius << ", currentRadius: " << currentRadius);
    
    // Spiral motion in XY plane
    return juce::Vector3D<double>(
        centerPos.x + currentRadius * std::cos(angle),
        centerPos.y + currentRadius * std::sin(angle),
        centerPos.z
    );
}

// Coordinate conversion helpers (same as before)
juce::Vector3D<double> AnimatorEngine::cartesianToSpherical(const juce::Vector3D<double>& cartesian)
{
    double x = cartesian.x, y = cartesian.y, z = cartesian.z;
    double distance = std::sqrt(x*x + y*y + z*z);
    
    if (distance < 1e-12) return juce::Vector3D<double>(0.0, 0.0, 0.0);
    
    double azimuth = std::atan2(y, x);
    double elevation = std::asin(z / distance);
    
    return juce::Vector3D<double>(azimuth, elevation, distance);
}

juce::Vector3D<double> AnimatorEngine::sphericalToCartesian(const juce::Vector3D<double>& spherical)
{
    double azimuth = spherical.x, elevation = spherical.y, distance = spherical.z;
    
    if (distance < 1e-12) return juce::Vector3D<double>(0.0, 0.0, 0.0);
    
    double x = distance * std::cos(elevation) * std::cos(azimuth);
    double y = distance * std::cos(elevation) * std::sin(azimuth);
    double z = distance * std::sin(elevation);
    
    return juce::Vector3D<double>(x, y, z);
}

double AnimatorEngine::calculateDistance(const juce::Vector3D<double>& a, const juce::Vector3D<double>& b) const
{
    auto diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}
