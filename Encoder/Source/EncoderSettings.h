/*
  ==============================================================================

    EncoderSettings.h
    Created: 26 Dec 2017 11:55:01pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class EncoderSettings
{
public:
	EncoderSettings();
	~EncoderSettings();
	XmlElement* getAsXmlElement(String tagName) const;
	void loadFromXml(XmlElement* xml_element);

public:
	bool oscReceiveFlag;
	int oscReceivePort;

	bool oscSendFlag;
	int oscSendPort;
	String oscSendTargetHost;
	int oscSendIntervalMs;
};
