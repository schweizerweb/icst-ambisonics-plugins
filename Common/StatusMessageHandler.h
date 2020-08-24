/*
  ==============================================================================

    StatusMessageHandler.h
    Created: 27 Sep 2018 9:46:35am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define TIMER_ID_LABEL_DELAY    1
#define TIMER_ID_UPDATE         2

class StatusMessageHandler : public MultiTimer
{
public:
	enum MessageStyle { Error, Success };
	
	StatusMessageHandler();
	void registerLabel(Label* label);
	void unregisterLabel();
    
    void registerDetailLog(TextEditor* editor);
    void unregisterDetailLog();
    
	void showMessage(String message, String detailMessage, MessageStyle style);
    
private:
    struct DetailMessage
    {
        String message;
        MessageStyle messageStyle;
    };
    
	Label* pLabel;
    TextEditor* pTextEditor;
	CriticalSection cs;
    bool errorFlag;
    void timerCallback(int timerID) override;
    std::queue<DetailMessage> messageQueue;
};
