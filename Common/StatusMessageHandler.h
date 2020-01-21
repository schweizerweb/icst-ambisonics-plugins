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
    
    void registerDetailLog(TextEditor* editor);
    void unregisterDetailLog();
    
	void showMessage(String message, String detailMessage, MessageStyle style);
	
private:
	void timerCallback() override;
	Label* pLabel;
    TextEditor* pTextEditor;
	CriticalSection cs;
};
