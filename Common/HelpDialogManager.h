/*
  ==============================================================================

    HelpDialogManager.h
    Created: 1 Apr 2020 8:22:24am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "HelpDialog.h"
#include "HelpComponent.h"

class HelpDialogManager : ActionListener
{
public:
    HelpDialogManager()
    {
        helpWindow = nullptr;
    }
    
    ~HelpDialogManager()
    {
        if(helpWindow != nullptr)
        {
            delete helpWindow;
        }
    }
    
    void actionListenerCallback(const juce::String &message) override
    {
        if(message == ACTION_CLOSE_HELP)
        {
            delete helpWindow;
            helpWindow = nullptr;
        }
    }
    
    void show(bool isEncoder, Component* pParent)
    {
        if(helpWindow)
            delete helpWindow;
        helpWindow = new HelpDialog(this, new HelpComponent(isEncoder));
        helpWindow->setVisible(true);
        helpWindow->updatePosition(pParent->getScreenBounds());
    }
private:
    HelpDialog* helpWindow;
};
