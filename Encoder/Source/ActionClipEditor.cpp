#include "ActionClipEditor.h"
#include "TimelineComponent.h"
#include "CommonClipSettings.h"
#include "ClipEditorDialog.h"

ActionClipEditor::ActionClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx)
    : timelineComp(timeline), timelineIndex(timelineIdx), clipIndex(clipIdx)
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

void ActionClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    
    // Calculate heights
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 40;
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

void ActionClipEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

int ActionClipEditor::getTotalRequiredHeight() const
{
    const int margins = 10 * 2;
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 30;
    const int actionsGroupHeight = getActionsControlsHeight() + 40;
    const int buttonHeight = 28;
    const int groupSpacing = 3 * 8;
    
    return margins + clipGroupHeight + groupSpacing + actionsGroupHeight + buttonHeight;
}

bool ActionClipEditor::applyChanges()
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
int ActionClipEditor::getNumRows()
{
    return currentClip.actions.size();
}

void ActionClipEditor::paintListBoxItem(int rowNumber, juce::Graphics& g,
                     int width, int height, bool rowIsSelected)
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
    
    // Use the getDescription method for display
    g.drawText(action.getDescription(), 10, 0, width - 10, height, juce::Justification::centredLeft);
}

void ActionClipEditor::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
    editAction(row);
}

void ActionClipEditor::createControls()
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
            // Send action message to close the window
            if (auto* broadcaster = findParentComponentOfClass<juce::ActionBroadcaster>())
                broadcaster->sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
        }
    };
    
    addAndMakeVisible(cancelButton);
    cancelButton.onClick = [this] {
        // Send action message to close the window
        if (auto* broadcaster = findParentComponentOfClass<juce::ActionBroadcaster>())
            broadcaster->sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
    };
    
    // Action controls
    addAndMakeVisible(actionsList);
    actionsList.setModel(this);
    
    addAndMakeVisible(addActionButton);
    addActionButton.onClick = [this] { addAction(); };
    
    addAndMakeVisible(removeActionButton);
    removeActionButton.onClick = [this] { removeSelectedAction(); };
}

int ActionClipEditor::getActionsControlsHeight() const
{
    const int buttonHeight = 28;
    const int verticalSpacing = 8;
    const int listHeight = 120;
    
    return buttonHeight + verticalSpacing + listHeight;
}

void ActionClipEditor::layoutActionControls(juce::Rectangle<int> area)
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

void ActionClipEditor::addAction()
{
    ActionDefinition newAction;
    newAction.setAction(ActionType::RotationX);
    newAction.setTiming(TimingType::AbsoluteTarget);
    newAction.setValue(0.0);
    
    if (editActionDialog(newAction, "Add New Action"))
    {
        currentClip.actions.add(newAction);
        actionsList.updateContent();
    }
}

void ActionClipEditor::removeSelectedAction()
{
    int selected = actionsList.getSelectedRow();
    if (selected >= 0)
    {
        currentClip.actions.remove(selected);
        actionsList.updateContent();
    }
}

void ActionClipEditor::editAction(int index)
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

bool ActionClipEditor::editActionDialog(ActionDefinition& action, const juce::String& title)
{
    // Create the dialog component
    auto* dialogComponent = new ActionEditDialog(action, title);
    
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(dialogComponent);
    options.dialogTitle = title;
    options.componentToCentreAround = this;
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = false;
    
    return options.runModal() != 0;
}
