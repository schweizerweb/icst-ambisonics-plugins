#pragma once

#include "CommonClipSettings.h"

class ActionClipEditor : public juce::Component, public juce::ListBoxModel
{
public:
    ActionClipEditor(TimelineComponent& timeline, int timelineIndex, int layerIndex, int clipIndex)
        : timelineComp(timeline), timelineIndex(timelineIndex), layerIndex(layerIndex), clipIndex(clipIndex)
    {
        if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
        {
            if (clipIndex >= 0 && clipIndex < timelineModel->actions.clips.size())
            {
                currentClip = timelineModel->actions.clips.getReference(clipIndex);
            }
        }
        
        createControls();
    }
    
    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        
        // Calculate heights
        const int clipGroupHeight = commonSettings.getRequiredHeight() + 30;
        const int actionsGroupHeight = getActionsControlsHeight() + 40;
        const int buttonHeight = 28;
        
        // Clip settings group
        auto clipGroupArea = area.removeFromTop(clipGroupHeight);
        clipGroup.setBounds(clipGroupArea);
        commonSettings.setBounds(clipGroupArea.reduced(8, 20));
        
        area.removeFromTop(8); // Spacing between groups
        
        // Actions group
        auto actionsGroupArea = area.removeFromTop(actionsGroupHeight);
        actionsGroup.setBounds(actionsGroupArea);
        
        auto actionsContentArea = actionsGroupArea.reduced(8, 20);
        layoutActionControls(actionsContentArea);
        
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
        const int actionsGroupHeight = getActionsControlsHeight() + 40;
        const int buttonHeight = 28;
        const int groupSpacing = 8;
        
        return margins + clipGroupHeight + groupSpacing + actionsGroupHeight + buttonHeight;
    }
    
    bool applyChanges()
    {
        if (!commonSettings.validate())
            return false;
            
        commonSettings.applyToClip(currentClip);
        
        if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
        {
            if (clipIndex >= 0 && clipIndex < timelineModel->actions.clips.size())
            {
                timelineModel->actions.clips.getReference(clipIndex) = currentClip;
                return true;
            }
        }
        
        return false;
    }
    
    // ListBoxModel implementation
    int getNumRows() override { return currentClip.actions.size(); }
    
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
                         int width, int height, bool rowIsSelected) override
    {
        if (rowNumber >= currentClip.actions.size()) return;
        
        auto& lf = getLookAndFeel();
        
        if (rowIsSelected)
            g.fillAll(lf.findColour(juce::ListBox::backgroundColourId).brighter(0.3f));
        else
            g.fillAll(lf.findColour(juce::ListBox::backgroundColourId));
            
        g.setColour(lf.findColour(juce::ListBox::textColourId));
        g.setFont(14.0f);
        
        const auto& action = currentClip.actions.getReference(rowNumber);
        juce::String text;
        
        switch (action.action)
        {
            case ActionType::RotationX: text = "Rotate X"; break;
            case ActionType::RotationY: text = "Rotate Y"; break;
            case ActionType::RotationZ: text = "Rotate Z"; break;
            case ActionType::Stretch: text = "Stretch"; break;
            default: text = "Unknown"; break;
        }
        
        switch (action.timing)
        {
            case TimingType::AbsoluteTarget: text += " → "; break;
            case TimingType::RelativeDuringClip: text += " + "; break;
            case TimingType::AbsolutePerSecond: text += " /s "; break;
            default: text += " ? "; break;
        }
        
        text += juce::String(action.value);
        g.drawText(text, 10, 0, width - 10, height, juce::Justification::centredLeft);
    }
    
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override
    {
        editAction(row);
    }
    
private:
    TimelineComponent& timelineComp;
    int timelineIndex, layerIndex, clipIndex;
    ActionClip currentClip;
    
    CommonClipSettings commonSettings;
    
    juce::GroupComponent clipGroup{"Clip", "Clip Properties"};
    juce::GroupComponent actionsGroup{"Actions", "Action Properties"};
    
    juce::TextButton applyButton{"Apply"}, cancelButton{"Cancel"};
    
    juce::ListBox actionsList;
    juce::TextButton addActionButton{"Add"}, removeActionButton{"Remove"};
    
    void createControls()
    {
        setOpaque(true);
        
        addAndMakeVisible(clipGroup);
        addAndMakeVisible(actionsGroup);
        
        addAndMakeVisible(commonSettings);
        commonSettings.setClipData(currentClip);
        
        // Buttons
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
        
        // Action controls
        addAndMakeVisible(actionsList);
        actionsList.setModel(this);
        
        addAndMakeVisible(addActionButton);
        addActionButton.onClick = [this] { addAction(); };
        
        addAndMakeVisible(removeActionButton);
        removeActionButton.onClick = [this] { removeSelectedAction(); };
    }
    
    int getActionsControlsHeight() const
    {
        const int buttonHeight = 28;
        const int verticalSpacing = 8;
        const int listHeight = 120;
        
        return buttonHeight + verticalSpacing + listHeight;
    }
    
    void layoutActionControls(juce::Rectangle<int> area)
    {
        const int buttonHeight = 28;
        const int verticalSpacing = 8;
        const int buttonWidth = 80;
        const int buttonSpacing = 8;
        
        // Buttons at top
        auto buttonArea = area.removeFromTop(buttonHeight);
        addActionButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
        buttonArea.removeFromLeft(buttonSpacing);
        removeActionButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // List takes remaining space
        actionsList.setBounds(area);
    }
    
    void addAction()
    {
        ActionDefinition newAction;
        newAction.action = ActionType::RotationX;
        newAction.timing = TimingType::AbsoluteTarget;
        newAction.value = 0.0;
        
        if (editActionDialog(newAction, "Add New Action"))
        {
            currentClip.actions.add(newAction);
            actionsList.updateContent();
        }
    }
    
    void removeSelectedAction()
    {
        int selected = actionsList.getSelectedRow();
        if (selected >= 0)
        {
            currentClip.actions.remove(selected);
            actionsList.updateContent();
        }
    }
    
    void editAction(int index)
    {
        if (index >= 0 && index < currentClip.actions.size())
        {
            auto action = currentClip.actions.getReference(index);
            
            if (editActionDialog(action, "Edit Action"))
            {
                currentClip.actions.getReference(index) = action;
                actionsList.updateContent();
            }
        }
    }
    
    bool editActionDialog(ActionDefinition& action, const juce::String& title)
    {
        juce::AlertWindow w(title, "", juce::AlertWindow::NoIcon);
        w.addComboBox("type", {"Rotation X", "Rotation Y", "Rotation Z", "Stretch"}, "Type");
        w.addComboBox("timing", {"Absolute Target", "Relative During Clip", "Absolute Per Second"}, "Timing");
        w.addTextEditor("value", juce::String(action.value), "Value");
        
        w.getComboBoxComponent("type")->setSelectedItemIndex((int)action.action - 1);
        w.getComboBoxComponent("timing")->setSelectedItemIndex((int)action.timing - 1);
        
        w.addButton("OK", 1);
        w.addButton("Cancel", 0);
        
        if (w.runModalLoop() == 1)
        {
            action.action = (ActionType)(w.getComboBoxComponent("type")->getSelectedItemIndex() + 1);
            action.timing = (TimingType)(w.getComboBoxComponent("timing")->getSelectedItemIndex() + 1);
            action.value = w.getTextEditorContents("value").getDoubleValue();
            return true;
        }
        
        return false;
    }
};
