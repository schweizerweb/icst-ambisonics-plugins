/*
  ==============================================================================

    StatusMessageHandler.h
    Created: 27 Sep 2018 9:46:35am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class StatusMessageHandler : public Timer
{
public:
	enum MessageStyle { Error, Success };
	
	StatusMessageHandler();
	void registerLabel(Label* label);
	void unregisterLabel();
	void showMessage(String message, MessageStyle style);
	
private:
	void timerCallback() override;
	Label* pLabel;
	CriticalSection cs;
};
