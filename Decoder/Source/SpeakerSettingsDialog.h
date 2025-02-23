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
#include "../../Common/AdditionalWindow.h"

#define ACTION_CLOSE_SETTINGS	"CloseSettings"

class SpeakerSettingsDialog : public AdditionalWindow, public ActionBroadcaster
{
public:
	SpeakerSettingsDialog(ActionListener* actionListener, Component* pComponent)
		: AdditionalWindow("Speaker settings", pComponent)
	{
		setAlwaysOnTop(false);
		setContentOwned(pComponent, true);
		addActionListener(actionListener);
		setResizable(true, true);
		setUsingNativeTitleBar(false);
        setResizeLimits(800, 500, 10000, 10000);
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_SETTINGS);
	}
};
