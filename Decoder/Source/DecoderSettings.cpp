/*
  ==============================================================================

    DecoderSettings.cpp
    Created: 11 Dec 2017 11:21:33pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DecoderSettings.h"

DecoderSettings::DecoderSettings(): oscReceive(DEFALUT_RECEIVE_FLAG), oscReceivePort(DEFAULT_RECEIVE_PORT), lastUIWidth(DEFAULT_UI_WIDTH), lastUIHeight(DEFAULT_UI_HEIGTH)
{
}

void DecoderSettings::saveToXml(XmlElement* xmlElement) const
{
	xmlElement->setAttribute("uiWidth", lastUIWidth);
	xmlElement->setAttribute("uiHeight", lastUIHeight);
	xmlElement->setAttribute("oscReceive", oscReceive);
	xmlElement->setAttribute("oscReceivePort", oscReceivePort);
}

void DecoderSettings::loadFromXml(XmlElement* xmlElement)
{
	lastUIWidth = jmax(xmlElement->getIntAttribute("uiWidth", lastUIWidth), DEFAULT_UI_WIDTH);
	lastUIHeight = jmax(xmlElement->getIntAttribute("uiHeight", lastUIHeight), DEFAULT_UI_HEIGTH);
	oscReceive = xmlElement->getBoolAttribute("oscReceive", DEFALUT_RECEIVE_FLAG);
	oscReceivePort = xmlElement->getIntAttribute("oscReceivePort", DEFAULT_RECEIVE_PORT);
}
