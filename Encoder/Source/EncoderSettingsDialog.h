/*
  ==============================================================================

    EncoderSettingsDialog.h
    Created: 1 Nov 2019 12:42:17pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AdditionalWindow.h"
#define ACTION_CLOSE_SETTINGS	"CloseSettings"

class EncoderSettingsDialog : public ActionBroadcaster, public AdditionalWindow
{
public:
	EncoderSettingsDialog(ActionListener* actionListener, Component* pComponent)
		: AdditionalWindow("Encoder  Settings", pComponent)
	{
        setAlwaysOnTop(true);
        setContentOwned(pComponent, true);
		addActionListener(actionListener);
        setResizable(true, true);
		setUsingNativeTitleBar(false);
		setResizeLimits(700, 650, 3000, 3000);
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_SETTINGS);
	}
};
