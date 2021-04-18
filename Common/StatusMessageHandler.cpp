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

void StatusMessageHandler::registerDetailLog(StatusMessageReceiver* pMessageReceiver)
{
    const ScopedLock lock(cs);
    messageReceivers.addIfNotAlreadyThere(pMessageReceiver);
    startTimer(TIMER_ID_UPDATE, 20);
}

void StatusMessageHandler::unregisterDetailLog(StatusMessageReceiver* pMessageReceiver)
{
    const ScopedLock lock(cs);
    messageReceivers.removeAllInstancesOf(pMessageReceiver);
    if(messageReceivers.size() == 0)
    {
        stopTimer(TIMER_ID_UPDATE);
    }
}

void StatusMessageHandler::showMessage(String message, String detailMessage, StatusMessage::MessageStyle style)
{
    if(messageReceivers.size() > 0)
    {
        StatusMessage msg;
        msg.timestamp = Time::getCurrentTime();
        msg.message = detailMessage;
        msg.messageStyle = style;
        
        messageQueue.push(msg);
    }
    
	if (pLabel != nullptr)
	{
        const ScopedLock lock(cs);
        
        if(style == StatusMessage::Error)
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
            size_t discardedCount = messageQueue.size();
            std::queue<StatusMessage> empty;
            std::swap( messageQueue, empty );
            
            for(auto r : messageReceivers)
                r->notifyOverflow((int)discardedCount);
        }
        else
        {
            while(!messageQueue.empty())
            {
                StatusMessage msg = messageQueue.front();
                messageQueue.pop();
                for(auto r : messageReceivers)
                    r->notify(msg);
            }
        }
        
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
