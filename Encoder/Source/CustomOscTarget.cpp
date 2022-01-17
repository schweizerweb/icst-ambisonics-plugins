/*
  ==============================================================================

    CustomOscTarget.cpp
    Created: 15 Jan 2021 11:14:10pm
    Author:  chris

  ==============================================================================
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

CustomOscTarget::~CustomOscTarget()
{
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
