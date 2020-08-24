/*
  ==============================================================================

    OSCHandlerDecoder.cpp
    Created: 4 May 2020 11:22:17am
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "OSCHandlerDecoder.h"

OSCHandlerDecoder::OSCHandlerDecoder(AmbiSourceSet* pAmbiPointArray) : OSCHandler(pAmbiPointArray)
{
}

bool OSCHandlerDecoder::handleSpecific(const OSCMessage &message)
{
    OSCAddressPattern pattern = message.getAddressPattern();
    if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_INTERNAL)))
    {
        handleOwnInternalStyle(message);
        return true;
    }
    
    return false;
}


void OSCHandlerDecoder::handleOwnInternalStyle(const OSCMessage& message) const
{
    bool valid =
        message.size() == 7
        && message[0].isString()
        && message[1].isString()
        && message[2].isFloat32()
        && message[3].isFloat32()
        && message[4].isFloat32()
        && message[5].isFloat32()
        && message[6].isInt32();
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(internal style)", &message);
        return;
    }
    
    String id = message[0].getString();
    String name = message[1].getString();
    float x = message[2].getFloat32();
    float y = message[3].getFloat32();
    float z = message[4].getFloat32();
    float rms = message[5].getFloat32();
    Colour color = Colour(uint32(message[6].getInt32()));
    
    pAmbiPoints->setChannelXYZExt(id, name, x, y, z, rms, color);
}

