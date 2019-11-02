/*
  ==============================================================================

    EncoderSettingsDialog.h
    Created: 1 Nov 2019 12:42:17pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define ACTION_CLOSE_SETTINGS	"CloseSettings"

class EncoderSettingsDialog : public DialogWindow, public ActionBroadcaster
{
public:
	EncoderSettingsDialog(ActionListener* actionListener, Component* pComponent, bool multiEncoderMode)
		: DialogWindow("Encoder settings", Colours::white, false, true)
	{
		setAlwaysOnTop(true);
		setContentOwned(pComponent, true);
		addActionListener(actionListener);
		setUsingNativeTitleBar(false);
		setResizable(true, true);
		if(multiEncoderMode)
		{
			setResizeLimits(600, 400, 3000, 2000);
		}
		else
		{
			setResizeLimits(600, 300, 3000, 200);
		}
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_SETTINGS);
	}
};
