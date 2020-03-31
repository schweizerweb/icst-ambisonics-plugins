/*
  ==============================================================================

    HelpDialog.h
    Created: 30 Mar 2020 8:44:19pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define ACTION_CLOSE_HELP	"CloseHelp"

class HelpDialog : public DialogWindow, public ActionBroadcaster
{
public:
	HelpDialog(ActionListener* actionListener, Component* pComponent)
		: DialogWindow("Help", Colours::white, false, true)
	{
		setContentOwned(pComponent, true);
		addActionListener(actionListener);
		setUsingNativeTitleBar(false);
		setResizable(true, true);

		setResizeLimits(200, 200, 3000, 3000);
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_HELP);
	}
};
