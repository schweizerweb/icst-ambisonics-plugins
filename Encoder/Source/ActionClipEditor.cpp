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

    originalClip = currentClip;

    pSourceSet = timelineComp.getSources();
    pPointSelection = timelineComp.getPointSelection();
    if (pPointSelection != nullptr)
        pPointSelection->addChangeListener(this);

    createControls();
    startTimer(150);
}

ActionClipEditor::~ActionClipEditor()
{
    stopTimer();

    if (pPointSelection != nullptr)
        pPointSelection->removeChangeListener(this);
}

void ActionClipEditor::changeListenerCallback(juce::ChangeBroadcaster* /*source*/)
{
    updateReferenceFromGroup();
}

void ActionClipEditor::updateReferenceFromGroup()
{
    if (pSourceSet == nullptr) return;

    if (auto* group = pSourceSet->getGroup(timelineIndex))
    {
        preview.setReferencePosition(group->getVector3D());
        preview.setReferenceStretch(group->getStretch(), true);
    }
}

void ActionClipEditor::timerCallback()
{
    ActionClip clip = currentClip;
    clip.length = commonSettings.getLiveLength();
    clip.palindrome = commonSettings.getLivePalindrome();
    clip.repetitions = commonSettings.getLiveRepeatCount();
    preview.setActionClip(clip);
}

bool ActionClipEditor::currentClipHasRotation() const
{
    for (const auto& actionDef : currentClip.actions)
        if (actionDef.getAction() == ActionType::RotationX ||
            actionDef.getAction() == ActionType::RotationY ||
            actionDef.getAction() == ActionType::RotationZ)
            return true;
    return false;
}

void ActionClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    // Calculate heights
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 40;
    const int previewGroupHeight = getPreviewHeight();
    const int topRowHeight = juce::jmax(clipGroupHeight, previewGroupHeight);
    const int actionsGroupHeight = getActionsControlsHeight() + 40;
    const int buttonHeight = 28;

    // Clip properties and preview side by side, sharing one row
    auto topRowArea = area.removeFromTop(topRowHeight);

    auto clipGroupArea = topRowArea.removeFromLeft(getClipPropertiesWidth());
    clipGroup.setBounds(clipGroupArea);
    commonSettings.setBounds(clipGroupArea.reduced(8, 20));

    topRowArea.removeFromLeft(8); // spacing between the two top-row groups

    previewGroup.setBounds(topRowArea);
    preview.setBounds(topRowArea.reduced(8, 20));

    area.removeFromTop(8); // Spacing between rows

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
    const int previewGroupHeight = getPreviewHeight();
    const int topRowHeight = juce::jmax(clipGroupHeight, previewGroupHeight);
    const int actionsGroupHeight = getActionsControlsHeight() + 40;
    const int buttonHeight = 28;
    const int rowSpacing = 2 * 8; // top row -> actions group, actions group -> buttons

    return margins + topRowHeight + rowSpacing + actionsGroupHeight + buttonHeight;
}

int ActionClipEditor::getTotalRequiredWidth() const
{
    // Clip properties and the (now portrait) preview share one row - floored at 450 so this never
    // shrinks the dialog relative to its previous fixed width.
    return juce::jmax(450, getClipPropertiesWidth() + 8 + getPreviewWidth() + 20);
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

bool ActionClipEditor::isDirty()
{
    // currentClip.actions is already kept live by addAction()/removeSelectedAction()/editAction(),
    // but its base Clip fields (length/palindrome/repetitions/etc.) only get pulled from
    // CommonClipSettings' controls on Apply - applyToClip() onto a scratch copy pulls them live
    // for comparison here too, the same way timerCallback() already does for the live preview.
    ActionClip liveClip = currentClip;
    commonSettings.applyToClip(liveClip);
    return liveClip != originalClip;
}

bool ActionClipEditor::confirmDiscardIfDirty()
{
    if (!isDirty())
        return true;

    return juce::AlertWindow::showOkCancelBox(juce::AlertWindow::WarningIcon,
        "Discard Changes?", "This clip has unsaved changes. Discard them?",
        "Discard", "Keep Editing");
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
    addAndMakeVisible(previewGroup);

    addAndMakeVisible(commonSettings);
    commonSettings.setDisplayInSeconds(timelineComp.isDisplayTimeInSeconds());
    commonSettings.setClipData(currentClip);
    commonSettings.setRotationConstraintActive(currentClipHasRotation());

    addAndMakeVisible(preview);
    preview.setActionClip(currentClip);
    updateReferenceFromGroup();
    
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
        if (!confirmDiscardIfDirty())
            return;

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
    
    if (editActionDialog(newAction, "Add New Action", -1))
    {
        currentClip.actions.add(newAction);
        actionsList.updateContent();
        preview.setActionClip(currentClip);
        commonSettings.setRotationConstraintActive(currentClipHasRotation());
    }
}

void ActionClipEditor::removeSelectedAction()
{
    int selected = actionsList.getSelectedRow();
    if (selected >= 0)
    {
        currentClip.actions.remove(selected);
        actionsList.updateContent();
        preview.setActionClip(currentClip);
        commonSettings.setRotationConstraintActive(currentClipHasRotation());
    }
}

void ActionClipEditor::editAction(int index)
{
    if (index >= 0 && index < currentClip.actions.size())
    {
        auto action = currentClip.actions.getReference(index);

        if (editActionDialog(action, "Edit Action", index))
        {
            currentClip.actions.getReference(index) = action;
            actionsList.updateContent();
            preview.setActionClip(currentClip);
            commonSettings.setRotationConstraintActive(currentClipHasRotation());
        }
    }
}

bool ActionClipEditor::editActionDialog(ActionDefinition& action, const juce::String& title, int editingIndex)
{
    // Create the dialog component
    auto* dialogComponent = new ActionEditDialog(action, title);

    // Keep the preview live while the dialog is open: merge the in-progress edit into a copy of
    // the real clip (rather than previewing the single action in isolation) so other actions'
    // combined effect - e.g. an existing Jitter while editing a Rotation - keeps showing too.
    dialogComponent->onLiveChange = [this, &action, editingIndex]
    {
        ActionClip previewClip = currentClip;
        if (editingIndex >= 0 && editingIndex < previewClip.actions.size())
            previewClip.actions.getReference(editingIndex) = action;
        else
            previewClip.actions.add(action);
        previewClip.length = commonSettings.getLiveLength();
        preview.setActionClip(previewClip);
    };
    dialogComponent->onLiveChange();

    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(dialogComponent);
    options.dialogTitle = title;
    options.componentToCentreAround = this;
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = false;

    const bool accepted = options.runModal() != 0;

    // Whether accepted or cancelled, the dialog's live edits are gone now - restore the preview
    // to the clip's actual (unedited, or already-committed-by-the-caller) state.
    preview.setActionClip(currentClip);

    return accepted;
}
