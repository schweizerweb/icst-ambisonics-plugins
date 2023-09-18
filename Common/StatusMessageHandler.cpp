/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
