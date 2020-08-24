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
    startTimer(TIMER_ID_UPDATE, 20);
}

void StatusMessageHandler::unregisterDetailLog()
{
    const ScopedLock lock(cs);
    stopTimer(TIMER_ID_UPDATE);
    pTextEditor = nullptr;
}

void StatusMessageHandler::showMessage(String message, String detailMessage, MessageStyle style)
{
    if(pTextEditor != nullptr)
    {
        DetailMessage msg;
        msg.message = Time::getCurrentTime().toString(true, true, true, true) + ": " + detailMessage + "\r\n";
        msg.messageStyle = style;
        
        messageQueue.push(msg);
    }
    
	if (pLabel != nullptr)
	{
        const ScopedLock lock(cs);
        
        if(style == Error)
        {
            stopTimer(TIMER_ID_LABEL_DELAY);
            pLabel->setColour(Label::textColourId, Colours::red);
            pLabel->setColour(Label::backgroundColourId, Colours::yellow);
            errorFlag = true;
            pLabel->setText(message, sendNotification);
            startTimer(TIMER_ID_LABEL_DELAY, 3000);
        }
        else
        {
            if(!errorFlag) // do not overwrite error messages
            {
                stopTimer(TIMER_ID_LABEL_DELAY);
                pLabel->setColour(Label::textColourId, Colours::white);
                pLabel->setColour(Label::backgroundColourId, Colours::green);
                pLabel->setText(message, sendNotification);
                startTimer(TIMER_ID_LABEL_DELAY, 500);
            }
        }
	}
}

void StatusMessageHandler::timerCallback(int timerID)
{
    if(timerID == TIMER_ID_UPDATE)
    {
        stopTimer(TIMER_ID_UPDATE);
        const ScopedLock lock(cs);
     
        if(messageQueue.size() > 10)
        {
            pTextEditor->setColour(TextEditor::textColourId, Colours::red);
            pTextEditor->moveCaretToEnd();
            size_t discardedCount = messageQueue.size();
            std::queue<DetailMessage> empty;
            std::swap( messageQueue, empty );
            pTextEditor->insertTextAtCaret("Too many messages to display, " + String(discardedCount) + " elements discarded");
        }
        else
        {
            while(!messageQueue.empty())
            {
                DetailMessage msg = messageQueue.front();
                messageQueue.pop();
                pTextEditor->setColour(TextEditor::textColourId, msg.messageStyle == Error ? Colours::red : Colours::limegreen);
        
                pTextEditor->moveCaretToEnd();
                pTextEditor->insertTextAtCaret(msg.message);
            }
        }
        
        pTextEditor->setText(pTextEditor->getText().getLastCharacters(1200));
        startTimer(TIMER_ID_UPDATE, 20);
    }
    else if(timerID == TIMER_ID_LABEL_DELAY)
    {
        stopTimer(TIMER_ID_LABEL_DELAY);
        const ScopedLock lock(cs);
        if (pLabel != nullptr)
        {
            pLabel->setColour(Label::textColourId, Colours::green);
            pLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
            pLabel->setText("", dontSendNotification);
            errorFlag = false;
        }
    }
}
