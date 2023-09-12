/*
  ==============================================================================

    AmbiSpeaker.cpp
    Created: 30 Oct 2019 11:40:30am
    Author:  chris

  ==============================================================================
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
