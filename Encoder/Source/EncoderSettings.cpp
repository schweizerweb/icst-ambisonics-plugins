/*
  ==============================================================================

    EncoderSettings.cpp
    Created: 26 Dec 2017 11:55:01pm
    Author:  chris

  ==============================================================================
*/

#include "EncoderSettings.h"
#define XML_TAG_OSC_RECEIVE	"OscReceive"
#define XML_TAG_OSC_SEND "OscSend"
#define XML_TAG_DISTANCE_ENCODING "DistanceEncoding"
#define XML_ATTRIBUTE_ENABLE "Enable"
#define XML_ATTRIBUTE_PORT "Port"
#define XML_ATTRIBUTE_IP "Ip"
#define XML_ATTRIBUTE_INTERVAL "Interval"
#define XML_ATTRIBUTE_UNIT_CIRCLE_RADIUS "UnitCircleRadius"

EncoderSettings::EncoderSettings(): oscReceiveFlag(false), oscReceivePort(0), oscSendFlag(false), oscSendPort(0), oscSendIntervalMs(100), distanceEncodingFlag(true), unitCircleRadius(0.1f)
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

	XmlElement* distanceEncoding = new XmlElement(XML_TAG_DISTANCE_ENCODING);
	distanceEncoding->setAttribute(XML_ATTRIBUTE_ENABLE, distanceEncodingFlag);
	distanceEncoding->setAttribute(XML_ATTRIBUTE_UNIT_CIRCLE_RADIUS, unitCircleRadius);
	element->addChildElement(distanceEncoding);

	return element;
}

void EncoderSettings::loadFromXml(XmlElement* element)
{
	if (element == nullptr)
		return;

	XmlElement* oscReceive = element->getChildByName(XML_TAG_OSC_RECEIVE);
	oscReceiveFlag = oscReceive->getBoolAttribute(XML_ATTRIBUTE_ENABLE, false);
	oscReceivePort = oscReceive->getIntAttribute(XML_ATTRIBUTE_PORT, 5011);
	
	XmlElement* oscSend = element->getChildByName(XML_TAG_OSC_SEND);
	oscSendFlag = oscSend->getBoolAttribute(XML_ATTRIBUTE_ENABLE, false);
	oscSendPort = oscSend->getIntAttribute(XML_ATTRIBUTE_PORT, 5014);
	oscSendTargetHost = oscSend->getStringAttribute(XML_ATTRIBUTE_IP, "127.0.0.1");
	oscSendIntervalMs = oscSend->getIntAttribute(XML_ATTRIBUTE_INTERVAL, 100);

	XmlElement* distanceEncoding = element->getChildByName(XML_TAG_DISTANCE_ENCODING);
	if(distanceEncoding != nullptr)
	{
		distanceEncodingFlag = distanceEncoding->getBoolAttribute(XML_ATTRIBUTE_ENABLE, true);
		unitCircleRadius = float(distanceEncoding->getDoubleAttribute(XML_ATTRIBUTE_UNIT_CIRCLE_RADIUS, 0.1));
	}
}
