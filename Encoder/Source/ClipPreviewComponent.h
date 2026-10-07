#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"
#include "AnimatorMath.h"
#include "../../Common/PerlinNoise.h"
#include "../../Common/ScalingInfo.h"
#include "../../Common/ZoomSettings.h"

// Small looping radar-style preview for the Movement/Action clip editors - two square panels
// stacked in portrait order (top-down XY, front XZ below), showing a synthetic "formation" (one
// anchor + a few member points, no AmbiPoint/AmbiSourceSet involved) animated by the same math
// AnimatorEngine uses for real playback (AnimatorMath.h), so the preview never silently drifts
// from what the clip actually does.
//
// Movement and Action clips are previewed independently of each other (a MovementClipEditor only
// ever calls setMovementClip(), an ActionClipEditor only ever calls setActionClip()): for a
// Movement preview the whole formation translates together (anchor moves, members stay in rigid
// formation around it) and the whole trace is drawn as a static path; for an Action preview the
// anchor stays at the live reference position and the members rotate/stretch/jitter around it - a
// single anchor point wouldn't show any of those effects at all.
//
// World-to-screen mapping uses the project's own ScalingInfo (see setScalingInfo()/
// recomputeSceneScale()) when it's finite, so the preview shows real-world scale matching the main
// Radar view - the same distance looks the same size in both. When the scene scale is "infinite"
// (unbounded, the common case), there's no fixed real scale to borrow, so the preview falls back to
// its own linear auto-fit sized to the clip's own content instead (never the nonlinear atan()
// curve ScalingInfo::compress() itself uses in that mode - that would visually distort motion,
// making a constant real speed look like it's decelerating on screen).
//
// Looping semantics intentionally differ by type, matching what repeating the real clip would
// actually look like: MoveTo/Circle/Spiral/Stretch/Rotation all have a well-defined 0..1 progress
// tied to clip length, so they wrap and restart each loop. Jitter is the only unbounded one (noise
// has no cycle) - the preview lets it run forever rather than forcing an artificial reset.
//
// Interactive: Movement's Start/Target/Center point(s) can be dragged directly here instead of only
// typed into sliders - see HandleRef and the mouse handlers below. This component
// doesn't own clip data, so a drag is reported via onPointDragged rather than committed here; the
// host editor (MovementClipEditor) writes it into its own controls/model, the same as any other
// edit, so Apply/Cancel/dirty-detection all keep working unchanged. Action clips have no draggable
// handles - Stretch/Rotation aren't positions, and a fixed-pixel-offset icon representing an
// abstract magnitude/angle stopped making sense once the preview's scale is tied to the real scene
// (the formation could be any size on screen depending on the project's zoom) - they're edited via
// their own fields/dialog only.
class ClipPreviewComponent : public juce::Component, private juce::Timer
{
public:
    ClipPreviewComponent()
    {
        recomputeSceneScale();
        startTimerHz(30);
    }

    ~ClipPreviewComponent() override
    {
        stopTimer();
    }

    // Called once by the host editor after construction (ScalingInfo doesn't change while a clip
    // dialog is open, so this isn't polled like the clip data below).
    void setScalingInfo(ScalingInfo* scaling)
    {
        pScalingInfo = scaling;
        recomputeSceneScale();
    }

    // The main radar's zoom - only consulted in real-world view, and only when the project has no
    // finite distance scaler to take a world size from.
    void setZoomSettings(ZoomSettings* zoom)
    {
        pZoomSettings = zoom;
        recomputeSceneScale();
    }

    // false (default): frame the clip's own content, so a small movement still fills the panel.
    // true: show the clip at the scene's real scale, so you can judge where it sits in the room
    // rather than only what shape it traces.
    void setRealWorldView(bool shouldUseRealWorld)
    {
        if (realWorldView == shouldUseRealWorld)
            return;

        realWorldView = shouldUseRealWorld;
        recomputeSceneScale();
        repaint();
    }

    bool isRealWorldView() const { return realWorldView; }

    // The world radius the panel's circle represents in real-world view: the distance scaler when the
    // project has one, otherwise whatever the main radar is currently zoomed to. Returns 0 when
    // neither is available, which is what makes the toggle report itself unavailable.
    double getRealWorldRadius() const
    {
        if (pScalingInfo != nullptr && !pScalingInfo->IsInfinite())
            return (double)pScalingInfo->CartesianMax();

        if (pZoomSettings != nullptr)
            return (double)pZoomSettings->getCurrentRadius();

        return 0.0;
    }

    // Called on an initial push and then polled periodically (clip editors have no per-control
    // change notification, see ClipPreviewComponent's host editors). The running animation
    // (movementProgress etc.) isn't reset, so parameter edits update live without the preview
    // jumping/restarting - but the shape (movementStart, the full trace, the camera framing) is
    // recomputed fresh every call, so edits are reflected immediately rather than at the next loop.
    //
    // Takes the clip by value, not const&: MovementClip's Point3D members have a hand-written
    // operator= that only accepts a non-const lvalue, so assigning directly from a const reference
    // (or from a temporary, as applyChanges() does) won't compile - going through this mutable
    // local parameter first sidesteps that pre-existing Point3D quirk.
    void setMovementClip(MovementClip clip)
    {
        movementClip = clip;
        hasMovementClip = true;
        movementStart = AnimatorMath::computeMovementStartState(movementClip, referencePosition);
        rebuildMovementTrace();
        recomputeSceneScale();
    }

