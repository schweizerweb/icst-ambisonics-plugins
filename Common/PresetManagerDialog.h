/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
        setResizeLimits(700, 200, 3000, 3000);
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
    
    ~PresetManagerDialog() override
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
