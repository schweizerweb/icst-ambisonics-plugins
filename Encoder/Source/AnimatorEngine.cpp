#include "AnimatorEngine.h"

AnimatorEngine::AnimatorEngine()
{
}

void AnimatorEngine::reset(juce::OwnedArray<TimelineModel>* timelines, AmbiSourceSet* sourceSet, double sampleRate_, AnimatorSettings* animatorSettings)
{
    // Store references
    sampleRate = sampleRate_;
    pSourceSet = sourceSet;
    pAnimatorSettings = animatorSettings;
    
    // Copy only valid timelines (up to groupCount)
    copyTimelines(timelines);
    
    // Reset state
    activeMovements.clear();
    activeActions.clear();
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
        
        // Schedule action clips
        for (int clipIdx = 0; clipIdx < timeline->actions.clips.size(); ++clipIdx)
        {
            const auto& clip = timeline->actions.clips[clipIdx];
            ClipSchedule schedule{timelineIdx, clipIdx, clip.start, clip.end(), false};
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
    processActiveActions(positionMs);
    
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
        
        if (schedule.timelineIndex < (pSourceSet ? pSourceSet->groupCount() : 0) &&
            pSourceSet->getActiveGroup(schedule.timelineIndex))
        {
            auto* timeline = copiedTimelines[schedule.timelineIndex];
            
            if (schedule.isMovement)
            {
                const auto& clip = timeline->movement.clips[schedule.clipIndex];
                ms_t elapsedTime = currentTimeMs - schedule.start;
                
                if (elapsedTime < clip.length)
                {
                    startMovementClip(schedule.timelineIndex, clip, currentTimeMs, elapsedTime);
                }
            }
            else
            {
                // Handle action clips - NEW
                const auto& clip = timeline->actions.clips[schedule.clipIndex];
                ms_t elapsedTime = currentTimeMs - schedule.start;
                
                if (elapsedTime < clip.length)
                {
                    startActionClip(schedule.timelineIndex, clip, currentTimeMs, elapsedTime);
                }
            }
        }
        
        nextScheduledClipIndex++;
    }
    
    // Remove finished movements
    for (int i = activeMovements.size() - 1; i >= 0; --i)
    {
        const auto& movement = activeMovements[i];
        bool shouldRemove = false;
        
        if (currentTimeMs >= movement.actualStartTime + movement.clip.length ||
            !pSourceSet ||
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
    
    // Remove finished actions - NEW
    for (int i = activeActions.size() - 1; i >= 0; --i)
    {
        const auto& action = activeActions[i];
        bool shouldRemove = false;
        
        if (currentTimeMs >= action.actualStartTime + action.clip.length ||
            !pSourceSet ||
            action.timelineIndex >= pSourceSet->groupCount() ||
            !pSourceSet->getActiveGroup(action.timelineIndex))
        {
            shouldRemove = true;
        }
        
        if (shouldRemove)
        {
            activeActions.remove(i);
        }
    }
}

void AnimatorEngine::startMovementClip(int timelineIndex, const MovementClip& clip, ms_t currentTimeMs, ms_t elapsedTime)
{
    // Remove any existing movement for this timeline
    for (int i = activeMovements.size() - 1; i >= 0; --i)
    {
        if (activeMovements[i].timelineIndex == timelineIndex)
        {
            activeMovements.remove(i);
        }
    }
    
    // Start new movement with adjusted start time - use the constructor with elapsedTime
    ActiveMovement newMovement(timelineIndex, clip, currentTimeMs - elapsedTime, elapsedTime);
    
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
    }
    
    // Store start angle for circle/spiral
    if (clip.movementType == MovementType::Circle || clip.movementType == MovementType::Spiral)
    {
        auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
        juce::Vector3D<double> startPos = clip.useStartPoint ?
            juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ()) :
            getGroupPosition(timelineIndex);
        
        double relX = startPos.x - centerPos.x;
        double relY = startPos.y - centerPos.y;
        newMovement.startAngle = std::atan2(relY, relX);
        
        if(clip.movementType == MovementType::Spiral)
        {
            newMovement.startRadius = std::sqrt(relX * relX + relY * relY);
        }
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
        
        // Calculate progress based on the actual start time and current time
        ms_t timeInMovement = currentTimeMs - movement.actualStartTime;
        double progress = static_cast<double>(timeInMovement) / movement.clip.length;
        
        // Clamp progress to [0, 1] to handle edge cases
        progress = juce::jlimit(0.0, 1.0, progress);
        
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
    if (approximatelyEqual(startPos.x, 0.0) && approximatelyEqual(startPos.y, 0.0) && approximatelyEqual(startPos.z, 0.0))
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
    if (approximatelyEqual(startPos.x, 0.0) && approximatelyEqual(startPos.y, 0.0) && approximatelyEqual(startPos.z, 0.0))
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
    
    // Get stored start values
    double startRadius = 0.0;
    double startAngle = 0.0;
    for (auto& movement : activeMovements)
    {
        if (movement.timelineIndex == timelineIndex && movement.clip.movementType == MovementType::Spiral)
        {
            startRadius = movement.startRadius;
            startAngle = movement.startAngle;
            break;
        }
    }
    
    // FIX: Use count for direction, but absolute value for radius calculations
    double direction = (clip.count >= 0) ? -1.0 : 1.0; // Negative for clockwise, positive for counter-clockwise
    double absoluteCount = std::abs(clip.count);
    
    // Calculate the motion using direction
    double angle = startAngle + (2.0 * juce::MathConstants<double>::pi * absoluteCount * progress * direction);
    
    // Calculate current radius with spiral change - use absolute count
    double totalRadiusChange = clip.radiusChange * absoluteCount;
    double currentRadius = startRadius + (totalRadiusChange * progress);
    
    // Prevent negative radius
    if (currentRadius < 0.0)
        currentRadius = 0.0;
    
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

void AnimatorEngine::setAnimatorState(bool enable)
{
    bool change = (enable != pAnimatorSettings->enable);
    if(change)
    {
        pAnimatorSettings->enable = enable;
        sendChangeMessage();
        if(enable)
        {
            // TODO reset();
        }
    }
}

void AnimatorEngine::setAutoFollow(bool enable)
{
    bool change = (enable != pAnimatorSettings->autoFollow);
    if(change)
    {
        pAnimatorSettings->autoFollow = enable;
        sendChangeMessage();
    }
}

bool AnimatorEngine::getAnimatorState()
{
    return (pAnimatorSettings != nullptr && pAnimatorSettings->enable);
}

bool AnimatorEngine::getAutoFollow()
{
    return (pAnimatorSettings != nullptr && pAnimatorSettings->autoFollow);
}

void AnimatorEngine::startActionClip(int timelineIndex, const ActionClip& clip, ms_t currentTimeMs, ms_t elapsedTime)
{
    // Remove any existing action for this timeline
    for (int i = activeActions.size() - 1; i >= 0; --i)
    {
        if (activeActions[i].timelineIndex == timelineIndex)
        {
            activeActions.remove(i);
        }
    }
    
    // Start new action
    ActiveAction newAction(timelineIndex, clip, currentTimeMs - elapsedTime, elapsedTime);
    
    // Store initial state from the source set
    if (pSourceSet && timelineIndex < pSourceSet->groupCount())
    {
        if (auto* group = pSourceSet->getActiveGroup(timelineIndex))
        {
            newAction.initialRotation = group->getRotation();
            newAction.initialStretch = group->getStretch();
            newAction.hasInitialState = true;
        }
    }
    
    activeActions.add(newAction);
}

// In processActiveActions, add more robust checking:
void AnimatorEngine::processActiveActions(ms_t currentTimeMs)
{
    if (!pSourceSet) return;
    
    for (auto& action : activeActions)
    {
        // Skip if timeline index is invalid or group doesn't exist
        if (action.timelineIndex >= pSourceSet->groupCount() ||
            !pSourceSet->getActiveGroup(action.timelineIndex))
        {
            continue;
        }
        
        // Calculate progress with safety check
        ms_t timeInAction = currentTimeMs - action.actualStartTime;
        double progress = (action.clip.length > 0) ?
            static_cast<double>(timeInAction) / action.clip.length : 0.0;
        progress = juce::jlimit(0.0, 1.0, progress);
        
        // Process all actions in this clip
        for (const auto& actionDef : action.clip.actions)
        {
            if (actionDef.getAction() != ActionType::None)
            {
                processSingleAction(action.timelineIndex, actionDef, progress, action);
            }
        }
    }
}

void AnimatorEngine::processSingleAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction)
{
    switch (actionDef.getAction())
    {
        case ActionType::RotationX:
        case ActionType::RotationY:
        case ActionType::RotationZ:
            processRotationAction(timelineIndex, actionDef, progress, activeAction);
            break;
            
        case ActionType::Stretch:
            processStretchAction(timelineIndex, actionDef, progress, activeAction);
            break;
            
        case ActionType::None:
        default:
            break;
    }
}

void AnimatorEngine::processRotationAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction)
{
    if (!pSourceSet || timelineIndex >= pSourceSet->groupCount()) return;
    
    double angle = 0.0;
    
    switch (actionDef.getTiming())
    {
        case TimingType::AbsoluteTarget:
        {
            if (actionDef.getUseStartValue() && activeAction.hasInitialState)
            {
                // Interpolate between start value and target value
                angle = actionDef.getStartValue() + (actionDef.getValue() - actionDef.getStartValue()) * progress;
            }
            else
            {
                // Just progress toward target value
                angle = actionDef.getValue() * progress;
            }
            break;
        }
            
        case TimingType::RelativeDuringClip:
        {
            angle = actionDef.getValue() * progress;
            if (actionDef.getUseStartValue())
            {
                angle += actionDef.getStartValue();
            }
            break;
        }
            
        case TimingType::ConstantPerSecond:
        {
            double clipDurationSeconds = activeAction.clip.length / 1000.0;
            angle = actionDef.getValue() * clipDurationSeconds * progress;
            break;
        }
            
        case TimingType::None:
            return;
    }
    
    // Apply the rotation using the simple rotateGroup function
    double xAngle = 0.0, yAngle = 0.0, zAngle = 0.0;
    
    switch (actionDef.getAction())
    {
        case ActionType::RotationX: xAngle = angle; break;
        case ActionType::RotationY: yAngle = angle; break;
        case ActionType::RotationZ: zAngle = angle; break;
        default: return;
    }
    
    pSourceSet->rotateGroup(timelineIndex, xAngle, yAngle, zAngle);
    
    // Debug output
    DBG("Rotation - Timeline: " << timelineIndex
        << " | Action: " << (int)actionDef.getAction()
        << " | Progress: " << progress
        << " | Angle: " << angle << "°"
        << " | Applied: X=" << xAngle << "°, Y=" << yAngle << "°, Z=" << zAngle << "°");
}

