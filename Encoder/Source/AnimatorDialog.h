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

#define ACTION_CLOSE_ANIMATOR	"CloseAnimator"

class AnimatorDialog : public AdditionalWindow, public ActionBroadcaster
{
public:
AnimatorDialog(ActionListener* actionListener, Component* pComponent)
		: AdditionalWindow("Animator", pComponent)
	{
		setAlwaysOnTop(true);
        setContentOwned(pComponent, true);
		addActionListener(actionListener);
        setResizable(true, true);
        setUsingNativeTitleBar(false);
		setResizeLimits(700, 600, 3000, 600);
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_ANIMATOR);
	}
};
