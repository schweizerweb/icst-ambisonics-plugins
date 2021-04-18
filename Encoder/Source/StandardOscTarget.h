/*
  ==============================================================================

    StandardOscTarget.h
    Created: 18 Apr 2021 12:52:07am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define XML_ATTRIBUTE_STANDARD_OSC_TARGET_ENABLE "Enable"
#define XML_ATTRIBUTE_STANDARD_OSC_TARGET_HOST "Host"
#define XML_ATTRIBUTE_STANDARD_OSC_TARGET_PORT "Port"

#define DEFAULT_SEND_EXT_STANDARD_FLAG       false
#define DEFAULT_SEND_EXT_STANDARD_PORT       50002
#define DEFAULT_SEND_EXT_STANDARD_HOST       "127.0.0.1"

class StandardOscTarget
{
public:
    StandardOscTarget();
    StandardOscTarget(XmlElement* xmlElement);
    ~StandardOscTarget();
    XmlElement* getAsXmlElement(String tagName);

    bool enabledFlag;
    String targetHost;
    int targetPort;
};
