#include "AnimatorEngine.h"
#include "../../Common/MathHelper.h"
#include "../../Common/PerlinNoise.h"

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

    // A backward jump means the host (or the Animator's own cursor) seeked - e.g. a loop restart,
    // or clicking earlier on the timeline while playing. nextScheduledClipIndex only ever advances
    // forward (see updateActiveMovements()), so without this, any clip that starts earlier than the
    // current scan position would simply never be re-triggered after a seek. Clearing and rescanning
    // the whole (sorted) schedule re-evaluates every clip against the new position, correctly
    // restarting whichever ones should be active - startMovementClip()/startActionClip() already
    // recapture whatever initial state they need per clip.
    if (positionMs < lastPositionMs)
    {
        activeMovements.clear();
        activeActions.clear();
        nextScheduledClipIndex = 0;
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

                if (elapsedTime < clip.length && !clip.muted)
                {
                    startMovementClip(schedule.timelineIndex, clip, currentTimeMs, elapsedTime);
                }
            }
            else
            {
                // Handle action clips
                const auto& clip = timeline->actions.clips[schedule.clipIndex];
                ms_t elapsedTime = currentTimeMs - schedule.start;

                if (elapsedTime < clip.length && !clip.muted)
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
    
    // Remove finished actions
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
            clearJitterOffsets(action);
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

    // Capture the one-time start state (see AnimatorMath.h - shared with the clip editors' preview)
    const auto start = AnimatorMath::computeMovementStartState(clip, getGroupPosition(timelineIndex));
    newMovement.initialPosition = start.initialPosition;
    newMovement.startAngle = start.startAngle;
    newMovement.startRadius = start.startRadius;

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

        // Muting can be toggled while this clip is already playing - check live rather than
        // only at scheduling time, so it takes effect immediately.
        if (movement.clip.muted)
            continue;
        
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

    AnimatorMath::MovementStartState start;
    start.initialPosition = activeMovement.initialPosition;
    start.startAngle = activeMovement.startAngle;
    start.startRadius = activeMovement.startRadius;

    // Circle recomputes its radius live on every call when !useStartPoint (a pre-existing quirk,
    // see AnimatorMath.h) - only that case needs a fresh reference position; everything else
    // already has what it needs cached in `start` above.
    juce::Vector3D<double> currentReferencePosition;
    if (clip.movementType == MovementType::Circle && !clip.useStartPoint)
        currentReferencePosition = getGroupPosition(activeMovement.timelineIndex);

    return AnimatorMath::calculatePosition(clip, progress, start, currentReferencePosition);
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

void AnimatorEngine::setDisplayTimeInSeconds(bool enable)
{
    if (pAnimatorSettings == nullptr)
        return;

    bool change = (enable != pAnimatorSettings->displayTimeInSeconds);
    if (change)
    {
        pAnimatorSettings->displayTimeInSeconds = enable;
        sendChangeMessage();
    }
}

bool AnimatorEngine::getDisplayTimeInSeconds()
{
    return (pAnimatorSettings != nullptr && pAnimatorSettings->displayTimeInSeconds);
}

void AnimatorEngine::startActionClip(int timelineIndex, const ActionClip& clip, ms_t currentTimeMs, ms_t elapsedTime)
{
    // Remove any existing action for this timeline, clearing any jitter it left active first -
    // otherwise a source could be left stuck at a stray random offset if the replacing clip
    // doesn't also use Jitter.
    for (int i = activeActions.size() - 1; i >= 0; --i)
    {
        if (activeActions[i].timelineIndex == timelineIndex)
        {
            clearJitterOffsets(activeActions[i]);
            activeActions.remove(i);
        }
    }

    // Start new action
    ActiveAction newAction(timelineIndex, clip, currentTimeMs - elapsedTime, elapsedTime);

    // Keep initial state capture for stretch functionality
    if (pSourceSet && timelineIndex < pSourceSet->groupCount())
    {
        if (auto* group = pSourceSet->getActiveGroup(timelineIndex))
        {
            newAction.initialStretch = group->getStretch();
            newAction.hasInitialState = true;
        }
    }

    // Jitter: cache which sources belong to this group now, once, so processJitterAction() doesn't
    // need to rescan pSourceSet every call. There's no public "get group member at index" API - the
    // established pattern (GroupPointsSelectionComponent) is this reverse lookup instead.
    bool clipHasJitter = false;
    for (const auto& actionDef : clip.actions)
        if (actionDef.getAction() == ActionType::Jitter)
            clipHasJitter = true;

    if (clipHasJitter && pSourceSet)
    {
        if (auto* targetGroup = (timelineIndex < pSourceSet->groupCount()) ? pSourceSet->getActiveGroup(timelineIndex) : nullptr)
        {
            for (int i = 0; i < pSourceSet->size(); ++i)
            {
                if (auto* source = pSourceSet->get(i))
                {
                    if (source->getGroup() == targetGroup)
                        newAction.jitterSourceIndices.add(i);
                }
            }
            newAction.hasJitterState = true;
        }
    }

    activeActions.add(newAction);
}

void AnimatorEngine::clearJitterOffsets(const ActiveAction& activeAction)
{
    if (!pSourceSet || !activeAction.hasJitterState) return;

    for (int srcIndex : activeAction.jitterSourceIndices)
        if (auto* source = pSourceSet->get(srcIndex))
            source->setJitterOffset(juce::Vector3D<double>());
}

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
        
        // Calculate time delta since last processing for THIS action
        ms_t timeDelta = currentTimeMs - action.lastProcessTime;
        if (timeDelta <= 0) continue;

        // Muting can be toggled while this clip is already playing - check live rather than only
        // at scheduling time. Still advance lastProcessTime so an eventual unmute doesn't apply a
        // huge backlogged timeDelta all at once (rotation is accumulated incrementally).
        if (action.clip.muted)
        {
            action.lastProcessTime = currentTimeMs;
            clearJitterOffsets(action);
            continue;
        }

        // Calculate progress for clip timing
        ms_t timeInAction = currentTimeMs - action.actualStartTime;
        double progress = (action.clip.length > 0) ?
            static_cast<double>(timeInAction) / action.clip.length : 0.0;
        progress = juce::jlimit(0.0, 1.0, progress);
        
        // Accumulate rotations for this timeline (in radians)
        double xAngleRad = 0.0, yAngleRad = 0.0, zAngleRad = 0.0;
        
        // Process all actions in this clip
        for (const auto& actionDef : action.clip.actions)
        {
            if (actionDef.getAction() != ActionType::None)
            {
                if (actionDef.getAction() == ActionType::Stretch)
                {
                    // Keep full stretch functionality including initial state
                    processStretchAction(action.timelineIndex, actionDef, progress, action);
                }
                else if (actionDef.getAction() == ActionType::Jitter)
                {
                    processJitterAction(action.timelineIndex, actionDef, action, currentTimeMs);
                }
                else
                {
                    // Calculate angle based on time delta and timing type
                    double angleDeg = 0.0;
                    double timeDeltaSeconds = timeDelta / 1000.0;
                    
                    switch (actionDef.getTiming())
                    {
                        case TimingType::RelativeDuringClip:
                        {
                            // Calculate angle based on progress through clip
                            double totalAngle = actionDef.getValue();
                            double anglePerMs = totalAngle / action.clip.length;
                            angleDeg = anglePerMs * timeDelta;
                            break;
                        }
                            
                        case TimingType::ConstantPerSecond:
                        {
                            // Angle per second * time delta in seconds
                            angleDeg = actionDef.getValue() * timeDeltaSeconds;
                            break;
                        }
                            
                        case TimingType::AbsoluteTarget:
                            // Skip absolute targets for rotations
                            continue;
                            
                        case TimingType::None:
                            continue;
                    }
                    
                    // Convert to radians and accumulate
                    double angleRad = juce::degreesToRadians(angleDeg);
                    
                    switch (actionDef.getAction())
                    {
                        case ActionType::RotationX: xAngleRad += angleRad; break;
                        case ActionType::RotationY: yAngleRad += angleRad; break;
                        case ActionType::RotationZ: zAngleRad += angleRad; break;
                        case ActionType::Stretch:
                        case ActionType::None:
                        default:
                            break;
                    }
                }
            }
        }
        
        // Apply all accumulated rotations in a single call
        if (xAngleRad != 0.0 || yAngleRad != 0.0 || zAngleRad != 0.0)
        {
            pSourceSet->rotateGroup(action.timelineIndex, xAngleRad, yAngleRad, zAngleRad);
        }
        
        // Update last process time for THIS action
        action.lastProcessTime = currentTimeMs;
    }
}

