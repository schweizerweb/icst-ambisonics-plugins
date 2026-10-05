#pragma once

#include "ActionEditDialog.h"
#include "CommonClipSettings.h"
#include "ClipPreviewComponent.h"
#include "ClipEditorCloseGuard.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"

class TimelineComponent;

class ActionClipEditor : public juce::Component, public juce::ListBoxModel,
                         public juce::ChangeListener, public ClipEditorCloseGuard, private juce::Timer
{
public:
    ActionClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx);
    ~ActionClipEditor() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    int getTotalRequiredHeight() const;
    int getTotalRequiredWidth() const;
    bool applyChanges();

    // True if the live UI state (currentClip.actions, already kept live by add/remove/edit, plus
    // CommonClipSettings' fields, pulled live the same way the preview's own timerCallback() does)
    // differs from originalClip, the pristine snapshot captured at construction.
    bool isDirty();
    bool confirmDiscardIfDirty() override;

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    TimelineComponent& timelineComp;
    int timelineIndex, clipIndex;
    ActionClip currentClip;
    // Pristine snapshot of currentClip as loaded at construction, before any edits - unlike
    // MovementClipEditor (whose currentClip stays untouched until Apply), this editor's
    // currentClip.actions is mutated live by addAction()/removeSelectedAction()/editAction(), so a
    // separate copy is needed to detect unsaved changes.
    ActionClip originalClip;

    AmbiSourceSet* pSourceSet = nullptr;
    PointSelection* pPointSelection = nullptr;

    CommonClipSettings commonSettings;
    ClipPreviewComponent preview;

    GroupComponent clipGroup{"Clip", "Clip Properties"};
    GroupComponent actionsGroup{"Actions", "Action Properties"};
    GroupComponent previewGroup{"Preview", "Preview"};
    TextButton applyButton{"Apply"}, cancelButton{"Cancel"};

    juce::ListBox actionsList;
    juce::TextButton addActionButton{"Add"}, removeActionButton{"Remove"};

    void createControls();
    int getActionsControlsHeight() const;
    // Preview sits beside (not below) the Clip Properties group, in portrait orientation (two
    // square panels stacked) - narrower but taller than the old side-by-side layout.
    int getClipPropertiesWidth() const { return 280; }
    int getPreviewWidth() const { return 190; }
    int getPreviewHeight() const { return 320; }
    void layoutActionControls(juce::Rectangle<int> area);
    void addAction();
    void removeSelectedAction();
    void editAction(int index);
    // editingIndex is the position in currentClip.actions being edited, or -1 while adding a
    // brand-new action not yet in the list - used to build a correct live preview (the in-progress
    // edit merged into the rest of the clip) before OK is pressed.
    bool editActionDialog(ActionDefinition& action, const juce::String& title, int editingIndex);
    void updateReferenceFromGroup();
    // Whether currentClip.actions contains a Rotation action - drives
    // commonSettings.setRotationConstraintActive(), since only ActionClipEditor (not
    // CommonClipSettings itself) knows what's in the clip.
    bool currentClipHasRotation() const;

    // Clip editors have no per-control change notification, so the preview is kept live by
    // polling the Duration field instead - currentClip.actions is already kept live by
    // addAction()/removeSelectedAction()/editAction() themselves.
    void timerCallback() override;
};
