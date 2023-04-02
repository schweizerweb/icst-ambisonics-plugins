/*
  ==============================================================================

    DecoderSettings.h
    Created: 11 Dec 2017 11:21:33pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#define DEFAULT_EDIT_MODE_FLAG	true
#define DEFALUT_RECEIVE_FLAG	false
#define DEFAULT_RECEIVE_PORT	50000
#define DEFAULT_RECEIVE_TIMEOUT	1000
#define DEFAULT_UI_WIDTH		300
#define DEFAULT_UI_HEIGTH		600

class DecoderSettings
{
public:
	DecoderSettings();

	bool oscReceive;
	int oscReceivePort;
	int oscReceiveTimeoutMs;

	int lastUIWidth;
	int lastUIHeight;

	bool editMode;
    
    void saveToXml(XmlElement* xmlElement) const;
	void loadFromXml(XmlElement* xmlElement);
};
