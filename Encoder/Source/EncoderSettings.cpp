/*
  ==============================================================================

    EncoderSettings.cpp
    Created: 26 Dec 2017 11:55:01pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "EncoderSettings.h"
#define XML_TAG_OSC_RECEIVE	"OscReceive"
#define XML_TAG_OSC_HIDE_WARNINGS    "HideWarnings"
#define XML_TAG_OSC_SEND "OscSend"
#define XML_TAG_OSC_SEND_EXT "OscSendExt"
#define XML_TAG_DISTANCE_ENCODING "DistanceEncoding"
#define XML_TAG_DOPPLER_ENCODING "DopplerEncoding"
#define XML_TAG_MASTER_GAIN "MasterGain"
#define XML_TAG_DISPLAY "Display"
#define XML_TAG_OSC_SEND_EXT_XYZ "Xyz"
#define XML_TAG_OSC_SEND_EXT_AED "Aed"
#define XML_TAG_OSC_SEND_EXT_XYZ_INDEX "XyzIndex"
#define XML_TAG_OSC_SEND_EXT_AED_INDEX "AedIndex"
#define XML_ATTRIBUTE_ENABLE "Enable"
#define XML_ATTRIBUTE_PORT "Port"
#define XML_ATTRIBUTE_HOST "Host"
#define XML_ATTRIBUTE_INTERVAL "Interval"
#define XML_ATTRIBUTE_CONTINUOUS "Continuous"
#define XML_ATTRIBUTE_DISTANCE_SCALER "DistanceScaler"
#define XML_ATTRIBUTE_VALUE "Value"
#define XML_TAG_CUSTOM_OSC_TARGETS "CustomOscTargets"
#define XML_TAG_CUSTOM_OSC_TARGET "CustomOscTarget"
#define XML_TAG_CUSTOM_OSC_INPUTS "CustomOscInputs"
#define XML_TAG_CUSTOM_OSC_INPUT "CustomOscInput"

EncoderSettings::EncoderSettings():
	AmbiBasicSettings(DEFAULT_DISTANCE_SCALER),
    oscReceiveFlag(DEFAULT_RECEIVE_FLAG),
	oscReceivePort(DEFAULT_RECEIVE_PORT),
	oscSendFlag(DEFAULT_SEND_FLAG),
	oscSendPort(DEFAULT_SEND_PORT),
	oscSendTargetHost(DEFAULT_SEND_HOST),
	oscSendIntervalMs(DEFAULT_SEND_INTERVAL),
	oscSendExtMasterFlag(DEFAULT_SEND_EXT_MASTER_FLAG), 
	oscSendExtIntervalMs(DEFAULT_SEND_EXT_INTERVAL_MS),
    oscSendExtContinuousFlag(DEFAULT_SEND_EXT_CONTINUOUS_FLAG),
    oscSendExtXyz(new StandardOscTarget()),
    oscSendExtAed(new StandardOscTarget()),
    oscSendExtXyzIndex(new StandardOscTarget()),
    oscSendExtAedIndex(new StandardOscTarget()),
    distanceEncodingFlag(DEFAULT_DIST_ENC_FLAG),
    dopplerEncodingFlag(DEFAULT_DOPPLER_ENC_FLAG),
    hideWarnings(DEFAULT_HIDE_WARNINGS),
    masterGain(nullptr),
    localMasterGain(DEFAULT_MASTER_GAIN)
{
}

EncoderSettings::~EncoderSettings()
{
}

XmlElement* EncoderSettings::getAsXmlElement(String tagName) const
{
	XmlElement* element = new XmlElement(tagName);

    XmlElement* oscReceive = new XmlElement(XML_TAG_OSC_RECEIVE);
	oscReceive->setAttribute(XML_ATTRIBUTE_ENABLE, oscReceiveFlag);
	oscReceive->setAttribute(XML_ATTRIBUTE_PORT, oscReceivePort);
	XmlElement* customInputs = new XmlElement(XML_TAG_CUSTOM_OSC_INPUTS);
    for (auto i : customOscInput)
    {
        customInputs->addChildElement(i->getAsXmlElement(XML_TAG_CUSTOM_OSC_INPUT));
    }
    oscReceive->addChildElement(customInputs);
    element->addChildElement(oscReceive);
    
	XmlElement* oscSend = new XmlElement(XML_TAG_OSC_SEND);
	oscSend->setAttribute(XML_ATTRIBUTE_ENABLE, oscSendFlag);
	oscSend->setAttribute(XML_ATTRIBUTE_PORT, oscSendPort);
	oscSend->setAttribute(XML_ATTRIBUTE_HOST, oscSendTargetHost);
	oscSend->setAttribute(XML_ATTRIBUTE_INTERVAL, oscSendIntervalMs);
	element->addChildElement(oscSend);

	XmlElement* oscSendExt = new XmlElement(XML_TAG_OSC_SEND_EXT);
	oscSendExt->setAttribute(XML_ATTRIBUTE_ENABLE, oscSendExtMasterFlag);
	oscSendExt->setAttribute(XML_ATTRIBUTE_INTERVAL, oscSendExtIntervalMs);
    oscSendExt->setAttribute(XML_ATTRIBUTE_CONTINUOUS, oscSendExtContinuousFlag);
	XmlElement* oscSendExtXyzXml = oscSendExtXyz->getAsXmlElement(XML_TAG_OSC_SEND_EXT_XYZ);
	oscSendExt->addChildElement(oscSendExtXyzXml);
    XmlElement* oscSendExtAedXml = oscSendExtAed->getAsXmlElement(XML_TAG_OSC_SEND_EXT_AED);
	oscSendExt->addChildElement(oscSendExtAedXml);
    XmlElement* oscSendExtXyzIndexXml = oscSendExtXyzIndex->getAsXmlElement(XML_TAG_OSC_SEND_EXT_XYZ_INDEX);
    oscSendExt->addChildElement(oscSendExtXyzIndexXml);
    XmlElement* oscSendExtAedIndexXml = oscSendExtAedIndex->getAsXmlElement(XML_TAG_OSC_SEND_EXT_AED_INDEX);
    oscSendExt->addChildElement(oscSendExtAedIndexXml);
	XmlElement* customTargets = new XmlElement(XML_TAG_CUSTOM_OSC_TARGETS);
	for (auto target : customOscTargets)
	{
		customTargets->addChildElement(target->getAsXmlElement(XML_TAG_CUSTOM_OSC_TARGET));
	}
	oscSendExt->addChildElement(customTargets);
    element->addChildElement(oscSendExt);
    
    XmlElement* hideWarningsXml = new XmlElement(XML_TAG_OSC_HIDE_WARNINGS);
    hideWarningsXml->setAttribute(XML_ATTRIBUTE_ENABLE, hideWarnings);
    element->addChildElement(hideWarningsXml);
	
    XmlElement* distanceEncoding = new XmlElement(XML_TAG_DISTANCE_ENCODING);
    distanceEncoding->setAttribute(XML_ATTRIBUTE_ENABLE, distanceEncodingFlag);
    distanceEncodingParams.writeToXmlElement(distanceEncoding);
    element->addChildElement(distanceEncoding);

    XmlElement* dopplerEncoding = new XmlElement(XML_TAG_DOPPLER_ENCODING);
    dopplerEncoding->setAttribute(XML_ATTRIBUTE_ENABLE, dopplerEncodingFlag);
    dopplerEncoding->setAttribute(XML_ATTRIBUTE_DISTANCE_SCALER, getDistanceScaler());
    element->addChildElement(dopplerEncoding);
    
    writeToPresetXmlElement(element);
	
	return element;
}

void EncoderSettings::loadFromXml(XmlElement* element)
{
	if (element == nullptr)
		return;

    customOscTargets.clear();
    
	XmlElement* oscReceive = element->getChildByName(XML_TAG_OSC_RECEIVE);
	oscReceiveFlag = oscReceive->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_RECEIVE_FLAG);
	oscReceivePort = oscReceive->getIntAttribute(XML_ATTRIBUTE_PORT, DEFAULT_RECEIVE_PORT);
    XmlElement* customInputs = oscReceive->getChildByName(XML_TAG_CUSTOM_OSC_INPUTS);
    if (customInputs != nullptr)
    {
        XmlElement* i = customInputs->getChildByName(XML_TAG_CUSTOM_OSC_INPUT);
        while (i != nullptr)
        {
            customOscInput.add(new CustomOscInput(i));

            i = i->getNextElement();
        }
    }
    
	XmlElement* oscSend = element->getChildByName(XML_TAG_OSC_SEND);
	oscSendFlag = oscSend->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_SEND_FLAG);
	oscSendPort = oscSend->getIntAttribute(XML_ATTRIBUTE_PORT, DEFAULT_SEND_PORT);
	oscSendTargetHost = oscSend->getStringAttribute(XML_ATTRIBUTE_HOST, DEFAULT_SEND_HOST);
	oscSendIntervalMs = oscSend->getIntAttribute(XML_ATTRIBUTE_INTERVAL, DEFAULT_SEND_INTERVAL);

	XmlElement* oscSendExt = element->getChildByName(XML_TAG_OSC_SEND_EXT);
	if (oscSendExt != nullptr)
	{
		oscSendExtMasterFlag = oscSendExt->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_SEND_EXT_MASTER_FLAG);
		oscSendExtIntervalMs = oscSendExt->getIntAttribute(XML_ATTRIBUTE_INTERVAL, DEFAULT_SEND_EXT_INTERVAL_MS);
        oscSendExtContinuousFlag = oscSendExt->getBoolAttribute(XML_ATTRIBUTE_CONTINUOUS, DEFAULT_SEND_EXT_CONTINUOUS_FLAG);
        
		XmlElement* oscSendExtXyzXml = oscSendExt->getChildByName(XML_TAG_OSC_SEND_EXT_XYZ);
		oscSendExtXyz.reset(new StandardOscTarget(oscSendExtXyzXml));
		
        XmlElement* oscSendExtAedXml = oscSendExt->getChildByName(XML_TAG_OSC_SEND_EXT_AED);
		oscSendExtAed.reset(new StandardOscTarget(oscSendExtAedXml));
        
        XmlElement* oscSendExtXyzIndexXml = oscSendExt->getChildByName(XML_TAG_OSC_SEND_EXT_XYZ_INDEX);
        oscSendExtXyzIndex.reset(new StandardOscTarget(oscSendExtXyzIndexXml));
        
        XmlElement* oscSendExtAedIndexXml = oscSendExt->getChildByName(XML_TAG_OSC_SEND_EXT_AED_INDEX);
        oscSendExtAedIndex.reset(new StandardOscTarget(oscSendExtAedIndexXml));
        
		XmlElement* customTargets = oscSendExt->getChildByName(XML_TAG_CUSTOM_OSC_TARGETS);
		if (customTargets != nullptr)
		{
			XmlElement* target = customTargets->getChildByName(XML_TAG_CUSTOM_OSC_TARGET);
			while (target != nullptr)
			{
				customOscTargets.add(new CustomOscTarget(target));

				target = target->getNextElement();
			}
		}
	}

    XmlElement* hideWarningsXml = element->getChildByName(XML_TAG_OSC_HIDE_WARNINGS);
    if(hideWarningsXml != nullptr)
    hideWarnings = hideWarningsXml->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_HIDE_WARNINGS);
    
    XmlElement* distanceEncoding = element->getChildByName(XML_TAG_DISTANCE_ENCODING);
    if (distanceEncoding != nullptr)
    {
        distanceEncodingFlag = distanceEncoding->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_DIST_ENC_FLAG);
        distanceEncodingParams.loadFromXmlElement(distanceEncoding);
    }

    XmlElement* dopplerEncoding = element->getChildByName(XML_TAG_DOPPLER_ENCODING);
    if (dopplerEncoding != nullptr)
    {
        dopplerEncodingFlag = dopplerEncoding->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_DOPPLER_ENC_FLAG);
        setDistanceScaler(float(dopplerEncoding->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_SCALER, DEFAULT_DISTANCE_SCALER)));
    }
    
    loadFromPresetXml(element);
}

void EncoderSettings::writeToPresetXmlElement(XmlElement* xmlElement) const
{
    XmlElement* masterGainElement = new XmlElement(XML_TAG_MASTER_GAIN);
    masterGainElement->setAttribute(XML_ATTRIBUTE_VALUE, getMasterGain());
    xmlElement->addChildElement(masterGainElement);
}

void EncoderSettings::loadFromPresetXml(XmlElement* xmlElement)
{
    XmlElement* masterGainElement = xmlElement->getChildByName(XML_TAG_MASTER_GAIN);
    if (masterGainElement != nullptr)
    {
        setMasterGain(float(masterGainElement->getDoubleAttribute(XML_ATTRIBUTE_VALUE)));
    }
}

float EncoderSettings::getMasterGain() const
{
    return masterGain != nullptr ? masterGain->get() : localMasterGain;
}

bool EncoderSettings::setMasterGain(float gainDb)
{
    if (gainDb < EncoderConstants::MasterGainMin || gainDb > EncoderConstants::MasterGainMax)
        return false;

    if (masterGain != nullptr)
        *masterGain = gainDb;
    else
        localMasterGain = gainDb;

    return true;
}

void EncoderSettings::initialize(AudioProcessor* pProcessor)
{
    masterGain = new AudioParameterFloat("MasterGain", "MasterGain", NormalisableRange<float>(EncoderConstants::MasterGainMin, EncoderConstants::MasterGainMax), localMasterGain, "Master Gain for B-Format output");

    pProcessor->addParameter(masterGain);

    masterGain->addListener(this);
}

void EncoderSettings::parameterValueChanged(int /*parameterIndex*/, float /*newValue*/)
{
    sendChangeMessage();
}

void EncoderSettings::parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/)
{
}
