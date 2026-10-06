#pragma once

#include "TimelineModel.h"
#include "../../Common/PerlinNoise.h"

// Pure movement/action math shared by AnimatorEngine (real playback) and ClipPreviewComponent (the
// clip editors' looping preview), so the two can never silently drift apart. Every function here is
// a pure function of its arguments - no AmbiSourceSet/live engine state - by design: that's what
// lets the preview simulate a clip in isolation, and what makes AnimatorEngine's own use of these
// functions trivially seek/loop-safe (same principle established elsewhere in this engine: values
// recomputed fresh from (clip, progress) are safe, incremental accumulation is not). Rotation used
// to be the one exception - it accumulated per-tick deltas from wall-clock time - which is exactly
// why it couldn't support start values or survive a seek; computeClipRotation below replaces that
// with an absolute orientation computed purely from progress, so the rule now has no exceptions.
namespace AnimatorMath
{
    struct MovementStartState
    {
        juce::Vector3D<double> initialPosition; // MoveTo*, Spline/Polygon (empty-path fallback), RandomWalk
        double startAngle = 0.0;                // Circle/Spiral/Rose/Helix
        double startRadius = 0.0;               // Spiral/Helix
        juce::Vector3D<double> startOffset;     // start point relative to the centre - every centre-relative type
    };

    // Mirrors AnimatorEngine::startMovementClip's one-time capture logic. referencePosition is
    // whatever the group's position should be treated as "now" when clip.useStartPoint is false -
    // the real engine passes the live group position; a preview passes its own tracked/synthetic
    // reference.
    inline MovementStartState computeMovementStartState(const MovementClip& clip, juce::Vector3D<double> referencePosition)
    {
        MovementStartState state;

        const juce::Vector3D<double> startPos = clip.useStartPoint
            ? juce::Vector3D<double>(clip.startPointGroup.getX(), clip.startPointGroup.getY(), clip.startPointGroup.getZ())
            : referencePosition;

        // Captured for MoveTo (its literal starting point) and as the "hold here" fallback for a
        // waypoint path with no usable waypoints, and as RandomWalk's anchor.
        state.initialPosition = startPos;

        if (movementTypeUsesTargetAsCentre(clip.movementType))
        {
            auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());
            state.startOffset = startPos - centerPos;

            state.startAngle = std::atan2(state.startOffset.y, state.startOffset.x);
            state.startRadius = std::sqrt(state.startOffset.x * state.startOffset.x
                                          + state.startOffset.y * state.startOffset.y);
        }

