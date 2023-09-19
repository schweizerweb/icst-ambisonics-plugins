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

#define ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED "layout_changed"

class ChannelLayout: public ActionBroadcaster
{
public:
    ChannelLayout();
    int getNumInputChannels() const;
    int getNumOutputChannels() const;
    int getMaxAmbiOrder(bool isEncoder) const;
    void setNumInputChannels(int count);
    void setNumOutputChannels(int count);
    void setNumChannels(int countIn, int countOut);
    
private:
    int numInputChannels;
    int numOutputChannels;
};
