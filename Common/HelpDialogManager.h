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
#include "HelpDialog.h"
#include "HelpComponent.h"

class HelpDialogManager : ActionListener
{
public:
    HelpDialogManager()
    {
        helpWindow = nullptr;
    }
    
    ~HelpDialogManager() override
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
