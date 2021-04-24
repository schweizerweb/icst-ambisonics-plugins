/*
  =============================================================================

    AnimatorDialog.h
    Created: 22 Apr 2021 10:16:51p
    Author:  Schweizer Christian

  =============================================================================
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
		setResizeLimits(700, 300, 3000, 3000);
	}

	void closeButtonPressed() override
	{
		sendActionMessage(ACTION_CLOSE_ANIMATOR);
	}
};
