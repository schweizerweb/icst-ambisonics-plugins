#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"

class TimelineComponent;

class MovementClipEditor : public juce::Component
{
public:
    MovementClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx);
    
    void resized() override;
    void paint(juce::Graphics& g) override;
    
    int getTotalRequiredHeight() const;
    bool applyChanges();

private:
    TimelineComponent& timelineComp;
    int timelineIndex, clipIndex;
    MovementClip currentClip;
    
    CommonClipSettings commonSettings;
    
    juce::GroupComponent clipGroup{"Clip", "Clip Properties"};
    juce::GroupComponent movementGroup{"Movement", "Movement Properties"};
    
    juce::TextButton applyButton{"Apply"}, cancelButton{"Cancel"};
    
    juce::ToggleButton useStartPosition;
    juce::Slider startXSlider, startYSlider, startZSlider;
    juce::Slider targetXSlider, targetYSlider, targetZSlider;
    juce::Label startXLabel, startYLabel, startZLabel;
    juce::Label targetXLabel, targetYLabel, targetZLabel;
    
    void createControls();
    void createCoordinateSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                               float min, float max, float defaultValue);
    int getMovementControlsHeight() const;
    void layoutMovementControls(juce::Rectangle<int> area);
    void updateStartPositionVisibility();
};