    void setActionClip(const ActionClip& clip)
    {
        actionClip = clip;
        hasActionClip = true;
        recomputeSceneScale();
    }

    // For MovementClip's useStartPoint==false case, and as the Action preview's fixed anchor -
    // pass the group's live real position (same value the real engine would use right now).
    void setReferencePosition(juce::Vector3D<double> pos)
    {
        referencePosition = pos;
        if (hasMovementClip)
        {
            movementStart = AnimatorMath::computeMovementStartState(movementClip, referencePosition);
            rebuildMovementTrace();
        }
        recomputeSceneScale();
    }

    // For Stretch's "no start value defined" case - pass the group's live real stretch factor.
    void setReferenceStretch(double stretch, bool hasInitial)
    {
        referenceStretch = stretch;
        hasReferenceStretchFlag = hasInitial;

        // Sync stretchInitial immediately rather than waiting for the next loop-wrap in
        // timerCallback() to pick it up. Without this, the *first* loop always animated from the
        // 1.0 member-initializer default regardless of what the live group's real stretch was,
        // and only the *second* loop (the first wrap) would snap to the true value - looking like
        // an unexplained jump between loop 1 and loop 2 even though the live reference never
        // actually changed in between. Matches setReferencePosition()'s "edits reflected
        // immediately" contract just above.
        stretchInitial = hasReferenceStretchFlag ? referenceStretch : 1.0;
        recomputeSceneScale();
    }

    // Fired live on every drag tick (not buffered to mouse-up) for a Movement Start/Target/Center
    // point - newWorldPos is already a full XYZ (the axis the dragged panel doesn't control is
    // held at its previous value). The host editor writes this into its own coordinate sliders.
    std::function<void(bool isStartHandle, juce::Vector3D<double> newWorldPos)> onPointDragged;

    // Spline/Polygon waypoint editing. Dragging fires continuously like onPointDragged; adding is
    // bound to cmd/ctrl-click (never a bare click, which would make any stray click in the panel
    // silently append a point) and removing to a double-click on an existing handle.
    std::function<void(int index, juce::Vector3D<double> newWorldPos)> onWaypointDragged;
    std::function<void(juce::Vector3D<double> worldPos)> onWaypointAdded;
    std::function<void(int insertIndex, juce::Vector3D<double> worldPos)> onWaypointInserted;
    std::function<void(int index)> onWaypointRemoved;

private:
    static constexpr int numMembers = 3;
    static constexpr int traceSamples = 200;

    // Which handle (if any) is currently being dragged or hovered - see findHandleAt()/mouseDown().
    // A waypoint path has arbitrarily many handles, so a handle is identified by kind + index
    // rather than by a bare enum value.
    enum class HandleKind { None, MovementStart, MovementTarget, Waypoint };

    struct HandleRef
    {
        HandleKind kind = HandleKind::None;
        int index = -1; // waypoint index; unused for the other kinds

        bool operator==(const HandleRef& other) const { return kind == other.kind && index == other.index; }
        bool operator!=(const HandleRef& other) const { return !(*this == other); }
        bool isSet() const { return kind != HandleKind::None; }
    };

    HandleRef draggedHandle;
    HandleRef hoveredHandle;
    bool draggedPanelIsXY = true;

    // The offset between where the user actually clicked and the handle's exact screen position,
    // preserved for the whole drag so the handle doesn't jump to the cursor.
    juce::Point<float> dragGrabOffsetScreen;

    static constexpr float handleHitRadiusPx = 9.0f;

    // Deliberately wider than the handle radius: hitting a thin line needs more tolerance than
    // hitting a drawn dot, and a handle always wins the double-click anyway (removal is checked
    // first), so the two radii overlapping can't make a waypoint harder to delete.
    static constexpr float pathHitRadiusPx = 12.0f;

    bool isHandleActive(HandleRef h) const
    {
        return draggedHandle == h || hoveredHandle == h;
    }

    juce::Vector3D<double> waypointPosition(int index) const
    {
        if (index < 0 || index >= movementClip.waypoints.size()) return {};
        const auto& wp = movementClip.waypoints.getReference(index);
        return { wp.x, wp.y, wp.z };
    }

    // First ActionDefinition of this type with real timing, or nullptr. Note the real engine
    // (AnimatorEngine::processActiveActions) applies *every* matching entry (last one wins, via
    // repeated setGroupStretch calls) while this preview has always used first-match only - a
    // pre-existing inconsistency, out of scope here.
    const ActionDefinition* findAction(ActionType type) const
    {
        for (const auto& a : actionClip.actions)
            if (a.getAction() == type && a.getTiming() != TimingType::None)
                return &a;
        return nullptr;
    }

