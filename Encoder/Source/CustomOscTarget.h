/*
  ==============================================================================

    CustomOscTarget.h
    Created: 15 Jan 2021 11:14:10pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_ENABLED "Enabled"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_HOST "Host"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_PORT "Port"
#define XML_ATTRIBUTE_CUSTOM_OSC_TARGET_STRING "OscString"

#define XML_ATTRIBUTE_CUSTOM_OSC_INPUT_ENABLED "Enabled"
#define XML_ATTRIBUTE_CUSTOM_OSC_INPUT_STRING "OscString"
#define XML_ATTRIBUTE_CUSTOM_OSC_INPUT_COMMAND "CommandString"

class CustomOscTarget
{
public:
    CustomOscTarget();
    CustomOscTarget(XmlElement* xmlElement);
    ~CustomOscTarget();
    XmlElement* getAsXmlElement(String tagName);

    bool enabledFlag;
    String oscString;
    String targetHost;
    int targetPort;
};

class CustomOscInput
{
public:
    CustomOscInput();
    CustomOscInput(XmlElement* xmlElement);
    ~CustomOscInput();
    
    XmlElement* getAsXmlElement(String tagName);

    bool enabledFlag;
    String oscString;
    String commandString;
};
