/*
  ==============================================================================

    EncoderSettings.h
    Created: 26 Dec 2017 11:55:01pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiBasicSettings.h"
#include "DistanceEncodingParams.h"


#define DEFAULT_RECEIVE_FLAG		false
#define DEFALUT_RECEIVE_PORT		50001
#define DEFALUT_SEND_FLAG			true
#define DEFAULT_SEND_PORT			50000
#define DEFAULT_SEND_HOST			"127.0.0.1"
#define DEFAULT_SEND_INTERVAL		50
#define DEFALUT_SEND_EXT_FLAG		false
#define DEFAULT_SEND_EXT_PORT		50002
#define DEFAULT_SEND_EXT_HOST	    "127.0.0.1"
#define DEFAULT_DIST_ENC_FLAG		true
#define DEFAULT_DOPPLER_ENC_FLAG	false
#define MULTI_ENCODER_MODE (JucePlugin_MaxNumInputChannels > 1)

class EncoderSettings : public AmbiBasicSettings
{
public:
	EncoderSettings();
	~EncoderSettings();
	XmlElement* getAsXmlElement(String tagName) const;
	void loadFromXml(XmlElement* xml_element);
    void writeToPresetXmlElement(XmlElement* xmlElement) const;
    void loadFromPresetXml(XmlElement* xmlElement);

public:
	bool oscReceiveFlag;
	int oscReceivePort;

	bool oscSendFlag;
	int oscSendPort;
	String oscSendTargetHost;
	int oscSendIntervalMs;
	
	bool oscSendExtFlag;
	int oscSendExtPort;
	String oscSendExtTargetHost;

	bool distanceEncodingFlag;
	DistanceEncodingParams distanceEncodingParams;

	bool dopplerEncodingFlag;
};