    void timerCallback() override
    {
        constexpr double tickSeconds = 1.0 / 30.0;

        if (hasMovementClip)
        {
            const ms_t length = juce::jmax<ms_t>(1, movementClip.length);
            movementProgress += tickSeconds * 1000.0 / (double)length;
            if (movementProgress >= 1.0)
                movementProgress -= std::floor(movementProgress);
        }

        if (hasActionClip)
        {
            const ms_t length = juce::jmax<ms_t>(1, actionClip.length);
            stretchProgress += tickSeconds * 1000.0 / (double)length;
            if (stretchProgress >= 1.0)
            {
                stretchProgress -= std::floor(stretchProgress);
                stretchInitial = hasReferenceStretchFlag ? referenceStretch : 1.0;

                // stretchInitial just changed - it's tracking the group's live real stretch,
                // which can be far from 1.0 (Stretch, unlike Jitter, deliberately leaves its
                // effect in place after a clip ends, so an earlier clip in the timeline may have
                // left the group heavily grown/shrunk already). recomputeSceneScale()'s action-clip
                // fallback extent is derived from stretchInitial (see below), so the camera must be
                // refreshed here too, or a loop that restarts from a very different stretch than
                // the one the camera was framed for will send the formation flying off-panel.
                // This is a deliberate, rare exception to "never recompute mid-animation" above -
                // it fires once per loop restart, not every tick, so it doesn't reintroduce breathing.
                recomputeSceneScale();
            }

            // Rotation needs no per-tick bookkeeping here any more: like Stretch, it's an absolute
            // function of stretchProgress, evaluated in computeCurrentFormation() below. It used to
            // accumulate into a member that was never reset between loops, so a previewed rotation
            // drifted further on every pass.
            jitterElapsedSeconds += tickSeconds;
        }

        computeCurrentFormation();
        repaint();
    }

    // Rotates an offset by a group orientation, indexing the matrix exactly as
    // AmbiGroup::getAbsSourcePoint() does (AmbiGroup.cpp:473-475), so the preview and real playback
    // agree on direction and axis rather than approximating each other.
    static juce::Vector3D<double> rotateVector(juce::Vector3D<double> v, const juce::Quaternion<double>& orientation)
    {
        const auto m = orientation.getRotationMatrix();

        return juce::Vector3D<double>(
            m.mat[0] * v.x + m.mat[1] * v.y + m.mat[2]  * v.z,
            m.mat[4] * v.x + m.mat[5] * v.y + m.mat[6]  * v.z,
            m.mat[8] * v.x + m.mat[9] * v.y + m.mat[10] * v.z);
    }

    static juce::Vector3D<double> baseOffsetDirection(int index)
    {
        const double angle = index * (2.0 * juce::MathConstants<double>::pi / (double)numMembers);
        return juce::Vector3D<double>(std::cos(angle), std::sin(angle), 0.0);
    }

    // An internal display constant, only actually used as a world-space size when falling back to
    // auto-fit (see recomputeSceneScale()) - under the real ScalingInfo scale it's just "however
    // many real-world units the synthetic formation's members sit from the anchor."
    static double offsetMagnitude()
    {
        return 1.0;
    }

    // Deliberately NOT eased, unlike the moving dot in computeCurrentFormation(). This samples the
    // GEOMETRIC path, and easing only redistributes sample density along an identical shape - so
    // applying it here would change nothing except to leave the drawn polyline faceted wherever the
    // curve runs fast. An unchanging path under a dot whose speed visibly varies is also what makes
    // the speed curve readable at a glance.
    void rebuildMovementTrace()
    {
        fullTrace.clearQuick();
        if (!hasMovementClip) return;

        fullTrace.ensureStorageAllocated(traceSamples + 1);

        // Waypoint paths get their arc-length table built ONCE here rather than per sample.
        // calculatePosition() would otherwise rebuild it on all 201 calls, and this runs on every
        // 150ms poll AND on every drag tick - the one place where that cost actually matters.
        if (movementTypeUsesWaypoints(movementClip.movementType))
        {
            const auto path = AnimatorMath::buildWaypointPath(movementClip);

            for (int i = 0; i <= traceSamples; ++i)
            {
                const double p = (double)i / (double)traceSamples;
                const auto cycle = AnimatorMath::computeCycleState(p, movementClip.repetitions, movementClip.palindrome);
                fullTrace.add(AnimatorMath::evaluateWaypointPath(path, cycle.cycleProgress, movementStart));
            }
            return;
        }

        for (int i = 0; i <= traceSamples; ++i)
        {
            const double p = (double)i / (double)traceSamples;
            fullTrace.add(AnimatorMath::calculatePosition(movementClip, p, movementStart, referencePosition));
        }
    }

