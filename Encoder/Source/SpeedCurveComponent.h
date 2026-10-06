#pragma once
#include "JuceHeader.h"
#include "AnimatorMath.h"
#include "../../Common/AdditionalWindow.h"
#include "../../Common/UiState.h"

// Visual editor for a clip's speed curve: a cubic Bezier from the bottom-left corner to the
// top-right, where x is elapsed time and y is distance covered. The slope therefore IS the speed,
// which is why the editor canvas is kept square - in a wide, short box a constant speed reads as a
// shallow line and the whole point is lost.
//
// One component, two modes. Thumbnail mode is small, non-interactive and lives permanently in
// CommonClipSettings so the clip's timing is visible at a glance; clicking it opens Editor mode in
// a popup, which has the room for draggable handles and presets. That split exists because the
// clip-properties column is only 280px wide and a Spline movement editor is already ~856px tall -
// a full inline editor would push it past what fits on a laptop screen.
class SpeedCurveComponent : public juce::Component,
                            public juce::SettableTooltipClient
{
public:
    enum class Mode { Thumbnail, Editor };

    explicit SpeedCurveComponent(Mode modeToUse) : mode(modeToUse)
    {
        setOpaque(false);
        if (mode == Mode::Thumbnail)
            setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    // Fired whenever the curve changes, so the host can push it into its clip and the preview.
    std::function<void()> onCurveChanged;

    // Thumbnail mode only: the user clicked, open the editor.
    std::function<void()> onClicked;

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
        const bool isEditor = (mode == Mode::Editor);

        // An explicit dark panel rather than a translucent wash: this draws on AdditionalWindow's
        // hardcoded white in the popup and on the editor's grey inline, and a translucent fill
        // would come out a different colour in each.
        g.setColour(juce::Colour(0xff20242b));
        g.fillRoundedRectangle(box, 3.0f);

        if (isEditor)
        {
            g.setColour(juce::Colours::white.withAlpha(0.07f));
            for (int i = 1; i < 4; ++i)
            {
                const float gx = box.getX() + box.getWidth() * (float)i / 4.0f;
                const float gy = box.getY() + box.getHeight() * (float)i / 4.0f;
                g.drawLine(gx, box.getY(), gx, box.getBottom(), 1.0f);
                g.drawLine(box.getX(), gy, box.getRight(), gy, 1.0f);
            }
        }

        // Constant speed, dashed so it can never be mistaken for the curve itself - without it
        // there's no reference to judge the curve's deviation against.
        {
            juce::Path reference, dashed;
            reference.startNewSubPath(box.getBottomLeft());
            reference.lineTo(box.getTopRight());

            const float dashes[] = { 4.0f, 4.0f };
            juce::PathStrokeType(1.0f).createDashedStroke(dashed, reference, dashes, 2);

            g.setColour(juce::Colours::white.withAlpha(isEditor ? 0.3f : 0.22f));
            g.fillPath(dashed);
        }

        g.setColour(juce::Colours::white.withAlpha(0.2f));
        g.drawRoundedRectangle(box, 3.0f, 1.0f);

        // The curve, sampled in x - which is exactly what the engine evaluates, so what is drawn
        // here is literally what plays.
        juce::Path path;
        constexpr int samples = 96;
        for (int i = 0; i <= samples; ++i)
        {
            const double x = (double)i / (double)samples;
            const auto p = toScreen(box, x, AnimatorMath::bezierEase(curve, x));
            if (i == 0) path.startNewSubPath(p); else path.lineTo(p);
        }

        g.setColour(curve.enabled ? juce::Colour(0xff6fa8ff) : juce::Colours::grey);
        g.strokePath(path, juce::PathStrokeType(isEditor ? 2.5f : 1.8f));

        if (!isEditor)
            return;

        // Handles, each tied back to the corner it belongs to, so it reads as a Bezier rather than
        // as two loose dots floating in a box.
        const auto origin = toScreen(box, 0.0, 0.0);
        const auto corner = toScreen(box, 1.0, 1.0);
        const auto h1 = toScreen(box, curve.x1, curve.y1);
        const auto h2 = toScreen(box, curve.x2, curve.y2);

        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.drawLine({ origin, h1 }, 1.0f);
        g.drawLine({ corner, h2 }, 1.0f);

        drawHandle(g, h1, draggedHandle == 1);
        drawHandle(g, h2, draggedHandle == 2);

        // Corner markers, so it's obvious which end is the clip's start and which its end.
        g.setColour(juce::Colours::white.withAlpha(0.45f));
        g.setFont(11.0f);
        g.drawText("start", (int)box.getX() + 4, (int)box.getBottom() - 16, 60, 14, juce::Justification::left);
        g.drawText("end", (int)box.getRight() - 64, (int)box.getY() + 2, 60, 14, juce::Justification::right);
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (mode == Mode::Thumbnail)
        {
            if (onClicked) onClicked();
            return;
        }

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
        if (mode != Mode::Editor || draggedHandle == 0) return;

        auto box = getCurveBounds();
        const double x = juce::jlimit(0.0, 1.0, (double)((e.position.x - box.getX()) / box.getWidth()));
        const double y = juce::jlimit(0.0, 1.0, (double)((box.getBottom() - e.position.y) / box.getHeight()));

        if (draggedHandle == 1) { curve.x1 = x; curve.y1 = y; }
        else                    { curve.x2 = x; curve.y2 = y; }

        // Clamping to the box is not cosmetic: it's what guarantees x(t) stays monotonic, i.e. that
        // time can never run backwards mid-clip. See AnimatorMath::EasingCurve.
        clampHandles();
        repaint();
        if (onCurveChanged) onCurveChanged();
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        if (draggedHandle != 0) { draggedHandle = 0; repaint(); }
    }

    // The presets are just handle positions - no separate code path - so choosing one and then
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
        auto area = getLocalBounds().toFloat().reduced(mode == Mode::Editor ? 12.0f : 1.0f);

        if (mode != Mode::Editor)
            return area;

        // Square, so slope reads as speed rather than as an artifact of the aspect ratio.
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

    Mode mode;
    AnimatorMath::EasingCurve curve;
    int draggedHandle = 0; // 0 none, 1 or 2 = which control point

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpeedCurveComponent)
};

