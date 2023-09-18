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



#include "CustomOscInput.h"

CustomOscInput::CustomOscInput()
{
    enabledFlag = true;
    oscString = "/demo/{i} {x} {y}";
    commandString = "";
}

CustomOscInput::CustomOscInput(XmlElement* xmlElement)
{
    enabledFlag = xmlElement->getBoolAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_ENABLED, false);
    oscString = xmlElement->getStringAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_STRING, "");
    commandString = xmlElement->getStringAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_COMMAND, "");
}

XmlElement* CustomOscInput::getAsXmlElement(String tagName)
{
    XmlElement* e = new XmlElement(tagName);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_ENABLED, enabledFlag);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_STRING, oscString);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_COMMAND, commandString);

    return e;
}