    // World origin (0,0,0) is always the panel centre, for both clip types - matching the main
    // Radar view's fixed reference frame, and letting the anchor visibly pan across the panel as
    // the user drags the live group around in the main view.
    //
    // The actual scale (world units per panel radius) prefers the project's own ScalingInfo - see
    // the class comment - so the preview shows the same real-world size the main Radar would. That
    // falls back to a linear auto-fit sized to the clip's own content only when ScalingInfo is null
    // or "infinite" (unbounded scenes have no fixed real scale to borrow). The auto-fit itself must
    // stay independent of the live reference position for an Action clip: an earlier version
    // computed it from referencePosition's own distance from the origin, which made the exact same
    // formation look a different size purely depending on where in the scene the group happened to
    // be ("rotation near the centre looks much bigger than at the corners").
    // Where an Action preview's formation sits.
    //
    // Real-world view uses the group's actual position, which is the entire point of that view.
    // Auto-fit deliberately does NOT: its scale is derived from the formation's own extent (a stretch
    // factor, a jitter amplitude - see recomputeSceneScale), with no idea where the group happens to
    // be. Anchoring at the live position then pushed the formation straight off the panel as soon as
    // the group was moved away from the origin in the main radar - the action appeared to vanish.
    // An action is a transformation ABOUT the group, so framing it centred is also what you want to
    // look at; where the group sits is what real-world view is for.
    juce::Vector3D<double> actionAnchor() const
    {
        return realWorldView ? referencePosition : juce::Vector3D<double>();
    }

    void recomputeSceneScale()
    {
        // Frozen during any drag - the drag write-back calls setMovementClip() on every tick for
        // live feedback, which would otherwise re-fit the camera mid-drag and break the 1:1
        // screen<->world mapping screenToWorld()/the drag formulas assume. mouseUp() calls this
        // once more explicitly to re-fit after the drag ends.
        if (draggedHandle.isSet()) return;

        // Real-world view: the panel's circle IS the scene boundary, so a world coordinate at that
        // radius lands exactly on it (1.0, not the 0.85 headroom the auto-fit below uses - there the
        // circle is only decoration, here it means something).
        //
        // This applies to movement and action previews alike. The action preview used to have no
        // real-world option at all and always framed its own synthetic formation, which is why it
        // looked like a fixed little world of its own.
        if (realWorldView)
        {
            const double radius = getRealWorldRadius();

            if (radius > 0.0)
            {
                sceneScale = 1.0 / juce::jmax(0.05, radius);
                return;
            }
            // No scaler and no zoom to borrow - fall through to the auto-fit rather than show nothing.
        }

        if (hasMovementClip)
        {
            double maxDistanceFromOrigin = 0.0;
            for (const auto& p : fullTrace)
            {
                maxDistanceFromOrigin = juce::jmax(maxDistanceFromOrigin, std::abs(p.x));
                maxDistanceFromOrigin = juce::jmax(maxDistanceFromOrigin, std::abs(p.y));
                maxDistanceFromOrigin = juce::jmax(maxDistanceFromOrigin, std::abs(p.z));
            }
            // A little headroom (85% fill) so content doesn't touch the panel edge; floored so a
            // clip that's entirely at the origin (e.g. a fresh, untouched clip) doesn't divide by zero.
            sceneScale = 0.85 / juce::jmax(0.05, maxDistanceFromOrigin);
        }
        else if (hasActionClip)
        {
            // Not sampled from live (potentially jittering/rotating) positions - estimated from
            // the clip's own parameters, generously padded, so the camera doesn't need to move
            // every frame to keep up with the content it's framing.
            double maxJitterIntensity = 0.0;
            for (const auto& actionDef : actionClip.actions)
                if (actionDef.getAction() == ActionType::Jitter)
                    maxJitterIntensity = juce::jmax(maxJitterIntensity, actionDef.getValue());

            // Floored at 1.0 so the unstretched formation (progress 0 of a loop that hasn't
            // reached its first wrap yet) is always framed, even if a live reference stretch
            // would otherwise shrink the estimate below that.
            double maxAbsStretch = 1.0;
            if (auto* stretchAction = findAction(ActionType::Stretch))
            {
                // calculateStretch() is linear in progress for every TimingType, so its two
                // endpoints bound the whole loop - evaluated against stretchInitial, the same
                // initial state computeCurrentFormation() will actually animate from.
                const double s0 = AnimatorMath::calculateStretch(*stretchAction, 0.0, actionClip.length, stretchInitial, hasReferenceStretchFlag);
                const double s1 = AnimatorMath::calculateStretch(*stretchAction, 1.0, actionClip.length, stretchInitial, hasReferenceStretchFlag);
                maxAbsStretch = juce::jmax(maxAbsStretch, juce::jmax(std::abs(s0), std::abs(s1)));
            }

            const double extent = offsetMagnitude() * maxAbsStretch * 2.2 + maxJitterIntensity;
            sceneScale = 0.85 / juce::jmax(0.05, extent);
        }
    }

