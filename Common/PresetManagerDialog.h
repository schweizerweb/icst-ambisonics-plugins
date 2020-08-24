/*
  =============================================================================

    PresetManagerDialog.
    Created: 30 Apr 2020 8:17:36a
    Author:  Schweizer Christia

  =============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AdditionalWindow.h"
#include "PresetManagerComponent.h"

#define ACTION_CLOSE_PRESET_MANAGER	"ClosePresetManager"


class PresetManagerWindow : public AdditionalWindow, public ActionBroadcaster
{
public:
    PresetManagerWindow(ActionListener* actionListener, Component* pComponent): AdditionalWindow("Preset Manager", pComponent)
    {
        setAlwaysOnTop(true);
        setContentOwned(pComponent, true);
        addActionListener(actionListener);
        setResizable(true, true);
        setUsingNativeTitleBar(false);
        setResizeLimits(200, 200, 3000, 3000);
    }
    	void closeButtonPressed() override
    {
        sendActionMessage(ACTION_CLOSE_PRESET_MANAGER);
    }
};

class PresetManagerDialog : ActionListener
{
public:
    PresetManagerDialog()
    {
        window = nullptr;
    }
    
    ~PresetManagerDialog()
    {
        if(window != nullptr)
        {
            delete window;
        }
    }
    
    void actionListenerCallback(const juce::String &message) override
    {
        if(message == ACTION_CLOSE_PRESET_MANAGER)
        {
            delete window;
            window = nullptr;
        }
    }
    
    void show(Component* pParent, PresetHelper* pPresetHelper, bool showApplyButton = true)
    {
        if(window)
            delete window;
        window = new PresetManagerWindow(this, new PresetManagerComponent(pPresetHelper, showApplyButton));
        window->setVisible(true);
        window->updatePosition(pParent->getScreenBounds());
    }
    
private:
    PresetManagerWindow* window;
};

