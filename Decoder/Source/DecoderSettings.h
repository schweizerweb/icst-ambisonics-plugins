/*
  ==============================================================================

    DecoderSettings.h
    Created: 11 Dec 2017 11:21:33pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class DecoderSettings
{
public:
	bool oscReceive;
	int oscReceivePort;

	int lastUIWidth;
	int lastUIHeight;

	void saveToXml(XmlElement* xmlElement) const;
	void loadFromXml(XmlElement* xmlElement);
};
