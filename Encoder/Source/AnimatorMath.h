#pragma once

#include "TimelineModel.h"

// Pure movement/action math shared by AnimatorEngine (real playback) and ClipPreviewComponent (the
// clip editors' looping preview), so the two can never silently drift apart. Every function here is
// a pure function of its arguments - no AmbiSourceSet/live engine state - by design: that's what
// lets the preview simulate a clip in isolation, and what makes AnimatorEngine's own use of these
// functions trivially seek/loop-safe (same principle established elsewhere in this engine: values
// recomputed fresh from (clip, progress) are safe, incremental accumulation is not - the one
// exception is rotation, see calculateRotationTickRadians below).
namespace AnimatorMath
{
    struct MovementStartState
    {
        juce::Vector3D<double> initialPosition; // for MoveToCartesian/MoveToPolar
        double startAngle = 0.0;                // for Circle/Spiral
        double startRadius = 0.0;               // for Spiral
    };

    // Mirrors AnimatorEngine::startMovementClip's one-time capture logic. referencePosition is
    // whatever the group's position should be treated as "now" when clip.useStartPoint is false -
    // the real engine passes the live group position; a preview passes its own tracked/synthetic
    // reference.
    inline MovementStartState computeMovementStartState(const MovementClip& clip, juce::Vector3D<double> referencePosition)
    {
        MovementStartState state;

        if (clip.movementType == MovementType::MoveToCartesian || clip.movementType == MovementType::MoveToPolar)
        {
            state.initialPosition = clip.useStartPoint
                ? juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ())
                : referencePosition;
        }

        if (clip.movementType == MovementType::Circle || clip.movementType == MovementType::Spiral)
        {
            auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
            juce::Vector3D<double> startPos = clip.useStartPoint
                ? juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ())
                : referencePosition;

            const double relX = startPos.x - centerPos.x;
            const double relY = startPos.y - centerPos.y;
            state.startAngle = std::atan2(relY, relX);

