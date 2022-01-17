/*
  ==============================================================================

    CustomOscReceiver.h
    Created: 17 Jan 2022 10:21:03pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "CustomOscInput.h"
#include "../../Common/AmbiSourceSet.h"

class CustomOscReceiver
{
public:
    CustomOscReceiver(CustomOscInput* pInput);
    
public:
    bool matchesPattern(OSCAddressPattern* pPattern);
    bool handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage);
    
private:
    std::unique_ptr<OSCAddress> addressToMatch;
};
