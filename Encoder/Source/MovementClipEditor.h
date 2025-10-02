#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"
#include "TimelineComponent.h"

class MovementClipEditor : public juce::Component
{
public:
    MovementClipEditor(TimelineComponent& timeline, int timelineIndex, int layerIndex, int clipIndex)
        : timelineComp(timeline), timelineIndex(timelineIndex), layerIndex(layerIndex), clipIndex(clipIndex)
    {
        if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
        {
            if (clipIndex >= 0 && clipIndex < timelineModel->movement.clips.size())
            {
                currentClip = timelineModel->movement.clips.getReference(clipIndex);
            }
        }
        
        createControls();
    }
    
    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        
        // Calculate group heights
        const int clipGroupHeight = commonSettings.getRequiredHeight() + 40;
        const int movementGroupHeight = getMovementControlsHeight() + 40;
        const int buttonHeight = 28;
        
        // Clip settings group
        auto clipGroupArea = area.removeFromTop(clipGroupHeight);
        clipGroup.setBounds(clipGroupArea);
        commonSettings.setBounds(clipGroupArea.reduced(8, 20));
        
        area.removeFromTop(8); // Spacing between groups
        
        // Movement settings group
        auto movementGroupArea = area.removeFromTop(movementGroupHeight);
        movementGroup.setBounds(movementGroupArea);
        
        auto movementContentArea = movementGroupArea.reduced(8, 20);
        layoutMovementControls(movementContentArea);
        
        // Buttons at bottom
        auto buttonArea = area.removeFromBottom(buttonHeight).reduced(10, 0);
        cancelButton.setBounds(buttonArea.removeFromRight(80));
        buttonArea.removeFromRight(8); // Button spacing
        applyButton.setBounds(buttonArea.removeFromRight(80));
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }
    
    int getTotalRequiredHeight() const
    {
        const int margins = 10 * 2;
        const int clipGroupHeight = commonSettings.getRequiredHeight() + 30;
        const int movementGroupHeight = getMovementControlsHeight() + 40;
        const int buttonHeight = 28;
        const int groupSpacing = 8;
        
        return margins + clipGroupHeight + groupSpacing + movementGroupHeight + buttonHeight;
    }
    
    bool applyChanges()
    {
        if (!commonSettings.validate())
            return false;
            
        commonSettings.applyToClip(currentClip);
        
        currentClip.useStartPoint = useStartPosition.getToggleState();
        currentClip.startPointGroup.setXYZ(startXSlider.getValue(), startYSlider.getValue(), startZSlider.getValue());
        currentClip.endPointGroup.setXYZ(targetXSlider.getValue(), targetYSlider.getValue(), targetZSlider.getValue());
        
        if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
        {
            if (clipIndex >= 0 && clipIndex < timelineModel->movement.clips.size())
            {
                timelineModel->movement.clips.getReference(clipIndex) = currentClip;
                return true;
            }
        }
        
        return false;
    }
    
