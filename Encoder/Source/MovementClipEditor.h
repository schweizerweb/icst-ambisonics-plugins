#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"

class TimelineComponent;

class PrecisionSlider : public juce::Slider
{
public:
    PrecisionSlider() = default;
    
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
    double threshold = 0.001;
    
    double normalizeNearZero(double value) const
    {
        if (std::abs(value) < threshold)
            return 0.0;
        return value;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionSlider)
};

class MovementClipEditor : public juce::Component, public juce::ChangeListener
{
public:
    MovementClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx);
    ~MovementClipEditor() override;
    
    void resized() override;
    void paint(juce::Graphics& g) override;
    
    int getTotalRequiredHeight() const;
    bool applyChanges();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    AmbiSourceSet* pSourceSet;
    PointSelection* pPointSelection;
    TimelineComponent& timelineComp;
    int timelineIndex, clipIndex;
    MovementClip currentClip;
    
    juce::Vector3D<double> currentPosition;
    bool currentPositionValid = false;
    
    CommonClipSettings commonSettings;
    
    juce::GroupComponent clipGroup{"Clip", "Clip Properties"};
    juce::GroupComponent movementGroup{"Movement", "Movement Properties"};
    
    juce::TextButton applyButton{"Apply"}, cancelButton{"Cancel"};
    juce::TextButton applyCurrentStartButton, applyCurrentTargetButton;
    
    juce::ToggleButton usePolarDisplay;
    juce::ToggleButton useStartPosition;
    
    // Movement type selector
    juce::ComboBox movementTypeCombo;
    juce::Label movementTypeLabel;

    PrecisionSlider startXSlider, startYSlider, startZSlider;
    PrecisionSlider targetXSlider, targetYSlider, targetZSlider;
    juce::Label startXLabel, startYLabel, startZLabel;
    juce::Label targetXLabel, targetYLabel, targetZLabel;
    
    // New properties
    PrecisionSlider countSlider;
    juce::Label countLabel;
    PrecisionSlider radiusChangeSlider;
    juce::Label radiusChangeLabel;

    void createControls();
    void createCoordinateSlider(PrecisionSlider& slider, juce::Label& label, const juce::String& name,
                               double min, double max, double defaultValue);
    void createApplyCurrentPositionButton(juce::TextButton& button, PrecisionSlider& xSlider, PrecisionSlider& ySlider, PrecisionSlider& zSlider);
    void updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid);
    void updateCurrentPosition(bool force = false);
    int getMovementControlsHeight() const;
    void layoutMovementControls(juce::Rectangle<int> area);
    void updateControlVisibility();
    void updateCoordinateSystem();
    void updateSliderLabelsAndRanges();  // New method to update UI based on coordinate system
    juce::Vector3D<double> getCurrentPositionInSelectedSystem() const;  // New method to get position in current coordinate system
    juce::String getCoordinateDisplayText(const juce::Vector3D<double>& vector, bool isValid) const;  // New method for coordinate display
    void onMovementTypeChanged();
};
