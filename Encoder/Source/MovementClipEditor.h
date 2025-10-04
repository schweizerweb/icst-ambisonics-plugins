#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"

class TimelineComponent;

class UnlimitedRangeSlider : public juce::Slider
{
public:
    double getValueFromText(const juce::String& text) override
    {
        // Allow any numeric value from text input
        return text.getDoubleValue();
    }
    
    juce::String getTextFromValue(double value) override
    {
        // Always display the actual value
        return juce::String(value, 2);
    }
    
private:
    // Override to allow thumb to move beyond range visually
    double snapValue(double attemptedValue, DragMode) override
    {
        return attemptedValue; // No snapping to range
    }
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
    
    juce::ToggleButton useStartPosition;
    UnlimitedRangeSlider startXSlider, startYSlider, startZSlider;
    UnlimitedRangeSlider targetXSlider, targetYSlider, targetZSlider;
    juce::Label startXLabel, startYLabel, startZLabel;
    juce::Label targetXLabel, targetYLabel, targetZLabel;
    
    void createControls();
    void createCoordinateSlider(UnlimitedRangeSlider& slider, juce::Label& label, const juce::String& name,
                               double min, double max, double defaultValue);
    void createApplyCurrentPositionButton(juce::TextButton& button, UnlimitedRangeSlider& xSlider, UnlimitedRangeSlider& ySlider, UnlimitedRangeSlider& zSlider);
    void updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid);
    void updateCurrentPosition();
    int getMovementControlsHeight() const;
    void layoutMovementControls(juce::Rectangle<int> area);
    void updateStartPositionVisibility();
};
