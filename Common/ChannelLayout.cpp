/*
  ==============================================================================

    ChannelLayout.cpp
    Created: 27 Jul 2023 11:08:28pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "ChannelLayout.h"
ChannelLayout::ChannelLayout()
{
    numInputChannels = 0;
    numOutputChannels = 0;
}

int ChannelLayout::getNumInputChannels() const
{
    return numInputChannels;
}

int ChannelLayout::getNumOutputChannels() const
{
    return numOutputChannels;
}

int ChannelLayout::getMaxAmbiOrder(bool isEncoder) const
{
    int maxAmbiOrder = 0;
    for(int i = 1; i <= 7; i++)
    {
        if((i+1)*(i+1) <= numOutputChannels)
            maxAmbiOrder = i;
    }
    return maxAmbiOrder;
}

void ChannelLayout::setNumInputChannels(int count)
{
    bool changeFlag = (numInputChannels != count);
    
    numInputChannels = count;
    
    if(changeFlag)
    {
        sendActionMessage(ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED);
    }
}

void ChannelLayout::setNumOutputChannels(int count)
{
    bool changeFlag = (numOutputChannels != count);
    
    numOutputChannels = count;
    
    if(changeFlag)
    {
        sendActionMessage(ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED);
    }
}

void ChannelLayout::setNumChannels(int countIn, int countOut)
{
    bool changeFlag = (numInputChannels != countIn) || (numOutputChannels != countOut);
    
    numInputChannels = countIn;
    numOutputChannels = countOut;
    
    if(changeFlag)
    {
        sendActionMessage(ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED);
    }
}
