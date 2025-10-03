#pragma once

#include "ActionEditDialog.h"
#include "CommonClipSettings.h"

class TimelineComponent;

class ActionClipEditor : public juce::Component, public juce::ListBoxModel
{
public:
    ActionClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx);
    
    void resized() override;
    void paint(juce::Graphics& g) override;
    int getTotalRequiredHeight() const;
    bool applyChanges();
    
    // ListBoxModel implementation
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
                         int width, int height, bool rowIsSelected) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

private:
    TimelineComponent& timelineComp;
    int timelineIndex, clipIndex;
    ActionClip currentClip;
    
    CommonClipSettings commonSettings;
    
    juce::GroupComponent clipGroup{"Clip", "Clip Properties"};
    juce::GroupComponent actionsGroup{"Actions", "Action Properties"};
    
    juce::TextButton applyButton{"Apply"}, cancelButton{"Cancel"};
    
    juce::ListBox actionsList;
    juce::TextButton addActionButton{"Add"}, removeActionButton{"Remove"};
    
    void createControls();
    int getActionsControlsHeight() const;
    void layoutActionControls(juce::Rectangle<int> area);
    void addAction();
    void removeSelectedAction();
    void editAction(int index);
    bool editActionDialog(ActionDefinition& action, const juce::String& title);
};