// Contents of the popup: the square editor, an enable toggle, the preset buttons and the
// per-repetition/whole-clip scope choice.
class SpeedCurveEditorComponent : public juce::Component
{
public:
    SpeedCurveEditorComponent()
    {
        addAndMakeVisible(editor);
        editor.onCurveChanged = [this] { enableOnEdit(); notify(); };

        addAndMakeVisible(enabledToggle);
        enabledToggle.setButtonText("Use speed curve");
        enabledToggle.setTooltip("When off, the clip plays at a constant speed and the curve below is ignored.");
        enabledToggle.onClick = [this]
        {
            editor.setEnabledCurve(enabledToggle.getToggleState());
            updateEnablement();
            notify();
        };

        for (const auto& p : SpeedCurveComponent::presets())
        {
            auto* b = presetButtons.add(new juce::TextButton(p.name));
            addAndMakeVisible(b);
            b->onClick = [this, p] { editor.applyPreset(p); };
        }

        addAndMakeVisible(captionLabel);
        captionLabel.setText("Horizontal is time, vertical is distance covered. "
                             "The dashed line is constant speed - drag the two handles away from it.",
                             juce::dontSendNotification);
        captionLabel.setJustificationType(juce::Justification::topLeft);
        captionLabel.setFont(juce::Font(juce::FontOptions(12.0f)));
        captionLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.6f));

        addAndMakeVisible(scopeLabel);
        scopeLabel.setText("Applies to:", juce::dontSendNotification);

        addAndMakeVisible(scopeCombo);
        scopeCombo.addItem("Each repetition", 1);
        scopeCombo.addItem("Whole clip", 2);
        scopeCombo.setTooltip("Each repetition: every cycle eases identically. "
                              "Whole clip: time is warped across all repetitions, so the repetitions "
                              "themselves speed up or slow down.");
        scopeCombo.onChange = [this]
        {
            auto c = editor.getCurve();
            c.perRepetition = (scopeCombo.getSelectedId() != 2);
            editor.setCurve(c);
            notify();
        };

        setSize(430, 390);
    }

    std::function<void(const AnimatorMath::EasingCurve&)> onCurveChanged;

    void setCurve(const AnimatorMath::EasingCurve& c)
    {
        editor.setCurve(c);
        enabledToggle.setToggleState(c.enabled, juce::dontSendNotification);
        scopeCombo.setSelectedId(c.perRepetition ? 1 : 2, juce::dontSendNotification);
        updateEnablement();
    }

    // The scope choice is meaningless with a single forward cycle, so it's hidden rather than left
    // as a control that silently does nothing.
    void setScopeRelevant(bool relevant)
    {
        scopeLabel.setVisible(relevant);
        scopeCombo.setVisible(relevant);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(14);

        enabledToggle.setBounds(area.removeFromTop(24));
        area.removeFromTop(2);
        captionLabel.setBounds(area.removeFromTop(32));
        area.removeFromTop(8);

        auto scopeRow = area.removeFromBottom(26);
        scopeLabel.setBounds(scopeRow.removeFromLeft(78));
        scopeCombo.setBounds(scopeRow.removeFromLeft(180));
        area.removeFromBottom(10);

        // Presets stacked down the right so the canvas keeps as much square area as possible.
        auto presetColumn = area.removeFromRight(104);
        for (auto* b : presetButtons)
        {
            b->setBounds(presetColumn.removeFromTop(26));
            presetColumn.removeFromTop(5);
        }

        area.removeFromRight(12);
        editor.setBounds(area);
    }

