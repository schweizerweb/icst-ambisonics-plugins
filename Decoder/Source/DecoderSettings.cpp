/*
  ==============================================================================

    DecoderSettings.cpp
    Created: 11 Dec 2017 11:21:33pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DecoderSettings.h"

DecoderSettings::DecoderSettings(): oscReceive(DEFALUT_RECEIVE_FLAG), oscReceivePort(DEFAULT_RECEIVE_PORT), oscReceiveTimeoutMs(DEFAULT_RECEIVE_TIMEOUT), lastUIWidth(DEFAULT_UI_WIDTH), lastUIHeight(DEFAULT_UI_HEIGTH), editMode(DEFAULT_EDIT_MODE_FLAG)
{
}

void DecoderSettings::saveToXml(XmlElement* xmlElement) const
{
	xmlElement->setAttribute("uiWidth", lastUIWidth);
	xmlElement->setAttribute("uiHeight", lastUIHeight);
	xmlElement->setAttribute("oscReceive", oscReceive);
	xmlElement->setAttribute("oscReceivePort", oscReceivePort);
	xmlElement->setAttribute("oscReceiveTimeoutMs", oscReceiveTimeoutMs);
	xmlElement->setAttribute("editMode", editMode);
}

void DecoderSettings::loadFromXml(XmlElement* xmlElement)
{
	lastUIWidth = jmax(xmlElement->getIntAttribute("uiWidth", lastUIWidth), DEFAULT_UI_WIDTH);
	lastUIHeight = jmax(xmlElement->getIntAttribute("uiHeight", lastUIHeight), DEFAULT_UI_HEIGTH);
	oscReceive = xmlElement->getBoolAttribute("oscReceive", DEFALUT_RECEIVE_FLAG);
	oscReceivePort = xmlElement->getIntAttribute("oscReceivePort", DEFAULT_RECEIVE_PORT);
	oscReceiveTimeoutMs = xmlElement->getIntAttribute("oscReceiveTimeoutMs", DEFAULT_RECEIVE_TIMEOUT);
	editMode = xmlElement->getBoolAttribute("editMode", DEFAULT_EDIT_MODE_FLAG);
}
