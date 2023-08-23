/*
  ==============================================================================

    CustomOscInput.h
    Created: 17 Jan 2022 10:20:47pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#define XML_ATTRIBUTE_CUSTOM_OSC_INPUT_ENABLED "Enabled"
#define XML_ATTRIBUTE_CUSTOM_OSC_INPUT_STRING "OscString"
#define XML_ATTRIBUTE_CUSTOM_OSC_INPUT_COMMAND "CommandString"

class CustomOscInput
{
public:
    CustomOscInput();
    CustomOscInput(XmlElement* xmlElement);
    
    XmlElement* getAsXmlElement(String tagName);

    bool enabledFlag;
    String oscString;
    String commandString;
};