    void computeCurrentFormation()
    {
        // The MOVING DOT is eased, exactly as the engine eases it. The drawn trace deliberately is
        // not (see rebuildMovementTrace) - the contrast between an unchanged path and a changing
        // dot is what makes the speed curve legible here.
        const double easedMovement = hasMovementClip
            ? AnimatorMath::applyEasing(movementClip, movementProgress)
            : movementProgress;

        currentAnchor = hasMovementClip
            ? AnimatorMath::calculatePosition(movementClip, easedMovement, movementStart, referencePosition)
            : actionAnchor();

        double stretch = 1.0;
        // Identity, spelled out - juce::Quaternion<double>() is the ZERO quaternion.
        AnimatorMath::RotationState rotation;

        if (hasActionClip)
        {
            // Warped once for the whole clip, mirroring AnimatorEngine::processActiveActions.
            const double easedAction = AnimatorMath::applyEasing(actionClip, stretchProgress);

            if (auto* stretchAction = findAction(ActionType::Stretch))
            {
                const auto actionCycle = AnimatorMath::computeCycleState(easedAction, actionClip.repetitions, actionClip.palindrome);
                stretch = AnimatorMath::calculateStretch(*stretchAction, actionCycle.cycleProgress, actionClip.length, stretchInitial, hasReferenceStretchFlag);
            }

            // The preview has no live group orientation to inherit, so it passes "no captured start"
            // and relative sweeps start from identity. A clip with a defined start angle is
            // world-absolute anyway, so it previews exactly as it will play.
            rotation = AnimatorMath::computeClipRotation(actionClip,
                                                         AnimatorMath::rotationPhase(easedAction, actionClip.repetitions, actionClip.palindrome),
                                                         juce::Quaternion<double>(juce::Vector3D<double>(0.0, 0.0, 0.0), 1.0),
                                                         false);
        }

        const double magnitude = offsetMagnitude();

        for (int i = 0; i < numMembers; ++i)
        {
            auto offset = baseOffsetDirection(i) * magnitude;

            if (hasActionClip)
            {
                offset = offset * stretch;
                if (rotation.hasRotation)
                    offset = rotateVector(offset, rotation.orientation);

                for (const auto& actionDef : actionClip.actions)
                {
                    if (actionDef.getAction() == ActionType::Jitter)
                    {
                        const double t = jitterElapsedSeconds * actionDef.getJitterSpeed();
                        const double nx = PerlinNoise::noise1D(AnimatorMath::jitterSeed(actionClip.id, i, 0), t);
                        const double ny = PerlinNoise::noise1D(AnimatorMath::jitterSeed(actionClip.id, i, 1), t);
                        const double nz = PerlinNoise::noise1D(AnimatorMath::jitterSeed(actionClip.id, i, 2), t);
                        offset += juce::Vector3D<double>(nx, ny, nz) * actionDef.getValue();
                    }
                }
            }

            currentMembers[i] = currentAnchor + offset;
        }
    }

    // --- Panel geometry / projection - shared by painting, hit-testing and drag math ---

    // Mirrors paint()'s own bounds split exactly - the single source of truth for it now.
    juce::Rectangle<int> getPanelBounds(bool isXY) const
    {
        auto area = getLocalBounds().reduced(4);
        auto xyArea = area.removeFromTop(area.getHeight() / 2).reduced(2);
        auto xzArea = area.reduced(2);
        return isXY ? xyArea : xzArea;
    }

    struct PanelGeometry
    {
        juce::Rectangle<int> square;
        juce::Point<float> center;
        float radius;
    };

    PanelGeometry getPanelGeometry(bool isXY) const
    {
        auto bounds = getPanelBounds(isXY);
        const int side = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto square = juce::Rectangle<int>(0, 0, side, side).withCentre(bounds.getCentre());
        return { square, square.getCentre().toFloat(), (float)side * 0.48f };
    }

    juce::Point<float> worldToScreen(juce::Vector3D<double> worldPos, const PanelGeometry& geom, bool isXY) const
    {
        const auto rel = worldPos * sceneScale;
        const double a = rel.x;
        const double b = isXY ? rel.y : rel.z;
        return { geom.center.x + (float)a * geom.radius, geom.center.y - (float)b * geom.radius };
    }

    // Exact inverse of worldToScreen() - the axis the panel doesn't control is held at whatever
    // heldAxisSource already has (e.g. the point's own current Z, when dragging in the XY panel).
    juce::Vector3D<double> screenToWorld(juce::Point<float> screenPos, const PanelGeometry& geom, bool isXY,
                                          juce::Vector3D<double> heldAxisSource) const
    {
        const double a = (screenPos.x - geom.center.x) / (double)geom.radius;
        const double b = (geom.center.y - screenPos.y) / (double)geom.radius;
        juce::Vector3D<double> result = heldAxisSource;
        result.x = a / sceneScale;
        if (isXY) result.y = b / sceneScale;
        else      result.z = b / sceneScale;
        return result;
    }

    // --- Hit-testing ---

    struct HandleHit
    {
        HandleRef handle;
        bool isXYPanel = true;
        juce::Point<float> screenPos;
    };

