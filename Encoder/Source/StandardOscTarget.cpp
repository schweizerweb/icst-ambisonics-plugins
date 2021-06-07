/*
  ==============================================================================

    StandardOscTarget.cpp
    Created: 18 Apr 2021 12:52:07am
    Author:  Schweizer Christian

  ==============================================================================
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