        return state;
    }

    // Deterministic per-(seed, axis) hash for RandomWalk's noise lookups. Deliberately NOT derived
    // the way jitterSeed() is (which folds in the clip id): the user types this seed into the clip,
    // so the same seed must give the same path in every clip. Also deliberately not `seed, seed+1,
    // seed+2` - PerlinNoise's own hash mixes seed ^ index, so consecutive seeds share lattice
    // gradients and the X/Y/Z channels would come out visibly correlated (a diagonal drift).
    inline juce::uint32 walkSeed(int userSeed, int axis)
    {
        auto h = static_cast<juce::uint32>(userSeed) * 2654435761u;
        h ^= static_cast<juce::uint32>(axis) * 0x9E3779B9u;
        h ^= h >> 15;
        h *= 0x2545F491u;
        h ^= h >> 13;
        return h;
    }

    // --- Speed curves (easing) -----------------------------------------------------------------
    //
    // A user-designed cubic Bezier mapping elapsed time (x) to distance covered (y), with the ends
    // pinned at (0,0) and (1,1) and two draggable control handles - the CSS / After Effects model.
    //
    // Two properties are guaranteed by clamping all four coordinates to [0,1], and both matter:
    //  - x(t) is monotonically non-decreasing, so TIME CAN NEVER RUN BACKWARDS. Worst case x1=1,
    //    x2=0 gives x'(t) = 3(1-2t)^2 >= 0. This is exactly the hazard that ruled out a free
    //    multi-point spline, where an ordered set of points can still bulge backwards in x.
    //  - y(t) stays inside [0,1], since a Bezier lies within the convex hull of its control points.
    //    So overshoot/anticipation curves are deliberately NOT expressible: computeCycleState()
    //    clamps progress to [0,1], and an overshoot would be silently clipped rather than rendered.
    struct EasingCurve
    {
        // Defaults are the exact diagonal - y(x) == x - so an untouched curve is a true no-op and
        // every project that predates this feature plays bit-for-bit as before.
        double x1 = 1.0 / 3.0, y1 = 1.0 / 3.0;
        double x2 = 2.0 / 3.0, y2 = 2.0 / 3.0;
        bool enabled = false;
        bool perRepetition = true;
    };

    // Distance covered at elapsed fraction x. The curve is parametric, so this first solves
    // x(t) = x for t (Newton-Raphson, falling back to bisection where the derivative is flat - the
    // standard WebKit UnitBezier approach), then evaluates y(t).
    inline double bezierEase(const EasingCurve& curve, double x)
    {
        const double x1 = juce::jlimit(0.0, 1.0, curve.x1);
        const double y1 = juce::jlimit(0.0, 1.0, curve.y1);
        const double x2 = juce::jlimit(0.0, 1.0, curve.x2);
        const double y2 = juce::jlimit(0.0, 1.0, curve.y2);

        x = juce::jlimit(0.0, 1.0, x);

        // Endpoints are pinned, and resolving them exactly (rather than via the solver) is what
        // keeps a clip starting and finishing precisely on its own endpoints.
        if (x <= 0.0) return 0.0;
        if (x >= 1.0) return 1.0;

        // Polynomial form of a cubic Bezier with P0=(0,0), P3=(1,1).
        const double cx = 3.0 * x1, bx = 3.0 * (x2 - x1) - cx, ax = 1.0 - cx - bx;
        const double cy = 3.0 * y1, by = 3.0 * (y2 - y1) - cy, ay = 1.0 - cy - by;

        auto sampleX = [&](double t) { return ((ax * t + bx) * t + cx) * t; };
        auto sampleY = [&](double t) { return ((ay * t + by) * t + cy) * t; };
        auto sampledX = [&](double t) { return (3.0 * ax * t + 2.0 * bx) * t + cx; };

        double t = x; // x is a good first guess, since the curve never strays far from the diagonal

        for (int i = 0; i < 8; ++i)
        {
            const double error = sampleX(t) - x;
            if (std::abs(error) < 1e-9)
                return sampleY(t);

            const double derivative = sampledX(t);
            if (std::abs(derivative) < 1e-9)
                break; // flat spot - Newton would diverge, hand over to bisection

            t -= error / derivative;
        }

        double low = 0.0, high = 1.0;
        t = juce::jlimit(0.0, 1.0, t);

        for (int i = 0; i < 32; ++i)
        {
            const double value = sampleX(t);
            if (std::abs(value - x) < 1e-9)
                break;

            if (value < x) low = t; else high = t;
            t = 0.5 * (low + high);
        }

        return sampleY(t);
    }

    // Warps RAW clip progress and returns RAW clip progress, which is the whole trick: everything
    // downstream (computeCycleState, rotationPhase, calculatePosition's own internal cycle handling)
    // then works completely unchanged, and both scopes are just two different warps.
    //
    //  - Whole clip:      y(progress). Time is warped across all repetitions, so the repetitions
    //                     themselves speed up or slow down over the clip.
    //  - Per repetition:  the warp is applied inside each segment. This is EXACT, not an
    //                     approximation: computeCycleState() recovers the same segment index k and
    //                     the same direction from the warped value, and simply sees local progress
    //                     y(f) where it would have seen f. Palindrome therefore keeps working - a
    //                     backward segment yields 1 - y(f), i.e. the return leg eases too.
    inline double applyEasing(const EasingCurve& curve, double progress, int repeatCount, bool palindrome)
    {
        if (!curve.enabled)
            return progress;

        progress = juce::jlimit(0.0, 1.0, progress);

        if (!curve.perRepetition)
            return bezierEase(curve, progress);

        const int totalSegments = juce::jmax(1, repeatCount) * (palindrome ? 2 : 1);
        if (totalSegments <= 1)
            return bezierEase(curve, progress);

        const double scaled = progress * (double)totalSegments;

        // Matching computeCycleState's own clamp, so progress == 1.0 resolves to the END of the last
        // segment rather than the start of one that doesn't exist.
        const double segmentIndex = juce::jlimit(0.0, (double)(totalSegments - 1), std::floor(scaled));
        const double local = juce::jlimit(0.0, 1.0, scaled - segmentIndex);

        return (segmentIndex + bezierEase(curve, local)) / (double)totalSegments;
    }

    // The form every caller actually uses: the clip carries both the curve and the repeat/palindrome
    // settings the per-repetition scope needs, so warping a clip's progress is a single call. The
    // four-argument version above stays separate because the curve editor needs to evaluate a curve
    // that isn't attached to a clip yet.
    inline double applyEasing(const Clip& clip, double progress)
    {
        EasingCurve curve;
        curve.x1 = clip.easeX1;
        curve.y1 = clip.easeY1;
        curve.x2 = clip.easeX2;
        curve.y2 = clip.easeY2;
        curve.enabled = clip.easingEnabled;
        curve.perRepetition = clip.easePerRepetition;

        return applyEasing(curve, progress, clip.repetitions, clip.palindrome);
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
    //  - Rotation does NOT use this - see rotationPhase() below. Rotation is cyclic, so "repeat"
    //    naturally means CONTINUE (three repeats of 360 degrees = three consecutive turns), whereas
    //    cycleProgress would restart the sweep and snap back at every segment boundary. Palindrome
    //    does want exactly this triangle wave though, so rotationPhase() reuses .cycleProgress for
    //    that case and only replaces the non-palindrome sawtooth.
    //
    // At repeatCount==1, palindrome==false (every clip that predates this feature, via XML
    // defaults), this is an exact no-op: cycleProgress==progress, direction==1.0 always.
    struct CycleState
    {
        double cycleProgress = 0.0; // in [0,1] - feed straight into calculatePosition/calculateStretch
        double direction = 1.0;     // +1.0 forward, -1.0 backward
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

    // Rotation's equivalent of computeCycleState, returning a "phase" to feed calculateRotationDegrees.
    // Unlike cycleProgress this is NOT bounded to [0,1]: for a bare repeatCount it ramps 0->repeatCount
    // so N repeats of a 360 degree rotation are N continuous turns rather than N restarts. That matters
    // because a rotation's value is an ANGLE SWEPT, not a position on a closed curve - restarting it
    // would snap the group back at every segment boundary, which is why rotation used to be barred
    // from a bare repeatCount > 1 altogether.
    //
    // Palindrome keeps the triangle wave from computeCycleState: the sweep runs out to the full angle
    // and back to zero, landing exactly on the starting orientation, repeatCount times.
    inline double rotationPhase(double progress, int repeatCount, bool palindrome)
    {
        repeatCount = juce::jmax(1, repeatCount);
        progress = juce::jlimit(0.0, 1.0, progress);

        if (palindrome)
            return computeCycleState(progress, repeatCount, true).cycleProgress;

        return progress * (double)repeatCount;
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

    // --- Spline/Polygon waypoint paths ---------------------------------------------------------
    //
    // A waypoint list is split into "sub-paths": maximal runs where only the first waypoint carries
    // startsNewSegment (index 0 always starts one, whatever its own flag says). Each sub-path gets
    // its OWN arc-length table, so the boundary between two sub-paths contributes zero length and
    // therefore consumes zero time - which is precisely the instant jump the multi-segment feature
    // is meant to produce. A single flat table would instead yield two samples with equal
    // cumulative length, and any "skip zero-length intervals" rule would silently swallow the first
    // point of every new sub-path.
    //
    // Progress maps to distance along the path rather than to span index, so the group travels at a
    // uniform speed regardless of how unevenly the waypoints are spaced. Samples store
    // (spanIndex, u) rather than a baked position, so lookup re-evaluates the curve exactly instead
    // of lerping between polyline samples - no faceting even at a low sample count.
    struct WaypointPath
    {
        struct Sample
        {
            int spanIndex = 0;       // span from waypoint spanIndex -> spanIndex+1
            double u = 0.0;          // local parameter within that span, 0..1
            double cumulative = 0.0; // arc length from the start of this sub-path
        };

        struct SubPath
        {
            juce::Array<Sample> samples;
            double length = 0.0;
            double offset = 0.0;  // total length of all preceding sub-paths
            int firstIndex = 0;
            int lastIndex = 0;
        };

        juce::Array<MovementWaypoint> points; // copied, so the path is safe to outlive its clip
        juce::Array<SubPath> subPaths;
        double totalLength = 0.0;
        bool isSpline = false;
        double tension = 0.0;
    };

    inline juce::Vector3D<double> waypointAt(const juce::Array<MovementWaypoint>& points, int index)
    {
        if (points.isEmpty()) return {};
        const auto& wp = points.getReference(juce::jlimit(0, points.size() - 1, index));
        return { wp.x, wp.y, wp.z };
    }

    // One span of a waypoint path. Polygon lerps; Spline uses a Cardinal (tension-weighted
    // Catmull-Rom) Hermite, with the neighbour points clamped to THIS sub-path so a segment break
    // can't bend the curve toward a point it should have jumped away from. Duplicating the end
    // points (rather than reflecting them) keeps the curve inside the hull at the ends.
    //
    // At tension 1 both tangents vanish and the Hermite collapses to the straight chord, so a
    // tension-1 Spline is geometrically identical to a Polygon - the easing the basis would
    // otherwise introduce is erased by the arc-length reparameterisation.
    inline juce::Vector3D<double> evaluateWaypointSpan(const WaypointPath& path, int spanIndex, double u,
                                                        int subFirst, int subLast)
    {
        const auto p1 = waypointAt(path.points, spanIndex);
        const auto p2 = waypointAt(path.points, spanIndex + 1);

        if (!path.isSpline)
            return p1 + (p2 - p1) * u;

        const auto p0 = waypointAt(path.points, juce::jmax(subFirst, spanIndex - 1));
        const auto p3 = waypointAt(path.points, juce::jmin(subLast, spanIndex + 2));

        const double c = juce::jlimit(0.0, 1.0, path.tension);
        const auto m1 = (p2 - p0) * (0.5 * (1.0 - c));
        const auto m2 = (p3 - p1) * (0.5 * (1.0 - c));

        const double u2 = u * u;
        const double u3 = u2 * u;
        const double h00 =  2.0 * u3 - 3.0 * u2 + 1.0;
        const double h10 =        u3 - 2.0 * u2 + u;
        const double h01 = -2.0 * u3 + 3.0 * u2;
        const double h11 =        u3 -       u2;

        return p1 * h00 + m1 * h10 + p2 * h01 + m2 * h11;
    }

    // Sampling note: the table stores arc length at discrete u values and the lookup interpolates
    // linearly between them, so the SHAPE is exact (every returned point lies on the real curve,
    // because the span is re-evaluated rather than lerped) while the SPEED is accurate to about
    // O(1/N^2). At N=16 that's well under 1% of path length even in the worst case (tension 1,
    // whose smoothstep basis is the most extreme reparameterisation) - imperceptible, and not worth
    // doubling the sample count for. Verified numerically; don't mistake it for a bug.
    inline WaypointPath buildWaypointPath(const MovementClip& clip)
    {
        constexpr int samplesPerSplineSpan = 16;

        WaypointPath path;
        path.points = clip.waypoints;
        path.isSpline = (clip.movementType == MovementType::Spline);
        path.tension = clip.tension;

        const int n = path.points.size();
        if (n == 0) return path;

        int first = 0;
        for (int i = 1; i <= n; ++i)
        {
            const bool breakHere = (i == n) || path.points.getReference(i).startsNewSegment;
            if (!breakHere) continue;

            WaypointPath::SubPath sub;
            sub.firstIndex = first;
            sub.lastIndex = i - 1;
            sub.offset = path.totalLength;

            // Seed with the sub-path's own first point at zero length, so even a single-point
            // sub-path is still evaluable (it simply gets allotted no time).
            WaypointPath::Sample seed;
            seed.spanIndex = first;
            sub.samples.add(seed);

            for (int span = first; span < i - 1; ++span)
            {
                const int steps = path.isSpline ? samplesPerSplineSpan : 1;
                auto previous = evaluateWaypointSpan(path, span, 0.0, sub.firstIndex, sub.lastIndex);

                for (int s = 1; s <= steps; ++s)
                {
                    const double u = (double)s / (double)steps;
                    const auto current = evaluateWaypointSpan(path, span, u, sub.firstIndex, sub.lastIndex);
                    const double stepLength = (current - previous).length();
                    previous = current;

                    // A zero-length step would add a duplicate cumulative value, which the lookup
                    // below would have to special-case; skipping it keeps the table strictly
                    // increasing.
                    if (stepLength <= 1e-12) continue;

                    sub.length += stepLength;

                    WaypointPath::Sample sample;
                    sample.spanIndex = span;
                    sample.u = u;
                    sample.cumulative = sub.length;
                    sub.samples.add(sample);
                }
            }

            path.totalLength += sub.length;
            path.subPaths.add(sub);
            first = i;
        }

        return path;
    }

    // start is only consulted for the degenerate "no usable waypoints" case, where the group holds
    // its starting position. Returning the origin there instead would teleport the group across the
    // room for the clip's whole duration - and an empty list is exactly what a clip has the moment
    // its type is switched to Spline/Polygon.
    inline juce::Vector3D<double> evaluateWaypointPath(const WaypointPath& path, double progress,
                                                        const MovementStartState& start)
    {
        if (path.points.isEmpty()) return start.initialPosition;

        if (path.subPaths.isEmpty() || path.totalLength <= 1e-12)
            return waypointAt(path.points, 0);

        const double s = juce::jlimit(0.0, path.totalLength, progress * path.totalLength);

        // Pick the sub-path holding s. Zero-length sub-paths are skipped outright (they're allotted
        // no time); at an exact boundary the earlier sub-path's end wins, and at s == totalLength
        // the fallback keeps the last one.
        int chosen = path.subPaths.size() - 1;
        for (int i = 0; i < path.subPaths.size(); ++i)
        {
            const auto& candidate = path.subPaths.getReference(i);
            if (candidate.length <= 0.0) continue;
            if (s <= candidate.offset + candidate.length) { chosen = i; break; }
        }

        const auto& sub = path.subPaths.getReference(chosen);
        if (sub.samples.size() < 2)
            return waypointAt(path.points, sub.firstIndex);

        const double sLocal = juce::jlimit(0.0, sub.length, s - sub.offset);

        int lo = 0;
        int hi = sub.samples.size() - 1;
        while (hi - lo > 1)
        {
            const int mid = (lo + hi) / 2;
            if (sub.samples.getReference(mid).cumulative <= sLocal) lo = mid;
            else hi = mid;
        }

        const auto& a = sub.samples.getReference(lo);
        const auto& b = sub.samples.getReference(hi);
        const double den = b.cumulative - a.cumulative;
        const double t = (den > 1e-12) ? (sLocal - a.cumulative) / den : 0.0;

        if (a.spanIndex == b.spanIndex)
            return evaluateWaypointSpan(path, a.spanIndex, a.u + (b.u - a.u) * t, sub.firstIndex, sub.lastIndex);

        // a is the end of its span (u == 1), which is geometrically the start of b's span - so
        // continue into b's span from u = 0 rather than interpolating across the discontinuity in u.
        return evaluateWaypointSpan(path, b.spanIndex, b.u * t, sub.firstIndex, sub.lastIndex);
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

            case MovementType::Spline:
            case MovementType::Polygon:
                return evaluateWaypointPath(buildWaypointPath(clip), progress, start);

            case MovementType::Lissajous:
            {
                auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());

                // The start point sets both amplitudes, signed (not absolute) so a start point left
                // of / below the centre mirrors the figure rather than collapsing into a dead zone.
                //
                // Lissajous is the ONE movement type whose position at progress 0 is not the start
                // point - it begins at centre + (ampX*sin(phase), 0, 0). That's inherent to the
                // parameterisation, hence the dialog labels its start controls "Amplitude X/Y"
                // rather than "Start".
                const double theta = 2.0 * juce::MathConstants<double>::pi * clip.count * progress;
                const double phase = juce::degreesToRadians(clip.phaseDeg);

                return juce::Vector3D<double>(
                    centerPos.x + start.startOffset.x * std::sin(clip.freqRatioA * theta + phase),
                    centerPos.y + start.startOffset.y * std::sin(clip.freqRatioB * theta),
                    centerPos.z
                );
            }

            case MovementType::Rose:
            {
                auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());

                // Same direction convention as Spiral (NOT Circle's subtract-form, which looks
                // equivalent but doesn't give reverse-on-negative-count for free).
                const double direction = (clip.count >= 0) ? -1.0 : 1.0;
                const double absoluteCount = std::abs(clip.count);
                const double sweep = 2.0 * juce::MathConstants<double>::pi * absoluteCount * progress * direction;

                // freqRatioA is the petal count k here - count keeps its "revolutions" meaning, as
                // everywhere else. Anchoring the cosine to the swept angle (so r == startRadius at
                // progress 0) makes the curve begin exactly on the start point in XY.
                const double phase = juce::degreesToRadians(clip.phaseDeg);
                const double radius = start.startRadius * std::cos(clip.freqRatioA * sweep + phase);
                const double angle = start.startAngle + sweep;

                // Flattened to the centre's height, matching Circle and Spiral - Rose is a circle
                // variant, and Helix is the type that deliberately travels vertically.
                return juce::Vector3D<double>(
                    centerPos.x + radius * std::cos(angle),
                    centerPos.y + radius * std::sin(angle),
                    centerPos.z
                );
            }

            case MovementType::Helix:
            {
                auto centerPos = juce::Vector3D<double>(clip.targetPointGroup.getX(), clip.targetPointGroup.getY(), clip.targetPointGroup.getZ());

                const double direction = (clip.count >= 0) ? -1.0 : 1.0;
                const double absoluteCount = std::abs(clip.count);
                const double angle = start.startAngle + (2.0 * juce::MathConstants<double>::pi * absoluteCount * progress * direction);

                // Spiral's radius behaviour (so radiusChange gives a conical helix for free)...
                const double totalRadiusChange = clip.radiusChange * absoluteCount;
                double currentRadius = start.startRadius + (totalRadiusChange * progress);
                if (currentRadius < 0.0)
                    currentRadius = 0.0;

                // ...but unlike Spiral, the start point's height is preserved rather than flattened
                // to the centre, so the helix begins exactly on the start point and rises from there.
                return juce::Vector3D<double>(
                    centerPos.x + currentRadius * std::cos(angle),
                    centerPos.y + currentRadius * std::sin(angle),
                    centerPos.z + start.startOffset.z + clip.heightRise * progress
                );
            }

            case MovementType::RandomWalk:
            {
                // Smooth seeded wander, written as an OFFSET from the start point so progress 0
                // lands exactly there. Pure and deterministic (no accumulated state), so scrubbing
                // backwards reproduces the identical path - the same seek-safety rule the rest of
                // this file follows.
                const double t = clip.count * progress;

                // noise1D's actual range is about +/-0.5, so double it to make "amplitude = distance
                // from centre to start point" mean what it says.
                const double amplitude = 2.0 * start.startRadius;

                const auto seedX = walkSeed(clip.randomSeed, 0);
                const auto seedY = walkSeed(clip.randomSeed, 1);
                const auto seedZ = walkSeed(clip.randomSeed, 2);

                return start.initialPosition + juce::Vector3D<double>(
                    amplitude * (PerlinNoise::noise1D(seedX, t) - PerlinNoise::noise1D(seedX, 0.0)),
                    amplitude * (PerlinNoise::noise1D(seedY, t) - PerlinNoise::noise1D(seedY, 0.0)),
                    amplitude * (PerlinNoise::noise1D(seedZ, t) - PerlinNoise::noise1D(seedZ, 0.0))
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

    // True for the three rotation ActionTypes - they're handled as one combined orientation per clip
    // rather than independently, so several call sites need to recognise the set.
    inline bool isRotationAction(ActionType type)
    {
        return type == ActionType::RotationX || type == ActionType::RotationY || type == ActionType::RotationZ;
    }

    // Builds a rotation quaternion from per-axis angles, replicating AmbiGroup::rotate()'s group-mode
    // convention EXACTLY: X about (1,0,0), Y about (0,1,0), Z about (0,0,-1), right-multiplied in
    // X->Y->Z order (AmbiGroup.cpp:299-315).
    //
    // Two traps this deliberately avoids:
    //  - That NEGATIVE Z axis. MathHelper::EulerToQuaternion uses standard +Z and a different
    //    composition, so using it here would silently reverse every Z rotation. Don't substitute it.
    //  - juce::Quaternion<double>() default-constructs to (0,0,0,0) - the ZERO quaternion, not
    //    identity (juce_Quaternion.h:48), and normalised() on it divides by zero. Identity must be
    //    spelled out.
    //
    // The exactlyEqual guards mirror AmbiGroup::rotate()'s own, keeping the no-rotation path exact.
    inline juce::Quaternion<double> buildRotationQuaternion(double xRad, double yRad, double zRad)
    {
        juce::Quaternion<double> q(juce::Vector3D<double>(0.0, 0.0, 0.0), 1.0); // identity

        if (!juce::exactlyEqual(xRad, 0.0))
            q *= juce::Quaternion<double>::fromAngle(xRad, juce::Vector3D<double>(1.0, 0.0, 0.0));

        if (!juce::exactlyEqual(yRad, 0.0))
            q *= juce::Quaternion<double>::fromAngle(yRad, juce::Vector3D<double>(0.0, 1.0, 0.0));

        if (!juce::exactlyEqual(zRad, 0.0))
            q *= juce::Quaternion<double>::fromAngle(zRad, juce::Vector3D<double>(0.0, 0.0, -1.0));

        return q.normalised();
    }

    // One rotation ActionDefinition's absolute angle, in DEGREES, at the given phase (from
    // rotationPhase()). Degrees and unwrapped on purpose: a quaternion cannot represent 720 degrees,
    // so the angle has to stay scalar right up to buildRotationQuaternion() for multi-turn rotations
    // to actually turn more than once.
    //
    // Timing semantics mirror calculateStretch exactly - AbsoluteTarget lands ON value,
    // RelativeDuringClip lands value PAST the start - and ConstantPerSecond derives its elapsed time
    // from phase rather than wall clock, which is the single trick that makes all of this seek-safe.
    inline double calculateRotationDegrees(const ActionDefinition& actionDef, double phase, ms_t clipLengthMs)
    {
        const double startDeg = actionDef.getUseStartValue() ? actionDef.getStartValue() : 0.0;

        switch (actionDef.getTiming())
        {
            case TimingType::AbsoluteTarget:
                return startDeg + (actionDef.getValue() - startDeg) * phase;

            case TimingType::RelativeDuringClip:
                return startDeg + actionDef.getValue() * phase;

            case TimingType::ConstantPerSecond:
                return startDeg + actionDef.getValue() * (clipLengthMs / 1000.0) * phase;

            case TimingType::None:
            default:
                return startDeg;
        }
    }

    struct RotationState
    {
        juce::Quaternion<double> orientation { juce::Vector3D<double>(0.0, 0.0, 0.0), 1.0 };
        bool hasRotation = false; // false => leave the group's orientation completely untouched
    };

    // One action clip's complete group orientation at the given phase. Replaces the old
    // accumulate-a-delta-per-tick scheme, which is what made rotation unable to honour a start value
    // and unable to survive a seek, a mute or a clip-end overshoot.
    //
    // Angles from several actions targeting the same axis add, as they did when deltas accumulated.
    //
    // The absolute/relative choice is made ONCE PER CLIP, and is deliberately all-or-nothing:
    //  - If ANY rotation action in the clip sets a start value, the whole clip is world-absolute -
    //    the orientation is built straight from the final angles, so at phase 0 the group SNAPS to
    //    the defined start rotation, discarding whatever orientation it had. That snap is the point:
    //    it's what makes a timeline reproducible run to run. Axes without their own start value
    //    contribute 0 degrees. A base that were partly explicit and partly inherited wouldn't be
    //    reproducible at all, which is why there's no per-axis version of this.
    //  - Otherwise the clip is relative: the sweep composes onto capturedStartQ, the orientation
    //    sampled once when the clip started. This is the pre-existing behaviour, and since the old
    //    editor force-cleared useStartValue for rotation, it's the branch every already-saved clip
    //    takes - their endpoints are unchanged.
    inline RotationState computeClipRotation(const ActionClip& clip, double phase,
                                             juce::Quaternion<double> capturedStartQ, bool hasCapturedStart)
    {
        RotationState result;

        double xDeg = 0.0, yDeg = 0.0, zDeg = 0.0;
        bool absolute = false;

        for (const auto& actionDef : clip.actions)
        {
            if (!isRotationAction(actionDef.getAction()) || actionDef.getTiming() == TimingType::None)
                continue;

            result.hasRotation = true;
            if (actionDef.getUseStartValue())
                absolute = true;

            const double angleDeg = calculateRotationDegrees(actionDef, phase, clip.length);

            switch (actionDef.getAction())
            {
                case ActionType::RotationX: xDeg += angleDeg; break;
                case ActionType::RotationY: yDeg += angleDeg; break;
                case ActionType::RotationZ: zDeg += angleDeg; break;
                case ActionType::Stretch:
                case ActionType::Jitter:
                case ActionType::None:
                default: break; // unreachable - isRotationAction() already filtered these out
            }
        }

        if (!result.hasRotation)
            return result;

        const auto sweep = buildRotationQuaternion(juce::degreesToRadians(xDeg),
                                                    juce::degreesToRadians(yDeg),
                                                    juce::degreesToRadians(zDeg));

        if (absolute || !hasCapturedStart)
        {
            result.orientation = sweep;
        }
        else
        {
            // juce::Quaternion only offers operator*= (no binary operator*), so compose via a local.
            auto composed = capturedStartQ;
            composed *= sweep;
            result.orientation = composed.normalised();
        }

        return result;
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