    HandleHit findHandleAt(juce::Point<float> screenPos) const
    {
        HandleHit best;
        float bestDist = handleHitRadiusPx;

        auto consider = [&](HandleRef handle, bool isXY, juce::Point<float> candidateScreenPos)
        {
            const float d = screenPos.getDistanceFrom(candidateScreenPos);
            if (d <= bestDist) { bestDist = d; best = { handle, isXY, candidateScreenPos }; }
        };

        if (!hasMovementClip)
            return best;

        const bool waypointType = movementTypeUsesWaypoints(movementClip.movementType);

        for (bool isXY : { true, false })
        {
            if (!getPanelBounds(isXY).contains(screenPos.roundToInt()))
                continue;

            const auto geom = getPanelGeometry(isXY);

            if (waypointType)
            {
                // The path's own points are the only positional handles for these types - the
                // target/centre point isn't used at all.
                for (int i = 0; i < movementClip.waypoints.size(); ++i)
                    consider({ HandleKind::Waypoint, i }, isXY, worldToScreen(waypointPosition(i), geom, isXY));
            }
            else
            {
                juce::Vector3D<double> targetWorld(movementClip.targetPointGroup.getX(), movementClip.targetPointGroup.getY(), movementClip.targetPointGroup.getZ());
                consider({ HandleKind::MovementTarget, -1 }, isXY, worldToScreen(targetWorld, geom, isXY));
            }

            if (movementClip.useStartPoint)
            {
                juce::Vector3D<double> startWorld(movementClip.startPointGroup.getX(), movementClip.startPointGroup.getY(), movementClip.startPointGroup.getZ());
                consider({ HandleKind::MovementStart, -1 }, isXY, worldToScreen(startWorld, geom, isXY));
            }
        }

        return best;
    }

    struct PathInsertion
    {
        bool valid = false;
        int insertIndex = 0;             // index the new waypoint takes, i.e. the end of the span hit
        juce::Vector3D<double> worldPos; // snapped onto the curve, not the raw click position
    };

    // Nearest point on the drawn path to a click, for double-click-to-insert. Walks the actual
    // spans rather than the chords between waypoints, so it follows a Spline's bulge as well as a
    // Polygon's straight edges (for a Polygon the two are the same thing).
    //
    // The returned position is the point ON the curve, not where the user clicked: inserting there
    // leaves a Polygon's shape completely unchanged, so the gesture reads as "give me a handle here"
    // rather than "drag the path to my cursor". The user drags the new handle afterwards.
    PathInsertion findPathInsertionAt(juce::Point<float> screenPos) const
    {
        PathInsertion best;

        if (!hasMovementClip || !movementTypeUsesWaypoints(movementClip.movementType))
            return best;
        if (movementClip.waypoints.size() < 2)
            return best;

        const auto path = AnimatorMath::buildWaypointPath(movementClip);
        float bestDist = pathHitRadiusPx;

        for (bool isXY : { true, false })
        {
            if (!getPanelBounds(isXY).contains(screenPos.roundToInt()))
                continue;

            const auto geom = getPanelGeometry(isXY);

            for (const auto& sub : path.subPaths)
            {
                // Spans run firstIndex..lastIndex-1, plus the closing span back to firstIndex when
                // the path is closed. The gap BETWEEN two sub-paths is deliberately not a span, so
                // double-clicking along an instant jump inserts nothing - there's no path there to
                // add a point to.
                const int lastSpan = (path.closed && sub.lastIndex > sub.firstIndex)
                                   ? sub.lastIndex : sub.lastIndex - 1;

                for (int span = sub.firstIndex; span <= lastSpan; ++span)
                {
                    constexpr int steps = 24; // per span - dense enough that the hit radius never slips through

                    for (int s = 0; s <= steps; ++s)
                    {
                        const auto world = AnimatorMath::evaluateWaypointSpan(path, span, (double)s / (double)steps,
                                                                              sub.firstIndex, sub.lastIndex);
                        const float d = screenPos.getDistanceFrom(worldToScreen(world, geom, isXY));

                        if (d <= bestDist)
                        {
                            bestDist = d;
                            best.valid = true;
                            best.insertIndex = span + 1;
                            best.worldPos = world;
                        }
                    }
                }
            }
        }

        return best;
    }

    // --- Mouse handling ---

