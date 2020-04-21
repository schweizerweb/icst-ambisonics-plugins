/*
  ==============================================================================

    AmbiSpeaker.h
    Created: 30 Oct 2019 11:40:30am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"
#include "FilterInfo.h"

#define XML_ATTRIBUTE_POINT_FILTER_TYPE "FilterType"
#define XML_ATTRIBUTE_POINT_FILTER_FREQUENCY "FilterFrequency"
#define XML_ATTRIBUTE_POINT_FILTER_Q "FilterQ"
#define XML_ATTRIBUTE_POINT_FILTER_GAIN_FACTOR "GainFactor"

class AmbiSpeaker : public AmbiPoint
{
public:
	
	virtual ~AmbiSpeaker() = default;
	AmbiSpeaker(AmbiSpeaker* other, bool copyImage = false);
	AmbiSpeaker(String id, Point3D<double> point, String name, Colour color = Colour(), double gain = 1.0);
	AmbiSpeaker(XmlElement* element);

	FilterInfo* getFilterInfo();

	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;

private:
	FilterInfo filterInfo;
};
