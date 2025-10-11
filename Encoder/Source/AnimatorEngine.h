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

    struct ActiveAction
    {
        int timelineIndex;
        ActionClip clip;
        ms_t actualStartTime;
        ms_t elapsedTime = 0;
        ms_t lastProcessTime = 0;
        
        double initialStretch = 1.0;
        bool hasInitialState = false;
        
        // Default constructor
        ActiveAction() = default;
        
        // Constructor without elapsed time
        ActiveAction(int idx, const ActionClip& c, ms_t start)
            : timelineIndex(idx), clip(c), actualStartTime(start), elapsedTime(0),
            lastProcessTime(start) {}
            
        // Constructor with elapsed time
        ActiveAction(int idx, const ActionClip& c, ms_t start, ms_t elapsed)
            : timelineIndex(idx), clip(c), actualStartTime(start), elapsedTime(elapsed),
            lastProcessTime(start) {}
    };
    
    // Private methods
    void copyTimelines(juce::OwnedArray<TimelineModel>* sourceTimelines);
    void preRenderMovements();
    void preRenderMovementClip(int timelineIndex, const MovementClip& clip);
    void updateActiveMovements(ms_t currentTimeMs);
    void startMovementClip(int timelineIndex, const MovementClip& clip, ms_t currentTimeMs, ms_t elapsedTime);
    void startActionClip(int timelineIndex, const ActionClip& clip, ms_t currentTimeMs, ms_t elapsedTime);
    void processActiveMovements(ms_t currentTimeMs);
    void processActiveActions(ms_t currentTimeMs);
    void processSingleAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction);
    void processRotationAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction);
    void processStretchAction(int timelineIndex, const ActionDefinition& actionDef, double progress, const ActiveAction& activeAction);
    
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
    juce::Array<ActiveAction> activeActions;
    
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
    
    class QuaternionHelper
    {
    public:
        static double vectorDot(const juce::Vector3D<double>& v1, const juce::Vector3D<double>& v2)
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }
        
        static juce::Vector3D<double> vectorCross(const juce::Vector3D<double>& v1, const juce::Vector3D<double>& v2)
        {
            return juce::Vector3D<double>(
                                          v1.y * v2.z - v1.z * v2.y,
                                          v1.z * v2.x - v1.x * v2.z,
                                          v1.x * v2.y - v1.y * v2.x
                                          );
        }
        
        static juce::Quaternion<double> multiplyQuaternions(const juce::Quaternion<double>& q1, const juce::Quaternion<double>& q2)
        {
            // Quaternion multiplication: q1 * q2
            double w = q1.scalar * q2.scalar - vectorDot(q1.vector, q2.vector);
            juce::Vector3D<double> v = q2.vector * q1.scalar + q1.vector * q2.scalar + vectorCross(q1.vector, q2.vector);
            return juce::Quaternion<double>(v, w);
        }
    };
};
