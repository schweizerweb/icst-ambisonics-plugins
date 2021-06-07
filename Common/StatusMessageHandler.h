/*
  ==============================================================================

    StatusMessageHandler.h
    Created: 27 Sep 2018 9:46:35am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "StatusMessageReceiver.h"

#define TIMER_ID_LABEL_DELAY    1
#define TIMER_ID_UPDATE         2

class StatusMessageHandler : public MultiTimer
{
public:
	StatusMessageHandler();
	void registerLabel(Label* label);
	void unregisterLabel();
    
    void registerDetailLog(StatusMessageReceiver* pMessageReceiver);
    void unregisterDetailLog(StatusMessageReceiver* pMessageReceiver);
    
	void showMessage(String message, String detailMessage, StatusMessage::MessageStyle style);
    
private:
	Label* pLabel;
	CriticalSection cs;
    bool errorFlag;
    void timerCallback(int timerID) override;
    std::queue<StatusMessage> messageQueue;
    Array<StatusMessageReceiver*> messageReceivers;
};
