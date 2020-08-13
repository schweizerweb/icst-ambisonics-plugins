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
#if MULTI_ENCODER_MODE
		setResizeLimits(600, 600, 3000, 3000);
#else
		setResizeLimits(600, 450, 3000, 3000);
#endif
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_SETTINGS);
	}
};
