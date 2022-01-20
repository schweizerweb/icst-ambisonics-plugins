/*
  ==============================================================================

    CustomOscInput.cpp
    Created: 17 Jan 2022 10:20:47pm
    Author:  Schweizer Christian

  ==============================================================================
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

CustomOscInput::~CustomOscInput()
{
    
}

XmlElement* CustomOscInput::getAsXmlElement(String tagName)
{
    XmlElement* e = new XmlElement(tagName);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_ENABLED, enabledFlag);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_STRING, oscString);
    e->setAttribute(XML_ATTRIBUTE_CUSTOM_OSC_INPUT_COMMAND, commandString);

    return e;
}