            if (clip.movementType == MovementType::Spiral)
                state.startRadius = std::sqrt(relX * relX + relY * relY);
        }

        return state;
    }

    // One clip-relative instant's state within its repeat/palindrome cycle, given the clip's own
    // clamped-[0,1] progress (time-in-clip / clip.length - the same value every caller in this file
    // already computes). repeatCount is defensively re-clamped to >=1 here even though the UI and
    // XML loader already enforce that, so a hand-edited/corrupt value can never read out of bounds.
    //
    // When palindrome is false this is an N-fold sawtooth: repeatCount equal-duration segments,
    // each replaying cycleProgress 0->1 forward. When palindrome is true there are 2*repeatCount
    // equal-duration segments alternating forward/backward (a triangle wave): even-indexed segments
    // run cycleProgress 0->1 forward, odd-indexed segments run cycleProgress 1->0 backward.
    //
    // Used two different ways by the two different kinds of math in this file:
    //  - Movement (calculatePosition, which applies this to its own progress on every call) and
    //    Stretch (calculateStretch's callers transform progress before calling it) are pure
    //    functions of progress: feeding .cycleProgress in place of the raw clip progress
    //    replays/palindromes the curve with zero changes to either function's own internal math,
    //    and every segment boundary has matching cycleProgress on both sides, so there's never a
    //    visible glitch at a boundary (e.g. repeatCount=2, palindrome=true: progress 0.5 sits at
    //    the end of segment 1 - backward, local=1.0, cycleProgress=0.0 - and the start of segment 2
    //    - forward, local=0.0, cycleProgress=0.0 - matching exactly).
    //  - Rotation is NOT a pure function of progress (see calculateRotationTickRadians) - it
    //    accumulates a per-tick delta applied as a RELATIVE rotation onto the group's current
    //    orientation, not an absolute set. Multiply that per-tick delta by .direction instead:
    //    every forward/backward segment pair has identical duration and rate, so the backward half
    //    exactly cancels the forward half - flipping the sign at each boundary is seamless at any
    //    repeatCount, landing back on the exact starting angle, with no "undo" correction needed.
    //    This is why Rotation only ever supports Palindrome, never a bare repeatCount > 1 without
    //    it - a bare repeat would need exactly that kind of undo, deliberately not implemented here.
    //
    // At repeatCount==1, palindrome==false (every clip that predates this feature, via XML
    // defaults), this is an exact no-op: cycleProgress==progress, direction==1.0 always.
    struct CycleState
    {
        double cycleProgress = 0.0; // in [0,1] - feed straight into calculatePosition/calculateStretch
        double direction = 1.0;     // +1.0 forward, -1.0 backward - multiply Rotation's per-tick delta by this
    };

    inline CycleState computeCycleState(double progress, int repeatCount, bool palindrome)
    {
        repeatCount = juce::jmax(1, repeatCount);
        const int totalSegments = palindrome ? (2 * repeatCount) : repeatCount;

        progress = juce::jlimit(0.0, 1.0, progress);
        const double scaled = progress * (double)totalSegments;
        // progress==1.0 (and float error landing exactly on a boundary) must resolve to the end of
        // the last segment, not the start of a (totalSegments)-th segment that doesn't exist.
        int segmentIndex = juce::jlimit(0, totalSegments - 1, (int)std::floor(scaled));
        const double segmentLocalProgress = juce::jlimit(0.0, 1.0, scaled - (double)segmentIndex);
        const bool isBackward = palindrome && ((segmentIndex % 2) == 1);

        CycleState state;
        state.cycleProgress = isBackward ? (1.0 - segmentLocalProgress) : segmentLocalProgress;
        state.direction = isBackward ? -1.0 : 1.0;
        return state;
    }

    inline juce::Vector3D<double> cartesianToSpherical(const juce::Vector3D<double>& cartesian)
    {
        const double x = cartesian.x, y = cartesian.y, z = cartesian.z;
        const double distance = std::sqrt(x * x + y * y + z * z);

        if (distance < 1e-12) return juce::Vector3D<double>(0.0, 0.0, 0.0);

        const double azimuth = std::atan2(y, x);
        const double elevation = std::asin(z / distance);

        return juce::Vector3D<double>(azimuth, elevation, distance);
    }

    inline juce::Vector3D<double> sphericalToCartesian(const juce::Vector3D<double>& spherical)
    {
        const double azimuth = spherical.x, elevation = spherical.y, distance = spherical.z;

        if (distance < 1e-12) return juce::Vector3D<double>(0.0, 0.0, 0.0);

        const double x = distance * std::cos(elevation) * std::cos(azimuth);
        const double y = distance * std::cos(elevation) * std::sin(azimuth);
        const double z = distance * std::sin(elevation);

        return juce::Vector3D<double>(x, y, z);
    }

    // currentReferencePosition is re-passed on every call (not folded into MovementStartState)
    // specifically to preserve an existing quirk: Circle recomputes its radius live on every call
    // when !useStartPoint (AnimatorEngine's original calculateCircle), unlike Spiral which caches
    // startRadius once. That asymmetry is deliberate/pre-existing behaviour, not a bug to fix here.
    inline juce::Vector3D<double> calculatePosition(const MovementClip& clip, double progress,
                                                      const MovementStartState& start, juce::Vector3D<double> currentReferencePosition)
    {
        // Repetitions/Palindrome compress N (or 2N, if palindrome) replays of this progress-driven
        // curve into the clip's existing length - orthogonal to clip.count (Circle/Spiral's own
        // windings-per-pass), which still multiplies within each replay below.
        progress = computeCycleState(progress, clip.repetitions, clip.palindrome).cycleProgress;

        switch (clip.movementType)
        {
            case MovementType::MoveToCartesian:
            {
                auto targetPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
                return start.initialPosition + (targetPos - start.initialPosition) * progress;
            }

            case MovementType::MoveToPolar:
            {
                auto targetPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
                auto startSpherical = cartesianToSpherical(start.initialPosition);
                auto targetSpherical = cartesianToSpherical(targetPos);

                const double startAzimuth = startSpherical.x;
                const double targetAzimuth = targetSpherical.x;
                double angularDist = targetAzimuth - startAzimuth;

                if (angularDist > juce::MathConstants<double>::pi)
                    angularDist -= 2.0 * juce::MathConstants<double>::pi;
                else if (angularDist < -juce::MathConstants<double>::pi)
                    angularDist += 2.0 * juce::MathConstants<double>::pi;

                const double interpAzimuth = startAzimuth + angularDist * progress;
                const double interpElevation = startSpherical.y + (targetSpherical.y - startSpherical.y) * progress;
                const double interpDistance = startSpherical.z + (targetSpherical.z - startSpherical.z) * progress;

                return sphericalToCartesian(juce::Vector3D<double>(interpAzimuth, interpElevation, interpDistance));
            }

            case MovementType::Circle:
            {
                auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());

                double radius;
                if (clip.useStartPoint)
                {
                    auto startPos = juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ());
                    auto diff = startPos - centerPos;
                    radius = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
                }
                else
                {
                    auto diff = currentReferencePosition - centerPos;
                    radius = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
                }

                const double angle = start.startAngle - (2.0 * juce::MathConstants<double>::pi * clip.count * progress);

                return juce::Vector3D<double>(
                    centerPos.x + radius * std::cos(angle),
                    centerPos.y + radius * std::sin(angle),
                    centerPos.z
                );
            }

            case MovementType::Spiral:
            {
                auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());

                const double direction = (clip.count >= 0) ? -1.0 : 1.0;
                const double absoluteCount = std::abs(clip.count);

                const double angle = start.startAngle + (2.0 * juce::MathConstants<double>::pi * absoluteCount * progress * direction);

                const double totalRadiusChange = clip.radiusChange * absoluteCount;
                double currentRadius = start.startRadius + (totalRadiusChange * progress);

                if (currentRadius < 0.0)
                    currentRadius = 0.0;

                return juce::Vector3D<double>(
                    centerPos.x + currentRadius * std::cos(angle),
                    centerPos.y + currentRadius * std::sin(angle),
                    centerPos.z
                );
            }

            default:
                return juce::Vector3D<double>(0.0, 0.0, 0.0);
        }
    }

    // Pure function of (actionDef, progress, clip length, initial stretch). Callers must skip
    // calling this entirely for TimingType::None (matching the original's early-return there - it
    // means "leave stretch untouched", not "set it to 1.0").
    inline double calculateStretch(const ActionDefinition& actionDef, double progress, ms_t clipLengthMs,
                                    double initialStretch, bool hasInitialState)
    {
        double currentStretch = 1.0;

        switch (actionDef.getTiming())
        {
            case TimingType::AbsoluteTarget:
            {
                if (actionDef.getUseStartValue())
                    currentStretch = actionDef.getStartValue() + (actionDef.getValue() - actionDef.getStartValue()) * progress;
                else if (hasInitialState)
                    currentStretch = initialStretch + (actionDef.getValue() - initialStretch) * progress;
                else
                    currentStretch = actionDef.getValue();
                break;
            }

            case TimingType::RelativeDuringClip:
            {
                currentStretch = 1.0 + actionDef.getValue() * progress;
                if (actionDef.getUseStartValue())
                    currentStretch = actionDef.getStartValue() * (1.0 + actionDef.getValue() * progress);
                else if (hasInitialState)
                    currentStretch = initialStretch * (1.0 + actionDef.getValue() * progress);
                break;
            }

            case TimingType::ConstantPerSecond:
            {
                const double clipDurationSeconds = clipLengthMs / 1000.0;
                const double elapsedSeconds = clipDurationSeconds * progress;

                if (actionDef.getUseStartValue())
                    currentStretch = actionDef.getStartValue() * (1.0 + actionDef.getValue() * elapsedSeconds);
                else if (hasInitialState)
                    currentStretch = initialStretch * (1.0 + actionDef.getValue() * elapsedSeconds);
                else
                    currentStretch = 1.0 + actionDef.getValue() * elapsedSeconds;
                break;
            }

            case TimingType::None:
            default:
                currentStretch = 1.0;
                break;
        }

        if (currentStretch < 0.01) currentStretch = 0.01;
        return currentStretch;
    }

    // One ActionDefinition's contribution to one tick's rotation increment (radians, per axis).
    // Rotation is NOT a pure function of absolute progress in this engine - it's accumulated
    // incrementally, tick by tick, from wall-clock time deltas - so unlike the functions above,
    // callers must sum this across every tick since the clip/preview started, not call it once.
    inline juce::Vector3D<double> calculateRotationTickRadians(const ActionDefinition& actionDef, ms_t timeDeltaMs, ms_t clipLengthMs)
    {
        double angleDeg = 0.0;
        const double timeDeltaSeconds = timeDeltaMs / 1000.0;

        switch (actionDef.getTiming())
        {
            case TimingType::RelativeDuringClip:
            {
                const double totalAngle = actionDef.getValue();
                const double anglePerMs = totalAngle / (double)clipLengthMs;
                angleDeg = anglePerMs * (double)timeDeltaMs;
                break;
            }

            case TimingType::ConstantPerSecond:
                angleDeg = actionDef.getValue() * timeDeltaSeconds;
                break;

            case TimingType::AbsoluteTarget: // not supported for rotation (matches the real engine)
            case TimingType::None:
            default:
                return juce::Vector3D<double>(0.0, 0.0, 0.0);
        }

        const double angleRad = juce::degreesToRadians(angleDeg);

        switch (actionDef.getAction())
        {
            case ActionType::RotationX: return juce::Vector3D<double>(angleRad, 0.0, 0.0);
            case ActionType::RotationY: return juce::Vector3D<double>(0.0, angleRad, 0.0);
            case ActionType::RotationZ: return juce::Vector3D<double>(0.0, 0.0, angleRad);
            case ActionType::Stretch:
            case ActionType::Jitter:
            case ActionType::None:
            default:
                return juce::Vector3D<double>(0.0, 0.0, 0.0);
        }
    }

    // Deterministic per-(clip, source/member, axis) seed for PerlinNoise::noise1D, promoted
    // verbatim from AnimatorEngine.cpp so the preview derives identical-shaped noise without
    // duplicating the hash formula.
    inline uint32_t jitterSeed(const juce::String& clipId, int sourceIndex, int axis)
    {
        return (uint32_t)clipId.hashCode()
             ^ ((uint32_t)sourceIndex * 2654435761u)
             ^ ((uint32_t)axis * 0x9E3779B9u);
    }
}
