#pragma once
#include "JuceHeader.h"
#include "AnimatorMath.h"
#include "InfoIconButton.h"

// Editor for a clip's speed curve: a cubic Bezier from the bottom-left corner to the top-right,
// where x is elapsed time and y is distance covered. The slope therefore IS the speed, which is why
// the canvas is kept square - in a wide, short box a constant speed reads as a shallow line and the
// whole point is lost.
//
// This sits inline in the clip editors' Clip Properties column. It used to be a small thumbnail that
// opened a popup, because the column wasn't tall enough for the real thing; now that both clip
// dialogs are a fixed height with full-height columns, the editor fits where it belongs and the
// popup is gone.
class SpeedCurveComponent : public juce::Component,
                            public juce::SettableTooltipClient
{
public:
    SpeedCurveComponent()
    {
        setOpaque(false);
    }

    // Fired whenever the curve changes, so the host can push it into its clip and the preview.
    std::function<void()> onCurveChanged;

    void setCurve(const AnimatorMath::EasingCurve& newCurve)
    {
        curve = newCurve;
        clampHandles();
        repaint();
    }

    const AnimatorMath::EasingCurve& getCurve() const { return curve; }

    void setEnabledCurve(bool shouldBeEnabled)
    {
        curve.enabled = shouldBeEnabled;
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        auto box = getCurveBounds();

        // An explicit dark panel rather than a translucent wash, so it looks the same whatever it is
        // drawn on top of.
        g.setColour(juce::Colour(0xff20242b));
        g.fillRoundedRectangle(box, 3.0f);

        g.setColour(juce::Colours::white.withAlpha(0.07f));
        for (int i = 1; i < 4; ++i)
        {
            const float gx = box.getX() + box.getWidth() * (float)i / 4.0f;
            const float gy = box.getY() + box.getHeight() * (float)i / 4.0f;
            g.drawLine(gx, box.getY(), gx, box.getBottom(), 1.0f);
            g.drawLine(box.getX(), gy, box.getRight(), gy, 1.0f);
        }

        // Constant speed, dashed so it can never be mistaken for the curve itself - without it there
        // is no reference to judge the curve's deviation against.
        {
            juce::Path reference, dashed;
            reference.startNewSubPath(box.getBottomLeft());
            reference.lineTo(box.getTopRight());

            const float dashes[] = { 4.0f, 4.0f };
            juce::PathStrokeType(1.0f).createDashedStroke(dashed, reference, dashes, 2);

            g.setColour(juce::Colours::white.withAlpha(0.3f));
            g.fillPath(dashed);
        }

        g.setColour(juce::Colours::white.withAlpha(0.2f));
        g.drawRoundedRectangle(box, 3.0f, 1.0f);

        // The curve, sampled in x - exactly what the engine evaluates, so what is drawn here is
        // literally what plays.
        juce::Path path;
        constexpr int samples = 96;
        for (int i = 0; i <= samples; ++i)
        {
            const double x = (double)i / (double)samples;
            const auto p = toScreen(box, x, AnimatorMath::bezierEase(curve, x));
            if (i == 0) path.startNewSubPath(p); else path.lineTo(p);
        }

        g.setColour(curve.enabled ? juce::Colour(0xff6fa8ff) : juce::Colours::grey);
        g.strokePath(path, juce::PathStrokeType(2.5f));

        // Handles, each tied back to the corner it belongs to, so it reads as a Bezier rather than
        // as two loose dots floating in a box.
        const auto h1 = toScreen(box, curve.x1, curve.y1);
        const auto h2 = toScreen(box, curve.x2, curve.y2);

        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.drawLine({ toScreen(box, 0.0, 0.0), h1 }, 1.0f);
        g.drawLine({ toScreen(box, 1.0, 1.0), h2 }, 1.0f);

        drawHandle(g, h1, draggedHandle == 1);
        drawHandle(g, h2, draggedHandle == 2);

        g.setColour(juce::Colours::white.withAlpha(0.45f));
        g.setFont(11.0f);
        g.drawText("start", (int)box.getX() + 4, (int)box.getBottom() - 16, 60, 14, juce::Justification::left);
        g.drawText("end", (int)box.getRight() - 64, (int)box.getY() + 2, 60, 14, juce::Justification::right);
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        auto box = getCurveBounds();
        const float d1 = e.position.getDistanceFrom(toScreen(box, curve.x1, curve.y1));
        const float d2 = e.position.getDistanceFrom(toScreen(box, curve.x2, curve.y2));

        // Nearest handle wins, so the two stay grabbable even when dragged on top of each other.
        draggedHandle = (d1 <= d2) ? ((d1 <= handleHitRadius) ? 1 : 0)
                                   : ((d2 <= handleHitRadius) ? 2 : 0);
        repaint();
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (draggedHandle == 0) return;

        auto box = getCurveBounds();
        const double x = juce::jlimit(0.0, 1.0, (double)((e.position.x - box.getX()) / box.getWidth()));
        const double y = juce::jlimit(0.0, 1.0, (double)((box.getBottom() - e.position.y) / box.getHeight()));

        if (draggedHandle == 1) { curve.x1 = x; curve.y1 = y; }
        else                    { curve.x2 = x; curve.y2 = y; }

        // Clamping to the box is not cosmetic: it is what guarantees x(t) stays monotonic, i.e. that
        // time can never run backwards mid-clip. See AnimatorMath::EasingCurve.
        clampHandles();
        repaint();
        if (onCurveChanged) onCurveChanged();
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        if (draggedHandle != 0) { draggedHandle = 0; repaint(); }
    }

    // The presets are just handle positions - no separate code path - so picking one and then
    // dragging from it behaves exactly as expected.
    struct Preset { const char* name; double x1, y1, x2, y2; };

    static std::vector<Preset> presets()
    {
        return {
            { "Linear",      1.0 / 3.0, 1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0 },
            { "Ease In",     0.42, 0.0,  1.0,  1.0  },
            { "Ease Out",    0.0,  0.0,  0.58, 1.0  },
            { "Ease In-Out", 0.42, 0.0,  0.58, 1.0  },
            { "Slow Start",  0.75, 0.0,  1.0,  0.6  },
            { "Slow End",    0.0,  0.4,  0.25, 1.0  }
        };
    }

    void applyPreset(const Preset& p)
    {
        curve.x1 = p.x1; curve.y1 = p.y1; curve.x2 = p.x2; curve.y2 = p.y2;
        clampHandles();
        repaint();
        if (onCurveChanged) onCurveChanged();
    }

private:
    static constexpr float handleHitRadius = 14.0f;

    juce::Rectangle<float> getCurveBounds() const
    {
        // Square, so slope reads as speed rather than as an artifact of the aspect ratio.
        auto area = getLocalBounds().toFloat().reduced(8.0f);
        const float side = juce::jmin(area.getWidth(), area.getHeight());
        return area.withSizeKeepingCentre(side, side);
    }

    static juce::Point<float> toScreen(juce::Rectangle<float> box, double x, double y)
    {
        return { box.getX() + (float)x * box.getWidth(),
                 box.getBottom() - (float)y * box.getHeight() };
    }

    void drawHandle(juce::Graphics& g, juce::Point<float> p, bool active) const
    {
        const float r = active ? 7.0f : 5.5f;
        g.setColour(juce::Colours::white);
        g.fillEllipse(p.x - r, p.y - r, r * 2.0f, r * 2.0f);
        g.setColour(juce::Colours::cornflowerblue);
        g.fillEllipse(p.x - r + 2.0f, p.y - r + 2.0f, (r - 2.0f) * 2.0f, (r - 2.0f) * 2.0f);
    }

    void clampHandles()
    {
        curve.x1 = juce::jlimit(0.0, 1.0, curve.x1);
        curve.y1 = juce::jlimit(0.0, 1.0, curve.y1);
        curve.x2 = juce::jlimit(0.0, 1.0, curve.x2);
        curve.y2 = juce::jlimit(0.0, 1.0, curve.y2);
    }

    AnimatorMath::EasingCurve curve;
    int draggedHandle = 0; // 0 none, 1 or 2 = which control point

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpeedCurveComponent)
};

