/*
  ==============================================================================

    AnimatorDialogManager.h
    Created: 22 Apr 2021 10:19:00pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "AnimatorDialog.h"
#include "AnimatorComponent.h"

class AnimatorDialogManager : ActionListener
{
public:
    AnimatorDialogManager()
    {
        window = nullptr;
    }
    
    ~AnimatorDialogManager()
    {
        if(window != nullptr)
        {
            delete window;
        }
    }
    
    void actionListenerCallback(const juce::String &message) override
    {
        if(message == ACTION_CLOSE_ANIMATOR)
        {
            delete window;
            window = nullptr;
        }
    }
    
    void show(AmbiSourceSet* pSourceSet, AnimatorDataset* pAnimatorDataset, Component* pParent)
    {
        if(window)
            delete window;
        window = new AnimatorDialog(this, new AnimatorComponent(pSourceSet, pAnimatorDataset));
        window->setVisible(true);
        window->updatePosition(pParent->getScreenBounds());
    }
private:
    AnimatorDialog* window;
};
