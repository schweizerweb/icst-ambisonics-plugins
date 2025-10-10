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
    bool getAnimatorState();

private:
    struct ActiveMovement
    {
        int timelineIndex;
        MovementClip clip;
        ms_t actualStartTime;
        bool isPrerendered;
        juce::Array<juce::Vector3D<double>> prerenderedFrames;
        double startAngle; // For circle/spiral
        double startRadius; // For spiral
        juce::Vector3D<double> initialPosition; // Store the initial position for MoveTo operations
        
        ActiveMovement()
            : timelineIndex(0), actualStartTime(0), isPrerendered(false), startAngle(0.0)
        {}
        
        ActiveMovement(int idx, const MovementClip& c, ms_t actualStart)
            : timelineIndex(idx), clip(c), actualStartTime(actualStart), isPrerendered(false), startAngle(0.0)
        {}
        
        double getProgress(ms_t currentTimeMs) const
        {
            ms_t elapsed = currentTimeMs - actualStartTime;
            if (elapsed < 0) return 0.0;
            if (elapsed > clip.length) return 1.0;
            double progress = static_cast<double>(elapsed) / clip.length;
            return juce::jlimit(0.0, 1.0, progress);
        }
    };

    // Private methods
    void copyTimelines(juce::OwnedArray<TimelineModel>* sourceTimelines);
    void preRenderMovements();
    void preRenderMovementClip(int timelineIndex, const MovementClip& clip);
    void updateActiveMovements(ms_t currentTimeMs);
    void startMovementClip(int timelineIndex, const MovementClip& clip, ms_t currentTimeMs);
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
