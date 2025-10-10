#pragma once

#include <JuceHeader.h>
#include "TimelineModel.h"
#include "../../Common/AmbiSourceSet.h"
#include "EncoderSettings.h"

class AnimatorEngine: public ChangeBroadcaster
{
public:
    AnimatorEngine();
    
    void reset(juce::OwnedArray<TimelineModel>* timelines, AmbiSourceSet* sourceSet, double sampleRate, AnimatorSettings* animatorSettings);
    void processAnimationAt(ms_t positionMs);
    
    void setAnimatorState(bool enable);
    void setAutoFollow(bool enable);
    
    bool getAnimatorState();
    bool getAutoFollow();

private:
    struct ActiveMovement
    {
        int timelineIndex;
        MovementClip clip;
        ms_t actualStartTime; // The actual time when this movement should have started
        ms_t elapsedTime = 0; // How much time has already elapsed when starting
        
        juce::Vector3D<double> initialPosition;
        double startAngle = 0.0;
        double startRadius = 0.0;
        
        // Original constructor (for when no elapsed time)
        ActiveMovement(int idx, const MovementClip& c, ms_t start)
            : timelineIndex(idx), clip(c), actualStartTime(start), elapsedTime(0) {}
        
        // New constructor with elapsed time
        ActiveMovement(int idx, const MovementClip& c, ms_t start, ms_t elapsed)
            : timelineIndex(idx), clip(c), actualStartTime(start), elapsedTime(elapsed) {}
        
        ActiveMovement()
            : timelineIndex(0), actualStartTime(0), elapsedTime(0)
                {}
        double getProgress(ms_t currentTimeMs) const
        {
            ms_t timeInMovement = currentTimeMs - actualStartTime;
            return static_cast<double>(timeInMovement) / clip.length;
        }
    };

    // Private methods
    void copyTimelines(juce::OwnedArray<TimelineModel>* sourceTimelines);
    void preRenderMovements();
    void preRenderMovementClip(int timelineIndex, const MovementClip& clip);
    void updateActiveMovements(ms_t currentTimeMs);
    void startMovementClip(int timelineIndex, const MovementClip& clip, ms_t currentTimeMs, ms_t elapsedTime);
    void processActiveMovements(ms_t currentTimeMs);
    
    // Movement calculation
    juce::Vector3D<double> calculateMovementPosition(const ActiveMovement& activeMovement, double progress);
    juce::Vector3D<double> calculateLinearCartesian(const MovementClip& clip, double progress, int timelineIndex);
    juce::Vector3D<double> calculateLinearPolar(const MovementClip& clip, double progress, int timelineIndex);
    juce::Vector3D<double> calculateCircle(const MovementClip& clip, double progress, int timelineIndex);
    juce::Vector3D<double> calculateSpiral(const MovementClip& clip, double progress, int timelineIndex);
    juce::Vector3D<double> getGroupPosition(int timelineIndex);
    
    // Coordinate conversion
    juce::Vector3D<double> cartesianToSpherical(const juce::Vector3D<double>& cartesian);
    juce::Vector3D<double> sphericalToCartesian(const juce::Vector3D<double>& spherical);
    double calculateDistance(const juce::Vector3D<double>& a, const juce::Vector3D<double>& b) const;

    // Member variables
    bool needsPreRender = false;
    ms_t lastPositionMs = 0;
    double sampleRate = 44100.0;
    AmbiSourceSet* pSourceSet = nullptr;
    AnimatorSettings* pAnimatorSettings = nullptr;
    
    juce::OwnedArray<TimelineModel> copiedTimelines;
    juce::Array<ActiveMovement> activeMovements;
    
    // For efficient clip lookup
    struct ClipSchedule
    {
        int timelineIndex;
        int clipIndex;
        ms_t start;
        ms_t end;
        bool isMovement;
        
        // Add default constructor
        ClipSchedule()
            : timelineIndex(0), clipIndex(0), start(0), end(0), isMovement(false)
        {}
        
        // Parameterized constructor
        ClipSchedule(int tIdx, int cIdx, ms_t s, ms_t e, bool isMov)
            : timelineIndex(tIdx), clipIndex(cIdx), start(s), end(e), isMovement(isMov)
        {}
        
        bool operator<(const ClipSchedule& other) const { return start < other.start; }
    };
    
    juce::Array<ClipSchedule> clipSchedule;
    int nextScheduledClipIndex = 0;
};
