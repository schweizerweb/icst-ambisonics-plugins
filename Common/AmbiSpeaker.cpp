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



#include "AmbiSpeaker.h"

AmbiSpeaker::AmbiSpeaker(AmbiSpeaker* other, bool copyImage) : 
	AmbiPoint(other, copyImage), filterInfo(other->filterInfo), filterBypass(other->filterBypass)
{
}

AmbiSpeaker::AmbiSpeaker(String _id, Point3D<double> _point, String _name, Colour _color, double _gain) : 
	AmbiPoint(_id, _point, _name, _color, _gain), filterBypass(DEFAULT_BYPASS_FILTER)
{
}

AmbiSpeaker::AmbiSpeaker(XmlElement* element) :
	AmbiPoint(element)
{
	filterInfo.loadFromXmlElement(element);
	filterBypass = bool(element->getBoolAttribute(XML_ATTRIBUTE_POINT_FILTER_BYPASS, DEFAULT_BYPASS_FILTER));
}

FilterBankInfo* AmbiSpeaker::getFilterInfo()
{
	return &filterInfo;
}

bool AmbiSpeaker::getFilterBypass()
{
    return filterBypass;
}

void AmbiSpeaker::setFilterBypass(bool byPass)
{
    filterBypass = byPass;
}

float AmbiSpeaker::getDisplayScaler()
{
	return 1.0;
}

XmlElement* AmbiSpeaker::getAsXmlElement(String tagName)
{
	XmlElement* element = getBaseXmlElement(tagName);
	filterInfo.writeToXmlElement(element);
	element->setAttribute(XML_ATTRIBUTE_POINT_FILTER_BYPASS, filterBypass);
	return element;
}
