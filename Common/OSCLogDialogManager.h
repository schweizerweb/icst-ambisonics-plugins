/*
  ==============================================================================

    OSCLogDialogManager.h
    Created: 17 Apr 2021 12:34:05am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
/*
  ==============================================================================

    HelpDialogManager.h
    Created: 1 Apr 2020 8:22:24am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "OSCLogDialog.h"
#include "OSCLogComponent.h"

class OSCLogDialogManager : ActionListener
{
public:
    OSCLogDialogManager()
    {
        window = nullptr;
    }
    
    ~OSCLogDialogManager()
    {
        if(window != nullptr)
        {
            delete window;
        }
    }
    
    void actionListenerCallback(const juce::String &message) override
    {
        if(message == ACTION_CLOSE_OSCLOG)
        {
            delete window;
            window = nullptr;
        }
    }
    
    void show(StatusMessageHandler* pStatusHandler, Component* pParent)
    {
        if(window)
            delete window;
        window = new OSCLogDialog(this, new OSCLogComponent(pStatusHandler));
        window->setVisible(true);
        window->updatePosition(pParent->getScreenBounds());
    }
private:
    OSCLogDialog* window;
};
