#pragma once
#include "ClipEditorDialog.h"

class TimelineComponent;

class ClipEditorDialogManager : public juce::ActionListener
{
public:
    ClipEditorDialogManager()
    {
        window = nullptr;
    }
    
    ~ClipEditorDialogManager() override
    {
        if (window != nullptr)
        {
            delete window;
        }
    }
    
    void closeAllWindows()
    {
        if (window != nullptr)
        {
            delete window;
            window = nullptr;
        }
    }
    
    void actionListenerCallback(const juce::String &message) override
    {
        if (message == ACTION_CLOSE_CLIP_EDITOR)
        {
            delete window;
            window = nullptr;
        }
    }
    
    void showMovementEditor(TimelineComponent* timelineComp, int timelineIndex, int clipIndex, juce::Component* pParent)
    {
        if (window)
            delete window;
            
        auto editor = std::make_unique<MovementClipEditor>(*timelineComp, timelineIndex, clipIndex);
        int height = editor->getTotalRequiredHeight();
        editor->setSize(450, height);
        
        window = new ClipEditorDialog(this, "Edit Movement Clip", std::move(editor), 450, height);
        window->setVisible(true);
    }
    
    void showActionEditor(TimelineComponent* timelineComp, int timelineIndex, int clipIndex, juce::Component* pParent)
    {
        if (window)
            delete window;
            
        auto editor = std::make_unique<ActionClipEditor>(*timelineComp, timelineIndex, clipIndex);
        int height = editor->getTotalRequiredHeight();
        editor->setSize(450, height);
        
        window = new ClipEditorDialog(this, "Edit Action Clip", std::move(editor), 450, height);
        window->setVisible(true);
    }

private:
    ClipEditorDialog* window;
};
