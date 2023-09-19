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



#include "CustomOscTarget.h"

CustomOscTarget::CustomOscTarget()
{
    enabledFlag = false;
    targetHost = "127.0.0.1";
    targetPort = 50003;
    oscString = "/demo/{n} {d}";
}

CustomOscTarget::CustomOscTarget(XmlElement* xmlElement)
{
    enabledFlag = xmlElement->getBoolAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_ENABLED, false);
    targetHost = xmlElement->getStringAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_HOST, "");
    targetPort = xmlElement->getIntAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_PORT, 0);
    oscString = xmlElement->getStringAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_STRING, "");
}

XmlElement* CustomOscTarget::getAsXmlElement(String tagName)
{
    XmlElement* e = new XmlElement(tagName);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_ENABLED, enabledFlag);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_HOST, targetHost);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_PORT, targetPort);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_TARGET_STRING, oscString);

    return e;
}
