#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"
#include "AnimatorMath.h"
#include "../../Common/PerlinNoise.h"
#include "../../Common/ScalingInfo.h"

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
// actually look like: MoveTo/Circle/Spiral/Stretch have a well-defined 0..1 progress tied to clip
// length, so they wrap and restart each loop. Rotation and Jitter have no such bound in the real
// engine (rotation accumulates forever, jitter has no cycle) - the preview lets them run forever
// too, rather than forcing an artificial reset.
//
// Interactive: Movement's Start/Target/Center point(s) can be dragged directly here instead of only
// typed into sliders - see the DraggedHandle enum and the mouse handlers below. This component
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

private:
    static constexpr int numMembers = 3;
    static constexpr int traceSamples = 200;

    // Which handle (if any) is currently being dragged or hovered - see findHandleAt()/mouseDown().
    enum class DraggedHandle { None, MovementStart, MovementTarget };

    DraggedHandle draggedHandle = DraggedHandle::None;
    DraggedHandle hoveredHandle = DraggedHandle::None;
    bool draggedPanelIsXY = true;

    // The offset between where the user actually clicked and the handle's exact screen position,
    // preserved for the whole drag so the handle doesn't jump to the cursor.
    juce::Point<float> dragGrabOffsetScreen;

    static constexpr float handleHitRadiusPx = 9.0f;

    bool isHandleActive(DraggedHandle h) const
    {
        return draggedHandle == h || hoveredHandle == h;
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
        constexpr ms_t tickMs = 33;

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

            // Shared with computeCurrentFormation()'s Stretch handling below - both read from the
            // same wrapping stretchProgress clock, matching how a real ActionClip's Repetitions/
            // Palindrome apply uniformly to every action within it (AnimatorEngine::processActiveActions
            // computes exactly one cycleState per clip too, not one per action).
            const auto actionCycle = AnimatorMath::computeCycleState(stretchProgress, actionClip.repetitions, actionClip.palindrome);

            for (const auto& actionDef : actionClip.actions)
            {
                if (actionDef.getAction() == ActionType::RotationX ||
                    actionDef.getAction() == ActionType::RotationY ||
                    actionDef.getAction() == ActionType::RotationZ)
                {
                    accumulatedRotation += AnimatorMath::calculateRotationTickRadians(actionDef, tickMs, actionClip.length) * actionCycle.direction;
                }
            }

            jitterElapsedSeconds += tickSeconds;
        }

        computeCurrentFormation();
        repaint();
    }

    static juce::Vector3D<double> rotateVector(juce::Vector3D<double> v, juce::Vector3D<double> anglesRad)
    {
        // Sequential X -> Y -> Z rotation - a deliberate visual simplification of the real engine's
        // internal quaternion/point-rotation mode split (not exposed to clip data), good enough to
        // judge speed/direction/axis at a glance.
        {
            const double c = std::cos(anglesRad.x), s = std::sin(anglesRad.x);
            const double y = v.y * c - v.z * s;
            const double z = v.y * s + v.z * c;
            v.y = y; v.z = z;
        }
        {
            const double c = std::cos(anglesRad.y), s = std::sin(anglesRad.y);
            const double x = v.x * c + v.z * s;
            const double z = -v.x * s + v.z * c;
            v.x = x; v.z = z;
        }
        {
            const double c = std::cos(anglesRad.z), s = std::sin(anglesRad.z);
            const double x = v.x * c - v.y * s;
            const double y = v.x * s + v.y * c;
            v.x = x; v.y = y;
        }
        return v;
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

    void rebuildMovementTrace()
    {
        fullTrace.clearQuick();
        if (!hasMovementClip) return;

        fullTrace.ensureStorageAllocated(traceSamples + 1);
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
    void recomputeSceneScale()
    {
        // Frozen during any drag - the drag write-back calls setMovementClip() on every tick for
        // live feedback, which would otherwise re-fit the camera mid-drag and break the 1:1
        // screen<->world mapping screenToWorld()/the drag formulas assume. mouseUp() calls this
        // once more explicitly to re-fit after the drag ends.
        if (draggedHandle != DraggedHandle::None) return;

        if (pScalingInfo != nullptr && !pScalingInfo->IsInfinite())
        {
            sceneScale = 0.85 / juce::jmax(0.05, (double)pScalingInfo->CartesianMax());
            return;
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
        currentAnchor = hasMovementClip
            ? AnimatorMath::calculatePosition(movementClip, movementProgress, movementStart, referencePosition)
            : referencePosition;

        double stretch = 1.0;
        if (hasActionClip)
        {
            if (auto* stretchAction = findAction(ActionType::Stretch))
            {
                const auto actionCycle = AnimatorMath::computeCycleState(stretchProgress, actionClip.repetitions, actionClip.palindrome);
                stretch = AnimatorMath::calculateStretch(*stretchAction, actionCycle.cycleProgress, actionClip.length, stretchInitial, hasReferenceStretchFlag);
            }
        }

        const double magnitude = offsetMagnitude();

        for (int i = 0; i < numMembers; ++i)
        {
            auto offset = baseOffsetDirection(i) * magnitude;

            if (hasActionClip)
            {
                offset = offset * stretch;
                offset = rotateVector(offset, accumulatedRotation);

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
        DraggedHandle kind = DraggedHandle::None;
        bool isXYPanel = true;
        juce::Point<float> screenPos;
    };

    HandleHit findHandleAt(juce::Point<float> screenPos) const
    {
        HandleHit best;
        float bestDist = handleHitRadiusPx;

        auto consider = [&](DraggedHandle kind, bool isXY, juce::Point<float> candidateScreenPos)
        {
            const float d = screenPos.getDistanceFrom(candidateScreenPos);
            if (d <= bestDist) { bestDist = d; best = { kind, isXY, candidateScreenPos }; }
        };

        if (!hasMovementClip)
            return best;

        for (bool isXY : { true, false })
        {
            if (!getPanelBounds(isXY).contains(screenPos.roundToInt()))
                continue;

            const auto geom = getPanelGeometry(isXY);

            juce::Vector3D<double> targetWorld(movementClip.targetPointGroup.getX(), movementClip.targetPointGroup.getY(), movementClip.targetPointGroup.getZ());
            consider(DraggedHandle::MovementTarget, isXY, worldToScreen(targetWorld, geom, isXY));

            if (movementClip.useStartPoint)
            {
                juce::Vector3D<double> startWorld(movementClip.startPointGroup.getX(), movementClip.startPointGroup.getY(), movementClip.startPointGroup.getZ());
                consider(DraggedHandle::MovementStart, isXY, worldToScreen(startWorld, geom, isXY));
            }
        }

        return best;
    }

    // --- Mouse handling ---

    void mouseMove(const juce::MouseEvent& e) override
    {
        const auto hit = findHandleAt(e.position);
        setMouseCursor(hit.kind != DraggedHandle::None ? juce::MouseCursor::DraggingHandCursor
                                                        : juce::MouseCursor::NormalCursor);
        if (hit.kind != hoveredHandle)
        {
            hoveredHandle = hit.kind;
            repaint();
        }
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        if (hoveredHandle != DraggedHandle::None)
        {
            hoveredHandle = DraggedHandle::None;
            repaint();
        }
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        const auto hit = findHandleAt(e.position);
        draggedHandle = hit.kind;
        if (draggedHandle == DraggedHandle::None)
            return;

        draggedPanelIsXY = hit.isXYPanel;
        dragGrabOffsetScreen = hit.screenPos - e.position;
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (draggedHandle == DraggedHandle::None) return;
        const auto geom = getPanelGeometry(draggedPanelIsXY);

        const bool isStart = (draggedHandle == DraggedHandle::MovementStart);
        juce::Vector3D<double> currentWorld = isStart
            ? juce::Vector3D<double>(movementClip.startPointGroup.getX(), movementClip.startPointGroup.getY(), movementClip.startPointGroup.getZ())
            : juce::Vector3D<double>(movementClip.targetPointGroup.getX(), movementClip.targetPointGroup.getY(), movementClip.targetPointGroup.getZ());
        const auto newWorld = screenToWorld(e.position + dragGrabOffsetScreen, geom, draggedPanelIsXY, currentWorld);
        if (onPointDragged)
            onPointDragged(isStart, newWorld);
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        draggedHandle = DraggedHandle::None;
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
            juce::Vector3D<double> targetWorld(movementClip.targetPointGroup.getX(), movementClip.targetPointGroup.getY(), movementClip.targetPointGroup.getZ());
            drawPointHandle(g, toScreen(targetWorld), juce::Colours::orange, isHandleActive(DraggedHandle::MovementTarget));

            if (movementClip.useStartPoint)
            {
                juce::Vector3D<double> startWorld(movementClip.startPointGroup.getX(), movementClip.startPointGroup.getY(), movementClip.startPointGroup.getZ());
                drawPointHandle(g, toScreen(startWorld), juce::Colours::yellow, isHandleActive(DraggedHandle::MovementStart));
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

    MovementClip movementClip;
    bool hasMovementClip = false;
    double movementProgress = 0.0;
    AnimatorMath::MovementStartState movementStart;
    juce::Array<juce::Vector3D<double>> fullTrace;

    ActionClip actionClip;
    bool hasActionClip = false;
    double stretchProgress = 0.0;
    double stretchInitial = 1.0;
    juce::Vector3D<double> accumulatedRotation;
    double jitterElapsedSeconds = 0.0;

    juce::Vector3D<double> referencePosition;
    double referenceStretch = 1.0;
    bool hasReferenceStretchFlag = false;

    double sceneScale = 1.0;

    juce::Vector3D<double> currentAnchor;
    juce::Vector3D<double> currentMembers[numMembers];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipPreviewComponent)
};
