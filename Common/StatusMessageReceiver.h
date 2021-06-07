/*
  ==============================================================================

    StatusMessageReceiver.h
    Created: 17 Apr 2021 4:23:43pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "StatusMessage.h"

class StatusMessageReceiver
{
public:
    virtual ~StatusMessageReceiver() {};
    virtual void notifyOverflow(int discardedCount) = 0;
    virtual void notify(StatusMessage msg) = 0;
};
