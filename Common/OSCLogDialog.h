/*
  ==============================================================================

    OSCLogDialog.h
    Created: 17 Apr 2021 12:29:15am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AdditionalWindow.h"

#define ACTION_CLOSE_OSCLOG	"CloseOscLog"

class OSCLogDialog : public AdditionalWindow, public ActionBroadcaster
{
public:
	OSCLogDialog(ActionListener* actionListener, Component* pComponent)
		: AdditionalWindow("OSC Log", pComponent)
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
		sendActionMessage(ACTION_CLOSE_OSCLOG);
	}
};
