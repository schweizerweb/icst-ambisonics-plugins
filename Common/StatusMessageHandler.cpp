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
    pTextEditor = nullptr;
    errorFlag = false;
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
    if(pTextEditor != nullptr)
    {
        pTextEditor->setColour(TextEditor::textColourId, style == Error ? Colours::red : Colours::limegreen);
        
        const ScopedLock lock(cs);
        pTextEditor->moveCaretToEnd();
        pTextEditor->insertTextAtCaret(Time::getCurrentTime().toString(true, true, true, true) + ": " + detailMessage + "\r\n");
    }
    
	if (pLabel != nullptr)
	{
        const ScopedLock lock(cs);
        
        if(style == Error)
        {
            stopTimer();
            pLabel->setColour(Label::textColourId, Colours::red);
            pLabel->setColour(Label::backgroundColourId, Colours::yellow);
            errorFlag = true;
            pLabel->setText(message, sendNotification);
            startTimer(3000);
        }
        else
        {
            if(!errorFlag) // do not overwrite error messages
            {
                stopTimer();
                pLabel->setColour(Label::textColourId, Colours::white);
                pLabel->setColour(Label::backgroundColourId, Colours::green);
                pLabel->setText(message, sendNotification);
                startTimer(500);
            }
        }
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
        errorFlag = false;
	}
}
