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



#include "StandardOscTarget.h"
StandardOscTarget::StandardOscTarget()
{
    enabledFlag = DEFAULT_SEND_EXT_STANDARD_FLAG;
    targetHost = DEFAULT_SEND_EXT_STANDARD_HOST;
    targetPort = DEFAULT_SEND_EXT_STANDARD_PORT;
}

StandardOscTarget::StandardOscTarget(XmlElement* xmlElement) : StandardOscTarget()
{
    if(xmlElement != nullptr)
    {
        enabledFlag = xmlElement->getBoolAttribute(XML_ATTRIBUTE_STANDARD_OSC_TARGET_ENABLE, DEFAULT_SEND_EXT_STANDARD_FLAG);
        targetHost = xmlElement->getStringAttribute(XML_ATTRIBUTE_STANDARD_OSC_TARGET_HOST, DEFAULT_SEND_EXT_STANDARD_HOST);
        targetPort = xmlElement->getIntAttribute(XML_ATTRIBUTE_STANDARD_OSC_TARGET_PORT, DEFAULT_SEND_EXT_STANDARD_PORT);
    }
}

StandardOscTarget::~StandardOscTarget()
{
}

XmlElement* StandardOscTarget::getAsXmlElement(String tagName)
{
    XmlElement* e = new XmlElement(tagName);
    e->setAttribute(XML_ATTRIBUTE_STANDARD_OSC_TARGET_ENABLE, enabledFlag);
    e->setAttribute(XML_ATTRIBUTE_STANDARD_OSC_TARGET_HOST, targetHost);
    e->setAttribute(XML_ATTRIBUTE_STANDARD_OSC_TARGET_PORT, targetPort);
    
    return e;
}
