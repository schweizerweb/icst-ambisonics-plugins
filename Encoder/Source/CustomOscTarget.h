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

#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_ENABLED "Enabled"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_HOST "Host"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_PORT "Port"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_STRING "OscString"

class CustomOscTarget
{
public:
    CustomOscTarget();
    CustomOscTarget(XmlElement* xmlElement);
    
    XmlElement* getAsXmlElement(String tagName);

    bool enabledFlag;
    String oscString;
    String targetHost;
    int targetPort;
};
