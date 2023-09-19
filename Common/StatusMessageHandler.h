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
