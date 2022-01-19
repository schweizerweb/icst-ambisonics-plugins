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
#include "CustomOscBase.h"
#include "../../Common/AmbiSourceSet.h"

class CustomOscReceiver : CustomOscBase
{
public:
    CustomOscReceiver(CustomOscInput* pInput, ScalingInfo* pScaling);
    
public:
    bool matchesPattern(OSCAddress* pAddress);
    bool handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage);
    
private:
    std::unique_ptr<OSCAddressPattern> patternToMatch;
    bool hasIndex;
    bool hasName;
    bool xyzMode;
    bool isValid;
};
