/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
