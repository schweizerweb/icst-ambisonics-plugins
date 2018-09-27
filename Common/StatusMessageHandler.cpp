/*
  ==============================================================================

    StatusMessageHandler.cpp
    Created: 27 Sep 2018 9:46:35am
    Author:  chris

  ==============================================================================
*/

#include "StatusMessageHandler.h"

StatusMessageHandler::StatusMessageHandler()
{
	pLabel = nullptr;
}

void StatusMessageHandler::registerLabel(Label* label)
{
	const ScopedLock lock(cs);
	pLabel = label;
}

void StatusMessageHandler::unregisterLabel()
{
	const ScopedLock lock(cs);
	pLabel = nullptr;
}

void StatusMessageHandler::showMessage(String message, MessageStyle style)
{
	if (pLabel != nullptr)
	{
		int timeout;
		stopTimer();

		const ScopedLock lock(cs);
		switch (style)
		{
		case Error:
			pLabel->setColour(Label::textColourId, Colours::red);
			pLabel->setColour(Label::backgroundColourId, Colours::yellow);
			timeout = 3000;
			break;
		default:
			pLabel->setColour(Label::textColourId, Colours::white);
			pLabel->setColour(Label::backgroundColourId, Colours::green);
			timeout = 500;
		}

		pLabel->setText(message, sendNotification);
		startTimer(timeout);
	}
}

void StatusMessageHandler::timerCallback()
{
	stopTimer();
	const ScopedLock lock(cs);
	if (pLabel != nullptr)
	{
		pLabel->setColour(Label::textColourId, Colours::green);
		pLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
		pLabel->setText("", dontSendNotification);
	}
}