    void mouseMove(const juce::MouseEvent& e) override
    {
        const auto hit = findHandleAt(e.position);
        setMouseCursor(hit.handle.isSet() ? juce::MouseCursor::DraggingHandCursor
                                          : juce::MouseCursor::NormalCursor);
        if (hit.handle != hoveredHandle)
        {
            hoveredHandle = hit.handle;
            repaint();
        }
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        if (hoveredHandle.isSet())
        {
            hoveredHandle = {};
            repaint();
        }
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        const auto hit = findHandleAt(e.position);
        draggedHandle = hit.handle;

        if (draggedHandle.isSet())
        {
            draggedPanelIsXY = hit.isXYPanel;
            dragGrabOffsetScreen = hit.screenPos - e.position;
            return;
        }

        // Empty space: append a waypoint, but only on an explicit cmd/ctrl-click. A bare click must
        // stay inert - it's also how the user focuses the dialog or dismisses a selection.
        if (!hasMovementClip || !movementTypeUsesWaypoints(movementClip.movementType)) return;
        if (!(e.mods.isCommandDown() || e.mods.isCtrlDown())) return;
        if (onWaypointAdded == nullptr) return;

        for (bool isXY : { true, false })
        {
            if (!getPanelBounds(isXY).contains(e.position.roundToInt()))
                continue;

            // The axis this panel doesn't control is inherited from the last waypoint, so a point
            // added in the XY panel lands at the path's current height rather than at z = 0.
            const auto held = movementClip.waypoints.isEmpty()
                ? juce::Vector3D<double>()
                : waypointPosition(movementClip.waypoints.size() - 1);

            onWaypointAdded(screenToWorld(e.position, getPanelGeometry(isXY), isXY, held));
            return;
        }
    }

    void mouseDoubleClick(const juce::MouseEvent& e) override
    {
        // On a handle: remove it. Removal lives here rather than on a modifier-click because there's
        // no click-vs-drag discrimination in mouseDown - a modifier-click would both move the handle
        // and delete it.
        const auto hit = findHandleAt(e.position);

        if (hit.handle.isSet())
        {
            if (hit.handle.kind != HandleKind::Waypoint || onWaypointRemoved == nullptr)
                return; // the Start/Target handles have no double-click meaning

            const int index = hit.handle.index;

            // Both references point at an index that's about to shift - clear them before the list
            // changes, or a later drag would address the wrong (or a past-the-end) waypoint.
            draggedHandle = {};
            hoveredHandle = {};

            onWaypointRemoved(index);
            return;
        }

        // Not on a handle, but on the path itself: insert a waypoint there, BETWEEN the two it falls
        // between rather than appended, so the path keeps its shape and the new point is immediately
        // draggable. This is the discoverable counterpart to cmd/ctrl-click, which appends to the end.
        if (onWaypointInserted == nullptr)
            return;

        const auto insertion = findPathInsertionAt(e.position);
        if (!insertion.valid)
            return;

        // Same index-shift hazard as removal: everything from insertIndex on moves up by one.
        draggedHandle = {};
        hoveredHandle = {};

        onWaypointInserted(insertion.insertIndex, insertion.worldPos);
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (!draggedHandle.isSet()) return;
        const auto geom = getPanelGeometry(draggedPanelIsXY);

        if (draggedHandle.kind == HandleKind::Waypoint)
        {
            if (draggedHandle.index < 0 || draggedHandle.index >= movementClip.waypoints.size())
                return; // the list changed under us (e.g. a removal) - drop the stale drag

            const auto newWorld = screenToWorld(e.position + dragGrabOffsetScreen, geom, draggedPanelIsXY,
                                                 waypointPosition(draggedHandle.index));
            if (onWaypointDragged)
                onWaypointDragged(draggedHandle.index, newWorld);
            return;
        }

        const bool isStart = (draggedHandle.kind == HandleKind::MovementStart);
        juce::Vector3D<double> currentWorld = isStart
            ? juce::Vector3D<double>(movementClip.startPointGroup.getX(), movementClip.startPointGroup.getY(), movementClip.startPointGroup.getZ())
            : juce::Vector3D<double>(movementClip.targetPointGroup.getX(), movementClip.targetPointGroup.getY(), movementClip.targetPointGroup.getZ());
        const auto newWorld = screenToWorld(e.position + dragGrabOffsetScreen, geom, draggedPanelIsXY, currentWorld);
        if (onPointDragged)
            onPointDragged(isStart, newWorld);
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        draggedHandle = {};
        recomputeSceneScale(); // the last drag tick's setMovementClip() skipped this - refit now
        repaint();
    }

