/*
  ==============================================================================

    AmbiSpeaker.h
    Created: 30 Oct 2019 11:40:30am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"
#include "FilterBankInfo.h"


#define XML_ATTRIBUTE_POINT_FILTER_BYPASS "ByPass"

class AmbiSpeaker : public AmbiPoint
{
public:
	
	virtual ~AmbiSpeaker() = default;
	AmbiSpeaker(AmbiSpeaker* other, bool copyImage = false);
	AmbiSpeaker(String id, Point3D<double> point, String name, Colour color = Colour(), double gain = 1.0);
	AmbiSpeaker(XmlElement* element);

	FilterBankInfo* getFilterInfo();
    bool getFilterBypass();
    void setFilterBypass(bool byPass);
    
	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;

    bool allowMute() const override { return true; }
    bool allowSolo() const override { return false; }
    
private:
	FilterBankInfo filterInfo;
    bool filterBypass;
};
