#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"
#include "AnimatorMath.h"
#include "../../Common/PerlinNoise.h"

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
// World-to-screen mapping is a self-computed linear auto-fit (see recomputeSceneScale()), not the
// project's real ScalingInfo - that matters: ScalingInfo::compress() runs values through atan() in
// "infinite" scaling mode (the common case), which visually compresses motion nonlinearly the
// farther it gets from the origin. A clip with constant real speed would then look like it's
// decelerating on screen even though the simulation itself is perfectly linear - exactly the kind
// of display-only distortion that would make it impossible to tell a clip's own future speed/
// easing curves from an artifact of this preview. Auto-fitting to the preview's own content keeps
// the mapping linear (uniform time -> uniform screen distance) regardless of where in the real
// scene the clip happens to be.
//
// Looping semantics intentionally differ by type, matching what repeating the real clip would
// actually look like: MoveTo/Circle/Spiral/Stretch have a well-defined 0..1 progress tied to clip
// length, so they wrap and restart each loop. Rotation and Jitter have no such bound in the real
// engine (rotation accumulates forever, jitter has no cycle) - the preview lets them run forever
// too, rather than forcing an artificial reset.
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

private:
    static constexpr int numMembers = 3;
    static constexpr int traceSamples = 200;

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
                // extent is derived from stretchInitial (see below), so the camera must be
                // refreshed here too, or a loop that restarts from a very different stretch than
                // the one the camera was framed for will send the formation flying off-panel.
                // This is a deliberate, rare exception to "never recompute mid-animation" above -
                // it fires once per loop restart, not every tick, so it doesn't reintroduce breathing.
                recomputeSceneScale();
            }

            for (const auto& actionDef : actionClip.actions)
            {
                if (actionDef.getAction() == ActionType::RotationX ||
                    actionDef.getAction() == ActionType::RotationY ||
                    actionDef.getAction() == ActionType::RotationZ)
                {
                    accumulatedRotation += AnimatorMath::calculateRotationTickRadians(actionDef, tickMs, actionClip.length);
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

    // Purely an internal display constant now (world-to-screen is auto-fit, see
    // recomputeSceneScale()), not tied to the real scene's scale - any fixed positive value works
    // equally well, since the camera normalises it to fill the panel regardless.
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

    // A single uniform (non axis-distorting) linear scale, fit to the preview's own content -
    // recomputed only when the clip/parameters actually change, not every animation tick, so the
    // camera stays still while rotation/jitter/stretch move within it rather than "breathing" as
    // those wobble.
    //
    // The world origin (0,0,0) is always the panel centre - matching the main Radar view's fixed
    // reference frame - rather than auto-centring on wherever the content's own bounding box
    // happens to be. Centring on content was the bug behind "a movement starting at the centre
    // shows as running edge-to-edge": a MoveTo from (0,0,0) rightward has a trace spanning
    // [0, target], and centring on *that* puts its midpoint - not the origin - in the middle of
    // the panel, so the start point gets dragged out to the left edge instead of staying put at
    // the centre where it actually is.
    void recomputeSceneScale()
    {
        double maxDistanceFromOrigin = 0.0;

        auto include = [&](juce::Vector3D<double> p)
        {
            maxDistanceFromOrigin = juce::jmax(maxDistanceFromOrigin, std::abs(p.x));
            maxDistanceFromOrigin = juce::jmax(maxDistanceFromOrigin, std::abs(p.y));
            maxDistanceFromOrigin = juce::jmax(maxDistanceFromOrigin, std::abs(p.z));
        };

        if (hasMovementClip)
            for (const auto& p : fullTrace)
                include(p);

        if (hasActionClip)
        {
            // Not sampled from live (potentially jittering/rotating) positions - estimated from
            // the clip's own parameters, generously padded, so the camera doesn't need to move
            // every frame to keep up with the content it's framing.
            double maxJitterIntensity = 0.0;
            // Floored at 1.0 so the unstretched formation (progress 0 of a loop that hasn't
            // reached its first wrap yet) is always framed, even if a live reference stretch
            // would otherwise shrink the estimate below that.
            double maxAbsStretch = 1.0;

            for (const auto& actionDef : actionClip.actions)
            {
                if (actionDef.getAction() == ActionType::Jitter)
                {
                    maxJitterIntensity = juce::jmax(maxJitterIntensity, actionDef.getValue());
                }
                else if (actionDef.getAction() == ActionType::Stretch && actionDef.getTiming() != TimingType::None)
                {
                    // calculateStretch() is linear in progress for every TimingType, so its two
                    // endpoints bound the whole loop - evaluated against stretchInitial, the same
                    // initial state computeCurrentFormation() will actually animate from.
                    const double s0 = AnimatorMath::calculateStretch(actionDef, 0.0, actionClip.length, stretchInitial, hasReferenceStretchFlag);
                    const double s1 = AnimatorMath::calculateStretch(actionDef, 1.0, actionClip.length, stretchInitial, hasReferenceStretchFlag);
                    maxAbsStretch = juce::jmax(maxAbsStretch, juce::jmax(std::abs(s0), std::abs(s1)));
                }
            }

            const double extent = offsetMagnitude() * maxAbsStretch * 2.2 + maxJitterIntensity;
            include(referencePosition + juce::Vector3D<double>(extent, extent, extent));
            include(referencePosition - juce::Vector3D<double>(extent, extent, extent));
        }

        // A little headroom (85% fill) so content doesn't touch the panel edge; floored so a
        // clip that's entirely at the origin (e.g. a fresh, untouched clip) doesn't divide by zero.
        sceneScale = 0.85 / juce::jmax(0.05, maxDistanceFromOrigin);
    }

    void computeCurrentFormation()
    {
        currentAnchor = hasMovementClip
            ? AnimatorMath::calculatePosition(movementClip, movementProgress, movementStart, referencePosition)
            : referencePosition;

        double stretch = 1.0;
        if (hasActionClip)
        {
            for (const auto& actionDef : actionClip.actions)
            {
                if (actionDef.getAction() == ActionType::Stretch && actionDef.getTiming() != TimingType::None)
                {
                    stretch = AnimatorMath::calculateStretch(actionDef, stretchProgress, actionClip.length, stretchInitial, hasReferenceStretchFlag);
                    break;
                }
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

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff1a1f24));

        // Portrait order: top-down XY above, front XZ below.
        auto area = getLocalBounds().reduced(4);
        auto xyArea = area.removeFromTop(area.getHeight() / 2).reduced(2);
        auto xzArea = area.reduced(2);

        drawPanel(g, xyArea, true);
        drawPanel(g, xzArea, false);
    }

    void drawPanel(juce::Graphics& g, juce::Rectangle<int> bounds, bool isXY)
    {
        const int side = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto square = juce::Rectangle<int>(0, 0, side, side).withCentre(bounds.getCentre());

        g.setColour(juce::Colour(0xff0d1013));
        g.fillRect(square);
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.drawRect(square, 1);

        const auto center = square.getCentre().toFloat();
        const float radius = (float)side * 0.48f;

        g.setColour(juce::Colours::white.withAlpha(0.12f));
        g.drawLine(center.x - radius, center.y, center.x + radius, center.y, 1.0f);
        g.drawLine(center.x, center.y - radius, center.x, center.y + radius, 1.0f);
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f, 1.0f);

        g.setFont(juce::FontOptions(10.0f));
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.drawText(isXY ? "XY" : "XZ", square.removeFromTop(14).reduced(3, 0), juce::Justification::topLeft);

        auto toScreen = [&](juce::Vector3D<double> worldPos) -> juce::Point<float>
        {
            const auto rel = worldPos * sceneScale;
            const double a = rel.x;
            const double b = isXY ? rel.y : rel.z;
            return { center.x + (float)a * radius, center.y - (float)b * radius };
        };

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
    }

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
