/*
  ==============================================================================

    StatusMessageHandler.cpp
    Created: 27 Sep 2018 9:46:35am
    Author:  Christian Schweizer

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

void StatusMessageHandler::registerDetailLog(TextEditor *editor)
{
    const ScopedLock lock(cs);
    pTextEditor = editor;
}

void StatusMessageHandler::unregisterDetailLog()
{
    const ScopedLock lock(cs);
    pTextEditor = nullptr;
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
    
    if(pTextEditor != nullptr)
    {
        const ScopedLock lock(cs);
        pTextEditor->insertTextAtCaret(message);
        pTextEditor->insertTextAtCaret("\r\n");
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