private:
    void paint(juce::Graphics& g) override
    {
        // AdditionalWindow's DialogWindow background is hardcoded white; without this, Labels and
        // ToggleButtons (which paint a transparent background) use this app's light text colour and
        // become invisible - the enable toggle in particular vanished completely. Matches the other
        // animator dialogs, which each carry this same override for the same reason.
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    // Deliberately leaves the canvas and presets INTERACTIVE even when the curve is switched off.
    // Gating them behind the toggle meant that opening the editor on any clip that didn't already
    // have a curve - i.e. every existing clip, since the default is off - presented a dead panel
    // with no obvious way in. The toggle is for A/B-ing a curve you have already shaped, so touching
    // the curve simply switches it on (see enableOnEdit()). Only the alpha changes, as a hint that
    // the curve is currently not in effect.
    void updateEnablement()
    {
        editor.setAlpha(enabledToggle.getToggleState() ? 1.0f : 0.65f);
    }

    // Any edit to the curve implies wanting it, so switch it on rather than silently discarding the
    // drag. Applies to preset buttons too, since those route through editor.onCurveChanged.
    void enableOnEdit()
    {
        if (enabledToggle.getToggleState())
            return;

        enabledToggle.setToggleState(true, juce::dontSendNotification);
        editor.setEnabledCurve(true);
        updateEnablement();
    }

    void notify()
    {
        auto c = editor.getCurve();
        c.enabled = enabledToggle.getToggleState();
        c.perRepetition = (scopeCombo.getSelectedId() != 2);
        if (onCurveChanged) onCurveChanged(c);
    }

    SpeedCurveComponent editor { SpeedCurveComponent::Mode::Editor };
    juce::ToggleButton enabledToggle;
    juce::OwnedArray<juce::TextButton> presetButtons;
    juce::Label captionLabel;
    juce::Label scopeLabel;
    juce::ComboBox scopeCombo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpeedCurveEditorComponent)
};

// Popup window, following PreferencesDialog's pattern exactly - including the deferred close, since
// the callback deletes this window and must not run from inside its own close-button dispatch.
class SpeedCurveDialog : public AdditionalWindow
{
public:
    SpeedCurveDialog(std::unique_ptr<juce::Component> content, std::function<void()> onCloseRequested)
        : AdditionalWindow("Speed Curve", content.get()), closeCallback(std::move(onCloseRequested))
    {
        const int w = content->getWidth();
        const int h = content->getHeight();

        setAlwaysOnTop(true);
        setContentOwned(content.release(), true);
        setResizable(false, false);
        setUsingNativeTitleBar(false);

        const int titleBarHeight = getTitleBarHeight();
        setSize(w, h + titleBarHeight);
        UiState::restorePosition(*this, UiState::Windows::speedCurve);
    }

    ~SpeedCurveDialog() override
    {
        UiState::rememberPosition(*this, UiState::Windows::speedCurve);
    }

    void closeButtonPressed() override
    {
        if (closeCallback)
        {
            auto callback = closeCallback;
            juce::MessageManager::callAsync([callback] { callback(); });
        }
    }

private:
    std::function<void()> closeCallback;
};
