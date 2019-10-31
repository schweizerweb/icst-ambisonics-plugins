/*
  ==============================================================================

    AmbiSpeaker.cpp
    Created: 30 Oct 2019 11:40:30am
    Author:  chris

  ==============================================================================
*/

#include "AmbiSpeaker.h"

void FilterInfo::copyFrom(FilterInfo* info)
{
	filterType = info->filterType;
	cutOffFrequencyHz = info->cutOffFrequencyHz;
	qValue = info->qValue;
}

AmbiSpeaker::AmbiSpeaker(AmbiSpeaker* other, bool copyImage) : 
	AmbiPoint(other, copyImage), filterInfo(other->filterInfo)
{
}

AmbiSpeaker::AmbiSpeaker(String id, Point3D<double> point, String name, Colour color, double gain) : 
	AmbiPoint(id, point, name, color, gain)
{
}

AmbiSpeaker::AmbiSpeaker(XmlElement* element) :
	AmbiPoint(element)
{
	filterInfo.filterType = FilterInfo::FilterType(element->getIntAttribute(XML_ATTRIBUTE_POINT_FILTER_TYPE));
	filterInfo.cutOffFrequencyHz = float(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_FILTER_FREQUENCY, DEFAULT_FREQUENCY));
	filterInfo.qValue = float(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_FILTER_Q, DEFAULT_Q));
}

FilterInfo* AmbiSpeaker::getFilterInfo()
{
	return &filterInfo;
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
	return element;
}