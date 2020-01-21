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

void StatusMessageHandler::showMessage(String message, String detailMessage, MessageStyle style)
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
        pTextEditor->setColour(TextEditor::textColourId, style == Error ? Colours::red : Colours::limegreen);
        
        const ScopedLock lock(cs);
        pTextEditor->moveCaretToEnd();
        pTextEditor->insertTextAtCaret(Time::getCurrentTime().toString(true, true, true, true) + ": " + detailMessage + "\r\n");
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
