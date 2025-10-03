#pragma once
#include "JuceHeader.h"
#include "../../Common/AdditionalWindow.h"
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
        setAlwaysOnTop(false); // Allow interaction with parent
        setContentOwned(editorComponent.release(), true);
        addActionListener(actionListener);
        setResizable(false, false);
        setUsingNativeTitleBar(false);
        
        // Use the provided size + title bar height
        const int titleBarHeight = getTitleBarHeight();
        setSize(width, height + titleBarHeight);
        centreWithSize(getWidth(), getHeight());
    }

    void closeButtonPressed() override
    {
        sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
    }
};
