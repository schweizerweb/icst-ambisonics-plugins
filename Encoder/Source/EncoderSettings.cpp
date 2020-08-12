/*
  ==============================================================================

    EncoderSettings.cpp
    Created: 26 Dec 2017 11:55:01pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "EncoderSettings.h"
#define XML_TAG_OSC_RECEIVE	"OscReceive"
#define XML_TAG_OSC_SEND "OscSend"
#define XML_TAG_OSC_SEND_EXT "OscSendExt"
#define XML_TAG_DISTANCE_ENCODING "DistanceEncoding"
#define XML_TAG_DOPPLER_ENCODING "DopplerEncoding"
#define XML_TAG_DISPLAY "Display"
#define XML_ATTRIBUTE_ENABLE "Enable"
#define XML_ATTRIBUTE_PORT "Port"
#define XML_ATTRIBUTE_IP "Ip"
#define XML_ATTRIBUTE_INTERVAL "Interval"
#define XML_ATTRIBUTE_DISTANCE_SCALER "DistanceScaler"

EncoderSettings::EncoderSettings():
	AmbiBasicSettings(DEFAULT_DISTANCE_SCALER),
    oscReceiveFlag(DEFAULT_RECEIVE_FLAG),
	oscReceivePort(DEFALUT_RECEIVE_PORT),
	oscSendFlag(DEFALUT_SEND_FLAG),
	oscSendPort(DEFAULT_SEND_PORT),
	oscSendTargetHost(DEFAULT_SEND_HOST),
	oscSendIntervalMs(DEFAULT_SEND_INTERVAL),
	oscSendExtFlag(DEFALUT_SEND_EXT_FLAG), 
	oscSendExtPort(DEFAULT_SEND_EXT_PORT),
	oscSendExtTargetHost(DEFAULT_SEND_EXT_HOST),
    distanceEncodingFlag(DEFAULT_DIST_ENC_FLAG),
    dopplerEncodingFlag(DEFAULT_DOPPLER_ENC_FLAG),
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
	element->addChildElement(oscReceive);

	XmlElement* oscSend = new XmlElement(XML_TAG_OSC_SEND);
	oscSend->setAttribute(XML_ATTRIBUTE_ENABLE, oscSendFlag);
	oscSend->setAttribute(XML_ATTRIBUTE_PORT, oscSendPort);
	oscSend->setAttribute(XML_ATTRIBUTE_IP, oscSendTargetHost);
	oscSend->setAttribute(XML_ATTRIBUTE_INTERVAL, oscSendIntervalMs);
	element->addChildElement(oscSend);

	XmlElement* oscSendExt = new XmlElement(XML_TAG_OSC_SEND_EXT);
	oscSendExt->setAttribute(XML_ATTRIBUTE_ENABLE, oscSendExtFlag);
	oscSendExt->setAttribute(XML_ATTRIBUTE_PORT, oscSendExtPort);
	oscSendExt->setAttribute(XML_ATTRIBUTE_IP, oscSendExtTargetHost);
	element->addChildElement(oscSendExt);

    writeToPresetXmlElement(element);
	
	return element;
}

void EncoderSettings::loadFromXml(XmlElement* element)
{
	if (element == nullptr)
		return;

	XmlElement* oscReceive = element->getChildByName(XML_TAG_OSC_RECEIVE);
	oscReceiveFlag = oscReceive->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_RECEIVE_FLAG);
	oscReceivePort = oscReceive->getIntAttribute(XML_ATTRIBUTE_PORT, DEFALUT_RECEIVE_PORT);
	
	XmlElement* oscSend = element->getChildByName(XML_TAG_OSC_SEND);
	oscSendFlag = oscSend->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFALUT_SEND_FLAG);
	oscSendPort = oscSend->getIntAttribute(XML_ATTRIBUTE_PORT, DEFAULT_SEND_PORT);
	oscSendTargetHost = oscSend->getStringAttribute(XML_ATTRIBUTE_IP, DEFAULT_SEND_HOST);
	oscSendIntervalMs = oscSend->getIntAttribute(XML_ATTRIBUTE_INTERVAL, DEFAULT_SEND_INTERVAL);

	XmlElement* oscSendExt = element->getChildByName(XML_TAG_OSC_SEND_EXT);
	if (oscSendExt != nullptr)
	{
		oscSendExtFlag = oscSendExt->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFALUT_SEND_EXT_FLAG);
		oscSendExtPort = oscSendExt->getIntAttribute(XML_ATTRIBUTE_PORT, DEFAULT_SEND_EXT_PORT);
		oscSendExtTargetHost = oscSendExt->getStringAttribute(XML_ATTRIBUTE_IP, DEFAULT_SEND_EXT_HOST);
	}

    loadFromPresetXml(element);
}

void EncoderSettings::writeToPresetXmlElement(XmlElement* xmlElement) const
{
    XmlElement* distanceEncoding = new XmlElement(XML_TAG_DISTANCE_ENCODING);
    distanceEncoding->setAttribute(XML_ATTRIBUTE_ENABLE, distanceEncodingFlag);
    distanceEncodingParams.writeToXmlElement(distanceEncoding);
    xmlElement->addChildElement(distanceEncoding);

    XmlElement* dopplerEncoding = new XmlElement(XML_TAG_DOPPLER_ENCODING);
    dopplerEncoding->setAttribute(XML_ATTRIBUTE_ENABLE, dopplerEncodingFlag);
    dopplerEncoding->setAttribute(XML_ATTRIBUTE_DISTANCE_SCALER, getDistanceScaler());
    xmlElement->addChildElement(dopplerEncoding);
}

void EncoderSettings::loadFromPresetXml(XmlElement* xmlElement)
{
    XmlElement* distanceEncoding = xmlElement->getChildByName(XML_TAG_DISTANCE_ENCODING);
    if (distanceEncoding != nullptr)
    {
        distanceEncodingFlag = distanceEncoding->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_DIST_ENC_FLAG);
        distanceEncodingParams.loadFromXmlElement(distanceEncoding);     
    }

    XmlElement* dopplerEncoding = xmlElement->getChildByName(XML_TAG_DOPPLER_ENCODING);
    if (dopplerEncoding != nullptr)
    {
        dopplerEncodingFlag = dopplerEncoding->getBoolAttribute(XML_ATTRIBUTE_ENABLE, DEFAULT_DOPPLER_ENC_FLAG);
        setDistanceScaler(float(dopplerEncoding->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_SCALER, DEFAULT_DISTANCE_SCALER)));
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

	distanceEncodingParams.initialize(pProcessor);
}

void EncoderSettings::parameterValueChanged(int /*parameterIndex*/, float /*newValue*/)
{
    sendChangeMessage();
}

void EncoderSettings::parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/)
{
}
