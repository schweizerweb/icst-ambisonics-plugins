#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"
#include "ClipPreviewComponent.h"
#include "ClipEditorCloseGuard.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"

class TimelineComponent;

class PrecisionSlider : public juce::Slider
{
public:
    PrecisionSlider()
    {
        // juce::Slider has no direct API to right-align its value text box, so this goes through
        // a LookAndFeel override instead. Scoped to a dedicated shared instance (not the app's
        // default LookAndFeel), so it only affects PrecisionSliders, not every slider in the app.
        setLookAndFeel(&getRightAlignedLookAndFeel());
    }

    ~PrecisionSlider() override
    {
        setLookAndFeel(nullptr);
    }

    double getPreciseValue()
    {
        return normalizeNearZero(Slider::getValue());
    }

    double getValue() const
    {
        return normalizeNearZero(Slider::getValue());
    }

    void setPrecisionThreshold(double newThreshold) { threshold = newThreshold; }
    double getPrecisionThreshold() const { return threshold; }

private:
    class RightAlignedTextBoxLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        juce::Label* createSliderTextBox(juce::Slider& slider) override
        {
            auto* label = juce::LookAndFeel_V4::createSliderTextBox(slider);
            label->setJustificationType(juce::Justification::centredRight);
            return label;
        }
    };

    static RightAlignedTextBoxLookAndFeel& getRightAlignedLookAndFeel()
    {
        static RightAlignedTextBoxLookAndFeel lookAndFeel;
        return lookAndFeel;
    }

    double threshold = 0.001;

    double normalizeNearZero(double value) const
    {
        if (std::abs(value) < threshold)
            return 0.0;
        return value;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionSlider)
};

// A coordinate field that behaves as a draggable slider for bounded (finite-scaled) values, or
// as an IncDecButtons "updown" spinner (same style as the Count/Radius change fields) for
// unbounded ones - a draggable slider whose range spans an "infinite" scaler makes little sense,
// since almost its entire travel would represent values near zero. The updown style has no drag
// track to misrepresent, so it stays usable no matter how large the range is.
class CoordinateValueControl : public juce::Component
{
public:
    CoordinateValueControl()
    {
        addAndMakeVisible(slider);
        applyStyle();
    }

    void setRange(double min, double max, double step)
    {
        slider.setRange(min, max, step);
    }

    void setValue(double value)
    {
        slider.setValue(value, juce::dontSendNotification);
    }

    double getPreciseValue() const
    {
        return slider.getValue();
    }

    double getValue() const
    {
        return getPreciseValue();
    }

    void setUpDownStyle(bool shouldUseUpDown)
    {
        if (useUpDown == shouldUseUpDown)
            return;

        useUpDown = shouldUseUpDown;
        applyStyle();
        resized();
    }

private:
    void applyStyle()
    {
        if (useUpDown)
        {
            slider.setSliderStyle(juce::Slider::IncDecButtons);
            slider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 22);
        }
        else
        {
            slider.setSliderStyle(juce::Slider::LinearHorizontal);
            slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, textBoxWidth, 22);
        }
    }

    void resized() override
    {
        if (useUpDown)
        {
            // The +/- buttons fill whatever's left of the slider's own bounds once the text box
            // is subtracted, so without an explicit width they stretch to fill the whole row.
            // Cap the total at the same width as the text box, matching the draggable slider's
            // footprint, and anchor it to the right of whatever space is available so it lines up
            // under the label column's right edge rather than trailing off with empty space after it.
            slider.setBounds(getLocalBounds().removeFromRight(textBoxWidth * 2));
        }
        else
        {
            slider.setBounds(getLocalBounds());
        }
    }

    static constexpr int textBoxWidth = 70;
    bool useUpDown = false;
    PrecisionSlider slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoordinateValueControl)
};

class MovementClipEditor : public juce::Component, public juce::ChangeListener,
                            public ClipEditorCloseGuard, private juce::Timer
{
public:
    MovementClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx);
    ~MovementClipEditor() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    int getTotalRequiredHeight() const;
    int getTotalRequiredWidth() const;
    bool applyChanges();

    void changeListenerCallback(ChangeBroadcaster* source) override;

    // True if the live UI state differs from dirtyBaseline (see its own comment below).
    bool isDirty();
    bool confirmDiscardIfDirty() override;

