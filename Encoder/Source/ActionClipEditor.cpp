#include "ActionClipEditor.h"
#include "TimelineComponent.h"
#include "CommonClipSettings.h"
#include "ClipEditorDialog.h"
#include "../../Common/UiState.h"

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
    // applyToClip() rather than hand-copying individual fields: this used to pull only length,
    // palindrome and repetitions, so the speed curve never reached the preview and action clips
    // looked as though easing did nothing (it worked in playback, where the applied clip carries
    // it). Pulling the whole base clip the way isDirty() already does also means the next base
    // field added works here automatically instead of needing this list extended again.
    ActionClip clip = currentClip;
    commonSettings.applyToClip(clip);
    preview.setActionClip(clip);
}

void ActionClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    const int buttonHeight = 28;

    // Buttons come off the bottom first, so the three columns share whatever is left and each ends
    // up the same height.
    auto buttonArea = area.removeFromBottom(buttonHeight).reduced(10, 0);
    cancelButton.setBounds(buttonArea.removeFromRight(80));
    buttonArea.removeFromRight(8); // Button spacing
    applyButton.setBounds(buttonArea.removeFromRight(80));

    area.removeFromBottom(8);

    // Landscape: Clip Properties | Preview | Actions, side by side - see MovementClipEditor, which
    // needed this once the speed curve made the clip panel taller.
    auto clipGroupArea = area.removeFromLeft(getClipPropertiesWidth());
    clipGroup.setBounds(clipGroupArea);
    commonSettings.setBounds(clipGroupArea.reduced(8, 20));

    area.removeFromLeft(8); // spacing between columns

    auto previewArea = area.removeFromLeft(getPreviewWidth());
    previewGroup.setBounds(previewArea);
    preview.setBounds(previewArea.reduced(8, 20));

    area.removeFromLeft(8);

    actionsGroup.setBounds(area);
    layoutActionControls(area.reduced(8, 20));
}

void ActionClipEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

int ActionClipEditor::getTotalRequiredHeight() const
{
    const int margins = 10 * 2;
    const int buttonHeight = 28;
    const int rowSpacing = 8; // columns -> buttons

    // As tall as the tallest column, not the clip panel PLUS the actions panel. The + 40 on each is
    // the GroupComponent inset, since resized() hands out each content area already reduced(8, 20).
    const int columnHeight = juce::jmax(commonSettings.getRequiredHeight() + 40,
                                        juce::jmax(getPreviewHeight(),
                                                   getActionsControlsHeight() + 40));

    return margins + columnHeight + rowSpacing + buttonHeight;
}

int ActionClipEditor::getTotalRequiredWidth() const
{
    return 10 * 2 + getClipPropertiesWidth() + 8 + getPreviewWidth() + 8 + getActionsWidth();
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
            // See MovementClipEditor::applyChanges - one step per Apply, only when it changes
            // something.
            if (!(timelineModel->actions.clips.getReference(clipIndex) == currentClip))
                timelineComp.pushUndoStep("Edit Action Clip");

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

    // showYesNoCancelBox returns 1 for the first button, 2 for the second, 0 for the third
    // (or if the box is dismissed) - mapped here to Save & Close / Discard / Keep Editing.
    const int result = juce::AlertWindow::showYesNoCancelBox(juce::AlertWindow::WarningIcon,
        "Unsaved Changes", "This clip has unsaved changes.",
        "Save & Close", "Discard", "Keep Editing");

    if (result == 1) // Save & Close
    {
        if (!applyChanges())
            return false; // validation failed (e.g. invalid Start/Duration/End) - stay open, same as the Apply button would

        timelineComp.repaint();
        return true;
    }

    if (result == 2) // Discard
        return true;

    return false; // Keep Editing, or the box was dismissed
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
    // The curve is dragged continuously, so wait for the 150ms poll and it is impossible to
    // judge what the curve is doing - push it to the preview on every change instead.
    commonSettings.onCurveEdited = [this] { timerCallback(); };
    // No palindrome constraint here (it stays at its default of false, unlike MovementClipEditor's):
    // rotation used to need Palindrome for any repeat because an incremental sweep had no way to undo
    // itself at a cycle boundary. AnimatorMath::rotationPhase now ramps continuously for a bare
    // repeat, so N repeats are N consecutive turns with nothing to undo.

    addAndMakeVisible(preview);
    preview.setScalingInfo(pSourceSet != nullptr ? pSourceSet->getScalingInfo() : nullptr);
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

    // Hand-rolled instead of options.runModal(), purely so the window can be repositioned before it
    // becomes visible, and read back afterwards. runModal() is create() + runModalLoop() with
    // deleteWhenDismissed set, which would delete the window before we could save its position;
    // owning it here keeps it alive across both calls. create() leaves it sized, centred on this
    // editor and hidden, so an unknown position keeps that centring rather than jumping elsewhere.
    std::unique_ptr<juce::DialogWindow> dialog(options.create());
    UiState::restorePosition(*dialog, UiState::Windows::animatorActionEdit, /*centreIfUnknown*/ false);

    const bool accepted = dialog->runModalLoop() != 0;

    UiState::rememberPosition(*dialog, UiState::Windows::animatorActionEdit);
    dialog.reset();

    // Whether accepted or cancelled, the dialog's live edits are gone now - restore the preview
    // to the clip's actual (unedited, or already-committed-by-the-caller) state.
    preview.setActionClip(currentClip);

    return accepted;
}
