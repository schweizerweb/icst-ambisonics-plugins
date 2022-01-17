/*
  ==============================================================================

    CustomOscReceiver.cpp
    Created: 17 Jan 2022 10:21:03pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "CustomOscReceiver.h"

CustomOscReceiver::CustomOscReceiver(CustomOscInput* pInput)
{
    addressToMatch.reset(new OSCAddress(pInput->oscString)); // TODO replace {}
}

bool CustomOscReceiver::matchesPattern(OSCAddressPattern* pPattern)
{
    return pPattern->matches(*addressToMatch);
}

bool CustomOscReceiver::handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage)
{
    
}
