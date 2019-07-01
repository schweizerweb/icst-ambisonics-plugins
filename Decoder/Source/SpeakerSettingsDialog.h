/*
  ==============================================================================

    SpeakerSettingsDialog.h
    Created: 1 Jul 2019 7:27:06pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#define ACTION_CLOSE_SETTINGS	"CloseSettings"

class SpeakerSettingsDialog : public DialogWindow, public ActionBroadcaster
{
public:
	SpeakerSettingsDialog(ActionListener* actionListener, Component* pComponent)
		: DialogWindow("Speaker settings", Colours::white, false, true)
	{
		setAlwaysOnTop(true);
		setContentOwned(pComponent, true);
		addActionListener(actionListener);
		setResizable(false, false);
		setUsingNativeTitleBar(false);
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_SETTINGS);
	}
};