    // --- Painting ---

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff1a1f24));

        // Portrait order: top-down XY above, front XZ below.
        drawPanel(g, true);
        drawPanel(g, false);
    }

    void drawPanel(juce::Graphics& g, bool isXY)
    {
        const auto geom = getPanelGeometry(isXY);

        g.setColour(juce::Colour(0xff0d1013));
        g.fillRect(geom.square);
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.drawRect(geom.square, 1);

        g.setColour(juce::Colours::white.withAlpha(0.12f));
        g.drawLine(geom.center.x - geom.radius, geom.center.y, geom.center.x + geom.radius, geom.center.y, 1.0f);
        g.drawLine(geom.center.x, geom.center.y - geom.radius, geom.center.x, geom.center.y + geom.radius, 1.0f);
        g.drawEllipse(geom.center.x - geom.radius, geom.center.y - geom.radius, geom.radius * 2.0f, geom.radius * 2.0f, 1.0f);

        g.setFont(juce::FontOptions(10.0f));
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.drawText(isXY ? "XY" : "XZ", geom.square.withHeight(14).reduced(3, 0), juce::Justification::topLeft);

        auto toScreen = [&](juce::Vector3D<double> worldPos) { return worldToScreen(worldPos, geom, isXY); };

        // Whole trace, drawn as a static path (not a fading recent-history trail) so the complete
        // shape of the movement is visible at a glance, with the bullet riding along it below.
        if (hasMovementClip && fullTrace.size() > 1)
        {
            juce::Path path;
            for (int i = 0; i < fullTrace.size(); ++i)
            {
                auto p = toScreen(fullTrace.getReference(i));
                if (i == 0) path.startNewSubPath(p);
                else path.lineTo(p);
            }
            g.setColour(juce::Colours::cornflowerblue.withAlpha(0.5f));
            g.strokePath(path, juce::PathStrokeType(1.5f));
        }

        const auto anchorScreen = toScreen(currentAnchor);
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.fillEllipse(anchorScreen.x - 2.5f, anchorScreen.y - 2.5f, 5.0f, 5.0f);

        g.setColour(juce::Colours::cornflowerblue);
        for (const auto& member : currentMembers)
        {
            const auto p = toScreen(member);
            g.fillEllipse(p.x - 3.5f, p.y - 3.5f, 7.0f, 7.0f);
        }

        if (hasMovementClip)
        {
            if (movementTypeUsesWaypoints(movementClip.movementType))
            {
                for (int i = 0; i < movementClip.waypoints.size(); ++i)
                {
                    const bool breaksHere = i > 0 && movementClip.waypoints.getReference(i).startsNewSegment;
                    const auto screenPos = toScreen(waypointPosition(i));

                    // A point that begins a new sub-path is drawn in the "jump" colour and ringed,
                    // so the discontinuity is visible in the path itself and not only in the table.
                    drawPointHandle(g, screenPos,
                                     breaksHere ? juce::Colours::orangered : juce::Colours::yellow,
                                     isHandleActive({ HandleKind::Waypoint, i }));

                    if (breaksHere)
                    {
                        g.setColour(juce::Colours::orangered.withAlpha(0.8f));
                        g.drawEllipse(screenPos.x - 8.0f, screenPos.y - 8.0f, 16.0f, 16.0f, 1.2f);
                    }

                    g.setColour(juce::Colours::white.withAlpha(0.75f));
                    g.setFont(juce::FontOptions(9.0f));
                    g.drawText(juce::String(i + 1), juce::Rectangle<float>(screenPos.x + 5.0f, screenPos.y - 12.0f, 20.0f, 10.0f),
                               juce::Justification::topLeft);
                }
            }
            else
            {
                juce::Vector3D<double> targetWorld(movementClip.targetPointGroup.getX(), movementClip.targetPointGroup.getY(), movementClip.targetPointGroup.getZ());
                drawPointHandle(g, toScreen(targetWorld), juce::Colours::orange, isHandleActive({ HandleKind::MovementTarget, -1 }));
            }

            if (movementClip.useStartPoint)
            {
                juce::Vector3D<double> startWorld(movementClip.startPointGroup.getX(), movementClip.startPointGroup.getY(), movementClip.startPointGroup.getZ());
                drawPointHandle(g, toScreen(startWorld), juce::Colours::lightgreen, isHandleActive({ HandleKind::MovementStart, -1 }));
            }
        }
    }

    // Movement Start/Target/Center handles - a small diamond, brighter/larger when hovered or dragged.
    static void drawPointHandle(juce::Graphics& g, juce::Point<float> screenPos, juce::Colour colour, bool isActive)
    {
        const float r = isActive ? 6.0f : 4.5f;
        juce::Path diamond;
        diamond.addQuadrilateral(screenPos.x, screenPos.y - r, screenPos.x + r, screenPos.y,
                                  screenPos.x, screenPos.y + r, screenPos.x - r, screenPos.y);
        g.setColour(isActive ? colour : colour.withAlpha(0.75f));
        g.fillPath(diamond);
        g.setColour(juce::Colours::black.withAlpha(0.5f));
        g.strokePath(diamond, juce::PathStrokeType(1.0f));
    }

    ScalingInfo* pScalingInfo = nullptr;
    ZoomSettings* pZoomSettings = nullptr;
    bool realWorldView = false;

    MovementClip movementClip;
    bool hasMovementClip = false;
    double movementProgress = 0.0;
    AnimatorMath::MovementStartState movementStart;
    juce::Array<juce::Vector3D<double>> fullTrace;

    ActionClip actionClip;
    bool hasActionClip = false;
    double stretchProgress = 0.0;
    double stretchInitial = 1.0;
    double jitterElapsedSeconds = 0.0;

    juce::Vector3D<double> referencePosition;
    double referenceStretch = 1.0;
    bool hasReferenceStretchFlag = false;

    double sceneScale = 1.0;

    juce::Vector3D<double> currentAnchor;
    juce::Vector3D<double> currentMembers[numMembers];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipPreviewComponent)
};
