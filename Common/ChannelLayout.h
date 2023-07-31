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
