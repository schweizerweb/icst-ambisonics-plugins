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



#include "OSCHandlerDecoder.h"

OSCHandlerDecoder::OSCHandlerDecoder(AmbiSourceSet* pAmbiPointArray) : OSCHandler(pAmbiPointArray)
{
}

bool OSCHandlerDecoder::initSpecific()
{
    // no specific init yet
    return true;
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

