#pragma once
#include "JuceHeader.h"
#include "../../Common/AdditionalWindow.h"
#include "../../Common/UiState.h"
#include "ClipEditorCloseGuard.h"
#include "MovementClipEditor.h"
#include "ActionClipEditor.h"

class TimelineComponent;

#define ACTION_CLOSE_CLIP_EDITOR "CloseClipEditor"

class ClipEditorDialog : public AdditionalWindow, public juce::ActionBroadcaster
{
public:
    ClipEditorDialog(juce::ActionListener* actionListener, const juce::String& title, std::unique_ptr<juce::Component> editorComponent, int width, int height)
        : AdditionalWindow(title, editorComponent.get())
    {
        setAlwaysOnTop(true); // Allow interaction with parent
        setContentOwned(editorComponent.release(), true);
        addActionListener(actionListener);
        setResizable(false, false);
        setUsingNativeTitleBar(false);
        
        // Use the provided size + title bar height
        const int titleBarHeight = getTitleBarHeight();
        setSize(width, height + titleBarHeight);

        // Position only - the size comes from the hosted editor, which also resizes itself later
        // (MovementClipEditor does, per movement type). Movement and action editors deliberately
        // share one key: it's "where the user keeps the clip editor", not a per-type preference.
        UiState::restorePosition(*this, UiState::Windows::animatorClipEditor);
    }

    ~ClipEditorDialog() override
    {
        UiState::rememberPosition(*this, UiState::Windows::animatorClipEditor);
    }

    void closeButtonPressed() override
    {
        // Same dirty check the Cancel button now runs (both are "discard without applying" paths) -
        // getContentComponent() is the hosted MovementClipEditor/ActionClipEditor, both of which
        // implement ClipEditorCloseGuard.
        if (auto* guard = dynamic_cast<ClipEditorCloseGuard*>(getContentComponent()))
            if (!guard->confirmDiscardIfDirty())
                return;

        sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
    }
};
