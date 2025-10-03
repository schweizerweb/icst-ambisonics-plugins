#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"
#include "../../Common/AmbiSourceSet.h"

class TimelineComponent;

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
    juce::Slider startXSlider, startYSlider, startZSlider;
    juce::Slider targetXSlider, targetYSlider, targetZSlider;
    juce::Label startXLabel, startYLabel, startZLabel;
    juce::Label targetXLabel, targetYLabel, targetZLabel;
    
    void createControls();
    void createCoordinateSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                               float min, float max, float defaultValue);
    void createApplyCurrentPositionButton(juce::TextButton& button, juce::Slider& xSlider, juce::Slider& ySlider, juce::Slider& zSlider);
    void updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid);
    void updateCurrentPosition();
    int getMovementControlsHeight() const;
    void layoutMovementControls(juce::Rectangle<int> area);
    void updateStartPositionVisibility();
};
