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
        if((i+1)*(i+1) <= (isEncoder ? numOutputChannels : numInputChannels))
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