private:
    AmbiSourceSet* pSourceSet;
    PointSelection* pPointSelection;
    TimelineComponent& timelineComp;
    int timelineIndex, clipIndex;
    MovementClip currentClip;
    // What buildClipFromControls() produced right after the controls were first populated from
    // currentClip, before any user interaction - NOT simply a copy of currentClip itself. The
    // coordinate sliders snap to their configured interval (0.01 for X/Y/Z, 0.1 for Azimuth/
    // Elevation - see juce::Slider::setValue()/NormalisableRange::snapToLegalValue()), so loading
    // currentClip's full-precision values into them already quantizes away anything finer than that
    // grid. Comparing a later buildClipFromControls() against raw currentClip would then see that
    // quantization itself as a "change" and report dirty on a freshly opened, untouched dialog -
    // comparing against this already-quantized baseline instead makes it an apples-to-apples
    // comparison, so only a genuine edit moves it off the grid point it started on.
    MovementClip dirtyBaseline;
    
    juce::Vector3D<double> currentPosition;
    bool currentPositionValid = false;
    
    CommonClipSettings commonSettings;
    ClipPreviewComponent preview;

    juce::GroupComponent clipGroup{"Clip", "Clip Properties"};
    juce::GroupComponent movementGroup{"Movement", "Movement Properties"};
    juce::GroupComponent previewGroup{"Preview", "Preview"};
    
    juce::TextButton applyButton{"Apply"}, cancelButton{"Cancel"};
    juce::TextButton applyCurrentStartButton, applyCurrentTargetButton;
    
    juce::ToggleButton usePolarDisplay;
    juce::ToggleButton useStartPosition;
    
    // Movement type selector
    juce::ComboBox movementTypeCombo;
    juce::Label movementTypeLabel;

    CoordinateValueControl startXSlider, startYSlider, startZSlider;
    CoordinateValueControl targetXSlider, targetYSlider, targetZSlider;
    juce::Label startXLabel, startYLabel, startZLabel;
    juce::Label targetXLabel, targetYLabel, targetZLabel;

    // New properties
    CoordinateValueControl countSlider;
    juce::Label countLabel;
    CoordinateValueControl radiusChangeSlider;
    juce::Label radiusChangeLabel;

    // MoveTo (Cartesian/Polar) ends at a different point than it started, so a non-palindrome
    // repeat always jumps at the repeat boundary - unlike Circle/Spiral, which stay at least close
    // to their start angle/radius every pass. Shared by createControls() (initial state) and
    // onMovementTypeChanged() (live updates).
    static bool isMoveToType(MovementType type) { return type == MovementType::MoveToCartesian || type == MovementType::MoveToPolar; }

    void createControls();
    void createCoordinateSlider(CoordinateValueControl& slider, juce::Label& label, const juce::String& name,
                               double min, double max, double defaultValue);
    void createStandardSlider(CoordinateValueControl& slider, juce::Label& label, const juce::String& name, double defaultValue);
    void createApplyCurrentPositionButton(juce::TextButton& button, CoordinateValueControl& xSlider, CoordinateValueControl& ySlider, CoordinateValueControl& zSlider);
    void updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid);
    void updateCurrentPosition(bool force = false);
    int getMovementControlsHeight() const;
    // Preview sits beside (not below) the Clip Properties group, in portrait orientation (two
    // square panels stacked) - narrower but taller than the old side-by-side layout.
    int getClipPropertiesWidth() const { return 280; }
    int getPreviewWidth() const { return 190; }
    int getPreviewHeight() const { return 320; }
    void layoutMovementControls(juce::Rectangle<int> area);
    void updateControlVisibility();
    void updateCoordinateSystem();
    void updateSliderLabelsAndRanges();  // New method to update UI based on coordinate system
    juce::Vector3D<double> getCurrentPositionInSelectedSystem() const;  // New method to get position in current coordinate system
    // Shared Cartesian->Polar conversion used by both getCurrentPositionInSelectedSystem() and the
    // preview's onPointDragged callback - see MovementClipEditor.cpp.
    juce::Vector3D<double> convertCartesianToSelectedSystem(juce::Vector3D<double> cartesian) const;
    juce::String getCoordinateDisplayText(const juce::Vector3D<double>& vector, bool isValid) const;  // New method for coordinate display
    void onMovementTypeChanged();

    // Clip editors have no per-control change notification, so the preview is kept live by
    // polling the controls instead of wiring ~12 individual callbacks - see ClipPreviewComponent.
    MovementClip buildClipFromControls();
    void timerCallback() override { preview.setMovementClip(buildClipFromControls()); }
};
