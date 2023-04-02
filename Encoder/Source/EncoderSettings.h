/*
  ==============================================================================

    EncoderSettings.h
    Created: 26 Dec 2017 11:55:01pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "DistanceEncodingParams.h"
#include "CustomOscTarget.h"
#include "CustomOscInput.h"
#include "StandardOscTarget.h"


#define DEFAULT_RECEIVE_FLAG		false
#define DEFAULT_RECEIVE_PORT		50001
#define DEFAULT_HANDLE_STANDARD_FORMAT_FLAG true
#define DEFAULT_SEND_FLAG			true
#define DEFAULT_SEND_PORT			50000
#define DEFAULT_SEND_HOST			"127.0.0.1"
#define DEFAULT_SEND_INTERVAL		50
#define DEFAULT_SEND_EXT_MASTER_FLAG		false
#define DEFAULT_SEND_EXT_INTERVAL_MS		50
#define DEFAULT_SEND_EXT_CONTINUOUS_FLAG    false
#define DEFAULT_DIST_ENC_FLAG		true
#define DEFAULT_DOPPLER_ENC_FLAG	false
#define DEFAULT_HIDE_WARNINGS       false
#define MULTI_ENCODER_MODE (JucePlugin_MaxNumInputChannels > 1)
#define MAXIMUM_NUMBER_OF_GROUPS   (JucePlugin_MaxNumInputChannels > 1 ? 8 : 0)

class EncoderSettings
{
public:
	EncoderSettings();
	~EncoderSettings();
	XmlElement* getAsXmlElement(String tagName) const;
	void loadFromXml(XmlElement* xml_element);
    int getAmbiChannelCount() const;
    
    bool oscReceiveFlag;
	int oscReceivePort;
    bool oscHandleStandardFormatFlag;
    
	bool oscSendFlag;
	int oscSendPort;
	String oscSendTargetHost;
	int oscSendIntervalMs;
	
	bool oscSendExtMasterFlag;
	int oscSendExtIntervalMs;
    bool oscSendExtContinuousFlag;

    std::unique_ptr<StandardOscTarget> oscSendExtXyz;
    std::unique_ptr<StandardOscTarget> oscSendExtAed;
    std::unique_ptr<StandardOscTarget> oscSendExtXyzIndex;
    std::unique_ptr<StandardOscTarget> oscSendExtAedIndex;
    
	OwnedArray<CustomOscTarget> customOscTargets;
    OwnedArray<CustomOscInput> customOscInput;
    
    int ambiOrder;
    
	bool distanceEncodingFlag;
	DistanceEncodingParams distanceEncodingParams;
	
	bool dopplerEncodingFlag;

    bool hideWarnings;
};