void AnimatorEngine::processStretchAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction)
{
    if (!pSourceSet || timelineIndex >= pSourceSet->groupCount()) return;
    if (actionDef.getTiming() == TimingType::None) return; // matches the original: "leave stretch untouched", not "set it to 1.0"

    const double currentStretch = AnimatorMath::calculateStretch(actionDef, progress, activeAction.clip.length,
                                                                   activeAction.initialStretch, activeAction.hasInitialState);

    pSourceSet->setGroupStretch(timelineIndex, currentStretch, true);
}

void AnimatorEngine::processJitterAction(int /*timelineIndex*/, const ActionDefinition& actionDef, const ActiveAction& activeAction, ms_t currentTimeMs)
{
    if (!pSourceSet || !activeAction.hasJitterState) return;

    const double elapsedSeconds = (currentTimeMs - activeAction.actualStartTime) / 1000.0;
    const double t = elapsedSeconds * actionDef.getJitterSpeed();
    const double intensity = actionDef.getValue();

    for (int srcIndex : activeAction.jitterSourceIndices)
    {
        auto* source = pSourceSet->get(srcIndex);
        if (source == nullptr) continue;

        const double nx = PerlinNoise::noise1D(AnimatorMath::jitterSeed(activeAction.clip.id, srcIndex, 0), t);
        const double ny = PerlinNoise::noise1D(AnimatorMath::jitterSeed(activeAction.clip.id, srcIndex, 1), t);
        const double nz = PerlinNoise::noise1D(AnimatorMath::jitterSeed(activeAction.clip.id, srcIndex, 2), t);

        // Recomputed fresh from (seed, t) every call - never incremented - so this is automatically
        // seek/loop-safe, and stays a constant-amplitude wobble regardless of whatever the group's
        // own rotation/stretch/movement happen to be doing concurrently (getAbsSourcePoint() adds
        // this on top of that, rather than it being baked into the source's own stored position).
        source->setJitterOffset(juce::Vector3D<double>(nx, ny, nz) * intensity);
    }
}
