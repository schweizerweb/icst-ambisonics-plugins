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
#define XML_ATTRIBUTE_ENABLE "Enable"
#define XML_ATTRIBUTE_PORT "Port"
#define XML_ATTRIBUTE_IP "Ip"

EncoderSettings::EncoderSettings(): oscReceiveFlag(false), oscReceivePort(0), oscSendFlag(false), oscSendPort(0)
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
	oscReceive->setAttribute(XML_ATTRIBUTE_ENABLE, oscSendFlag);
	oscReceive->setAttribute(XML_ATTRIBUTE_PORT, oscSendPort);
	oscReceive->setAttribute(XML_ATTRIBUTE_IP, oscSendIp.toString());
	element->addChildElement(oscSend);

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
	oscSendIp = IPAddress(oscSend->getStringAttribute(XML_ATTRIBUTE_IP, "127.0.0.1"));
}
