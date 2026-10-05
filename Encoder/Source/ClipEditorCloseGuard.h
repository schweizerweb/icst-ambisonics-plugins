#pragma once

// Implemented by clip editor components (MovementClipEditor, ActionClipEditor) hosted inside a
// ClipEditorDialog, so both the dialog's own close ("X") button and its Cancel button can check
// for unsaved edits before discarding them. Previously both paths called
// ActionBroadcaster::sendActionMessage(ACTION_CLOSE_CLIP_EDITOR) directly with no dirty check at
// all - this is a tiny standalone interface (rather than living in ClipEditorDialog.h, which the
// editors would then need to include) specifically to avoid a circular include, since
// ClipEditorDialog.h already includes MovementClipEditor.h/ActionClipEditor.h.
class ClipEditorCloseGuard
{
public:
    virtual ~ClipEditorCloseGuard() = default;

    // Returns true if it's OK to close right now (the clip has no unsaved edits, or the user
    // confirmed discarding them via a blocking "Discard changes?" prompt) - false if the user chose
    // to keep editing, in which case the caller must NOT proceed with closing.
    virtual bool confirmDiscardIfDirty() = 0;
};
