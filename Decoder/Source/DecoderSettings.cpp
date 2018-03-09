/*
  ==============================================================================

    DecoderSettings.cpp
    Created: 11 Dec 2017 11:21:33pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DecoderSettings.h"

void DecoderSettings::saveToXml(XmlElement* xmlElement) const
{
	xmlElement->setAttribute("uiWidth", lastUIWidth);
	xmlElement->setAttribute("uiHeight", lastUIHeight);
	xmlElement->setAttribute("oscReceive", oscReceive);
	xmlElement->setAttribute("oscReceivePort", oscReceivePort);
}

void DecoderSettings::loadFromXml(XmlElement* xmlElement)
{
	lastUIWidth = jmax(xmlElement->getIntAttribute("uiWidth", lastUIWidth), 300);
	lastUIHeight = jmax(xmlElement->getIntAttribute("uiHeight", lastUIHeight), 600);
	oscReceive = xmlElement->getBoolAttribute("oscReceive", true);
	oscReceivePort = xmlElement->getIntAttribute("oscReceivePort", 50000);
}
