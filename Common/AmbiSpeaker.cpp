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

AmbiSpeaker::AmbiSpeaker(String id, Point3D<double> point, String name, Colour color, double gain) : 
	AmbiPoint(id, point, name, color, gain), filterBypass(DEFAULT_BYPASS_FILTER)
{
}

AmbiSpeaker::AmbiSpeaker(XmlElement* element) :
	AmbiPoint(element)
{
	filterInfo.filterType = FilterInfo::FilterType(element->getIntAttribute(XML_ATTRIBUTE_POINT_FILTER_TYPE));
	filterInfo.cutOffFrequencyHz = float(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_FILTER_FREQUENCY, DEFAULT_FREQUENCY));
	filterInfo.qValue = float(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_FILTER_Q, DEFAULT_Q));
    filterInfo.gainFactor = float(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_FILTER_GAIN_FACTOR, DEFAULT_GAIN_FACTOR));
    filterBypass = bool(element->getBoolAttribute(XML_ATTRIBUTE_POINT_FILTER_BYPASS, DEFAULT_BYPASS_FILTER));
}

FilterInfo* AmbiSpeaker::getFilterInfo()
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
	element->setAttribute(XML_ATTRIBUTE_POINT_FILTER_TYPE, int(filterInfo.filterType));
	element->setAttribute(XML_ATTRIBUTE_POINT_FILTER_FREQUENCY, double(filterInfo.cutOffFrequencyHz));
	element->setAttribute(XML_ATTRIBUTE_POINT_FILTER_Q, double(filterInfo.qValue));
    element->setAttribute(XML_ATTRIBUTE_POINT_FILTER_GAIN_FACTOR, double(filterInfo.gainFactor));
    element->setAttribute(XML_ATTRIBUTE_POINT_FILTER_BYPASS, filterBypass);
	return element;
}
