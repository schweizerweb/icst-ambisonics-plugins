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
	EncoderSettingsDialog(ActionListener* actionListener, Component* pComponent)
		: DialogWindow("Encoder settings", Colours::white, false, true)
	{
		setAlwaysOnTop(true);
		setContentOwned(pComponent, true);
		addActionListener(actionListener);
		setUsingNativeTitleBar(false);
		setResizable(true, true);
#if MULTI_ENCODER_MODE
		setResizeLimits(600, 750, 3000, 3000);
#else
		setResizeLimits(600, 280, 3000, 280);
#endif
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_SETTINGS);
	}
};