// The whole speed-curve control as it sits in the Clip Properties column: an enable toggle that
// doubles as the section heading, the square canvas with the preset buttons beside it, and the
// per-repetition scope.
//
// The canvas sits on the LEFT with the presets stacked to its right, rather than everything in one
// column - that keeps the canvas as large as the panel width allows while costing only its own
// height, instead of the presets and scope each adding a row underneath.
class SpeedCurveEditorComponent : public juce::Component
{
public:
    SpeedCurveEditorComponent()
    {
        addAndMakeVisible(enabledToggle);
        enabledToggle.setButtonText("Speed curve");
        enabledToggle.setTooltip("Shape how the clip's progress runs over its length. "
                                 "The dashed diagonal is constant speed.");
        enabledToggle.onClick = [this]
        {
            canvas.setEnabledCurve(enabledToggle.getToggleState());
            updateEnablement();
            notify();
        };

        addAndMakeVisible(canvas);
        // Any edit implies wanting the curve, so switch it on rather than discarding the drag. The
        // toggle is for A/B-ing a curve already shaped, not a gate to be found first.
        canvas.onCurveChanged = [this] { enableOnEdit(); notify(); };

        for (const auto& preset : SpeedCurveComponent::presets())
        {
            auto* b = presetButtons.add(new juce::TextButton(preset.name));
            addAndMakeVisible(b);
            b->setConnectedEdges(juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
            b->onClick = [this, preset]
            {
                canvas.applyPreset(preset);
                enableOnEdit();
                notify();
            };
        }

        addAndMakeVisible(perRepetitionToggle);
        perRepetitionToggle.setButtonText("Per repetition");
        perRepetitionToggle.setToggleState(true, juce::dontSendNotification);
        // Turning this off removes the segmentation entirely, which is what "Per direction" divides,
        // so the two have to be re-evaluated together.
        perRepetitionToggle.onClick = [this] { updateScopeEnablement(); notify(); };

        addAndMakeVisible(perRepetitionInfo);

        addAndMakeVisible(perDirectionToggle);
        perDirectionToggle.setButtonText("Per direction");
        perDirectionToggle.setToggleState(true, juce::dontSendNotification);
        perDirectionToggle.onClick = [this] { notify(); };

        addAndMakeVisible(perDirectionInfo);
    }

    std::function<void(const AnimatorMath::EasingCurve&)> onCurveChanged;

    void setCurve(const AnimatorMath::EasingCurve& c)
    {
        canvas.setCurve(c);
        enabledToggle.setToggleState(c.enabled, juce::dontSendNotification);
        perRepetitionToggle.setToggleState(c.perRepetition, juce::dontSendNotification);
        perDirectionToggle.setToggleState(c.perDirection, juce::dontSendNotification);
        updateEnablement(); // also refreshes the two scope toggles
    }

    AnimatorMath::EasingCurve getCurve() const
    {
        auto c = canvas.getCurve();
        c.enabled = enabledToggle.getToggleState();
        c.perRepetition = perRepetitionToggle.getToggleState();
        c.perDirection = perDirectionToggle.getToggleState();
        return c;
    }

    // Both controls stay VISIBLE and are greyed out when they'd have no effect - a control that
    // vanishes as you change Repetitions or Palindrome is harder to find than one that is simply
    // inactive, and keeping them in place means the panel's height never changes.
    void setScopeApplicable(bool repetitionApplicable, bool directionApplicable)
    {
        repetitionApplies = repetitionApplicable;
        directionApplies = directionApplicable;
        updateScopeEnablement();
    }

    // Reported so the host can keep the clip in step when "Per repetition" is pinned on at a single
    // repetition - otherwise the stored value and the shown value could disagree.
    bool isPerRepetitionPinned() const { return !repetitionApplies; }

    static int getRowHeight()  { return 24; }
    static int getRowSpacing() { return 6; }

    // Height needed for a given panel WIDTH: the canvas is square and takes whatever width is left
    // beside the preset column, so the width decides how tall the whole thing is.
    static int getRequiredHeight(int width)
    {
        const int canvasSide = juce::jmax(minCanvasSide, width - presetColumnWidth - presetGap);

        // Both scope rows are always budgeted, even though each hides when irrelevant - otherwise
        // ticking Palindrome would have to grow the dialog, which is exactly what the fixed height
        // exists to prevent.
        return getRowHeight() + getRowSpacing()                      // enable toggle
             + canvasSide + getRowSpacing()                          // canvas (square)
             + 2 * getRowHeight() + getRowSpacing();                 // per-repetition + per-direction
    }

    void resized() override
    {
        auto area = getLocalBounds();

        enabledToggle.setBounds(area.removeFromTop(getRowHeight()));
        area.removeFromTop(getRowSpacing());

        // Laid out bottom-up: Per direction below Per repetition. Both rows are always present.
        auto layoutScopeRow = [&](juce::ToggleButton& toggle, InfoIconButton& info)
        {
            auto scopeRow = area.removeFromBottom(getRowHeight());
            info.setBounds(scopeRow.removeFromRight(getRowHeight()).reduced(2, 2));
            scopeRow.removeFromRight(4);
            toggle.setBounds(scopeRow);
        };

        layoutScopeRow(perDirectionToggle, perDirectionInfo);
        layoutScopeRow(perRepetitionToggle, perRepetitionInfo);
        area.removeFromBottom(getRowSpacing());

        // Presets down the right, canvas takes the rest and stays square.
        auto presetColumn = area.removeFromRight(presetColumnWidth);
        area.removeFromRight(presetGap);
        canvas.setBounds(area);

        const int buttons = presetButtons.size();
        if (buttons > 0)
        {
            const int spacing = 3;
            const int each = juce::jmax(16, (presetColumn.getHeight() - (buttons - 1) * spacing) / buttons);

            for (auto* b : presetButtons)
            {
                b->setBounds(presetColumn.removeFromTop(each));
                presetColumn.removeFromTop(spacing);
            }
        }
    }

private:
    static constexpr int presetColumnWidth = 96;
    static constexpr int presetGap = 8;
    static constexpr int minCanvasSide = 110;

    void updateScopeEnablement()
    {
        const bool curveOn = enabledToggle.getToggleState();

        // At a single repetition "Per repetition" is redundant: off (ease the whole clip) and on
        // with "Per direction" off (ease the one repetition) describe the same single segment. So it
        // is pinned ON rather than merely greyed - otherwise a clip left with it off would strand
        // "Per direction", which only has meaning inside a segmented curve.
        if (!repetitionApplies && !perRepetitionToggle.getToggleState())
            perRepetitionToggle.setToggleState(true, juce::dontSendNotification);

        perRepetitionToggle.setEnabled(curveOn && repetitionApplies);

        // "Per direction" divides the per-repetition segmentation, so it means nothing once that is
        // switched off - even with Palindrome on.
        perDirectionToggle.setEnabled(curveOn && directionApplies && perRepetitionToggle.getToggleState());

        perRepetitionToggle.setAlpha(perRepetitionToggle.isEnabled() ? 1.0f : 0.5f);
        perDirectionToggle.setAlpha(perDirectionToggle.isEnabled() ? 1.0f : 0.5f);
    }

    void updateEnablement()
    {
        // With the curve switched off, everything that shapes it goes inactive too - the toggle is
        // the one live control, so there is no question about what to click first.
        //
        // The info icons are the deliberate exception: they explain why a control is greyed out,
        // which is exactly when that explanation is worth reading.
        const bool curveOn = enabledToggle.getToggleState();

        canvas.setEnabled(curveOn);
        canvas.setAlpha(curveOn ? 1.0f : 0.4f);

        for (auto* b : presetButtons)
            b->setEnabled(curveOn);

        updateScopeEnablement();
    }

    void enableOnEdit()
    {
        if (enabledToggle.getToggleState())
            return;

        enabledToggle.setToggleState(true, juce::dontSendNotification);
        canvas.setEnabledCurve(true);
        updateEnablement();
    }

    void notify()
    {
        if (onCurveChanged)
            onCurveChanged(getCurve());
    }

    SpeedCurveComponent canvas;
    juce::ToggleButton enabledToggle;
    juce::OwnedArray<juce::TextButton> presetButtons;
    juce::ToggleButton perRepetitionToggle;
    juce::ToggleButton perDirectionToggle;
    bool repetitionApplies = false;
    bool directionApplies = false;

    InfoIconButton perRepetitionInfo
    {
        "Per repetition: every cycle of this clip eases identically - three repetitions give three "
        "identical eased moves.\n\n"
        "Off: the curve warps time across the whole clip instead, so the repetitions themselves "
        "speed up or slow down over the clip's length.\n\n"
        "Only has an effect when Repetitions is above 1, or Palindrome is on."
    };

    InfoIconButton perDirectionInfo
    {
        "Palindrome plays each repetition out and then back again. This decides whether the speed "
        "curve describes one LEG of that journey or the whole round trip.\n\n"
        "On: the outward leg and the return leg each follow the curve, so both ease the same way.\n\n"
        "Off: a single curve is stretched across the whole go-and-back. The turnaround then happens "
        "where the curve reaches half its travel, which need not be halfway through in time - an "
        "ease-out, for instance, races out and creeps back."
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpeedCurveEditorComponent)
};
