/*
  ==============================================================================

    HelpDialog.h
    Created: 30 Mar 2020 8:44:19pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AdditionalWindow.h"

#define ACTION_CLOSE_HELP	"CloseHelp"

class HelpDialog : public AdditionalWindow, public ActionBroadcaster
{
public:
	HelpDialog(ActionListener* actionListener, Component* pComponent)
		: AdditionalWindow("Help", pComponent)
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
		sendActionMessage(ACTION_CLOSE_HELP);
	}
};