private:
    TimelineComponent& timelineComp;
    int timelineIndex, layerIndex, clipIndex;
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
    
    void createControls()
    {
        setOpaque(true);
        
        addAndMakeVisible(clipGroup);
        addAndMakeVisible(movementGroup);
        
        addAndMakeVisible(commonSettings);
        commonSettings.setClipData(currentClip);
        
        addAndMakeVisible(applyButton);
        applyButton.onClick = [this] {
            if (applyChanges())
            {
                timelineComp.repaint();
                if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
                    dw->exitModalState(1);
            }
        };
        
        addAndMakeVisible(cancelButton);
        cancelButton.onClick = [this] {
            if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
                dw->exitModalState(0);
        };
        
        addAndMakeVisible(useStartPosition);
        useStartPosition.setButtonText("Use Custom Start Position");
        useStartPosition.setToggleState(currentClip.useStartPoint, juce::dontSendNotification);
        useStartPosition.onClick = [this] { updateStartPositionVisibility(); };
        
        // Create coordinate sliders for start and target positions
        createCoordinateSlider(startXSlider, startXLabel, "Start X:", -10.0f, 10.0f, currentClip.startPointGroup.getX());
        createCoordinateSlider(startYSlider, startYLabel, "Start Y:", -10.0f, 10.0f, currentClip.startPointGroup.getY());
        createCoordinateSlider(startZSlider, startZLabel, "Start Z:", -10.0f, 10.0f, currentClip.startPointGroup.getZ());
        
        createCoordinateSlider(targetXSlider, targetXLabel, "Target X:", -10.0f, 10.0f, currentClip.endPointGroup.getX());
        createCoordinateSlider(targetYSlider, targetYLabel, "Target Y:", -10.0f, 10.0f, currentClip.endPointGroup.getY());
        createCoordinateSlider(targetZSlider, targetZLabel, "Target Z:", -10.0f, 10.0f, currentClip.endPointGroup.getZ());
        
        updateStartPositionVisibility();
    }
    
    void createCoordinateSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                               float min, float max, float defaultValue)
    {
        addAndMakeVisible(slider);
        slider.setRange(min, max, 0.01f);
        slider.setValue(defaultValue);
        slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 22);
        
        addAndMakeVisible(label);
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centredLeft);
    }
    
    int getMovementControlsHeight() const
    {
        const int rowHeight = 28;
        const int verticalSpacing = 8;
        
        // Checkbox + 3 start sliders + 3 target sliders + spacing
        return rowHeight + verticalSpacing + (rowHeight * 3) + verticalSpacing + (rowHeight * 3);
    }
    
    void layoutMovementControls(juce::Rectangle<int> area)
    {
        const int rowHeight = 28;
        const int labelWidth = 100;
        const int verticalSpacing = 8;
        const int rightMargin = 10;
        const int labelLeftMargin = 10; // Labels indented from group box
        
        // Checkbox
        auto checkboxArea = area.removeFromTop(rowHeight);
        useStartPosition.setBounds(checkboxArea.withTrimmedLeft(labelLeftMargin));
        
        area.removeFromTop(verticalSpacing);
        
        // Start position controls
        auto startXArea = area.removeFromTop(rowHeight);
        startXLabel.setBounds(startXArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        startXSlider.setBounds(startXArea.withTrimmedRight(rightMargin));
        
        auto startYArea = area.removeFromTop(rowHeight);
        startYLabel.setBounds(startYArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        startYSlider.setBounds(startYArea.withTrimmedRight(rightMargin));
        
        auto startZArea = area.removeFromTop(rowHeight);
        startZLabel.setBounds(startZArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        startZSlider.setBounds(startZArea.withTrimmedRight(rightMargin));
        
        area.removeFromTop(verticalSpacing);
        
        // Target position controls
        auto targetXArea = area.removeFromTop(rowHeight);
        targetXLabel.setBounds(targetXArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        targetXSlider.setBounds(targetXArea.withTrimmedRight(rightMargin));
        
        auto targetYArea = area.removeFromTop(rowHeight);
        targetYLabel.setBounds(targetYArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        targetYSlider.setBounds(targetYArea.withTrimmedRight(rightMargin));
        
        auto targetZArea = area.removeFromTop(rowHeight);
        targetZLabel.setBounds(targetZArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        targetZSlider.setBounds(targetZArea.withTrimmedRight(rightMargin));
    }
    
    void updateStartPositionVisibility()
    {
        bool enabled = useStartPosition.getToggleState();
        
        // Enable/disable start position controls with visual feedback
        startXSlider.setEnabled(enabled);
        startYSlider.setEnabled(enabled);
        startZSlider.setEnabled(enabled);
        startXLabel.setEnabled(enabled);
        startYLabel.setEnabled(enabled);
        startZLabel.setEnabled(enabled);
        
        startXSlider.setAlpha(enabled ? 1.0f : 0.5f);
        startYSlider.setAlpha(enabled ? 1.0f : 0.5f);
        startZSlider.setAlpha(enabled ? 1.0f : 0.5f);
        startXLabel.setAlpha(enabled ? 1.0f : 0.5f);
        startYLabel.setAlpha(enabled ? 1.0f : 0.5f);
        startZLabel.setAlpha(enabled ? 1.0f : 0.5f);
    }
};