void AnimatorEngine::processStretchAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction)
{
    if (!pSourceSet || timelineIndex >= pSourceSet->groupCount()) return;
    
    double currentStretch = 1.0;
    
    switch (actionDef.getTiming())
    {
        case TimingType::AbsoluteTarget:
        {
            if (actionDef.getUseStartValue())
            {
                currentStretch = actionDef.getStartValue() +
                                (actionDef.getValue() - actionDef.getStartValue()) * progress;
            }
            else if (activeAction.hasInitialState)
            {
                // Interpolate from initial stretch to target stretch
                currentStretch = activeAction.initialStretch +
                                (actionDef.getValue() - activeAction.initialStretch) * progress;
            }
            else
            {
                currentStretch = actionDef.getValue();
            }
            break;
        }
            
        case TimingType::RelativeDuringClip:
        {
            currentStretch = 1.0 + actionDef.getValue() * progress;
            if (actionDef.getUseStartValue())
            {
                currentStretch = actionDef.getStartValue() * (1.0 + actionDef.getValue() * progress);
            }
            else if (activeAction.hasInitialState)
            {
                currentStretch = activeAction.initialStretch * (1.0 + actionDef.getValue() * progress);
            }
            break;
        }
            
        case TimingType::ConstantPerSecond:
        {
            double clipDurationSeconds = activeAction.clip.length / 1000.0;
            currentStretch = 1.0 + actionDef.getValue() * clipDurationSeconds * progress;
            
            if (activeAction.hasInitialState)
            {
                currentStretch = activeAction.initialStretch * (1.0 + actionDef.getValue() * clipDurationSeconds * progress);
            }
            break;
        }
            
        case TimingType::None:
            return;
    }
    
    // Ensure stretch doesn't go negative
    if (currentStretch < 0.01) currentStretch = 0.01;
    
    pSourceSet->setGroupStretch(timelineIndex, currentStretch, true);
}
