/*
  ==============================================================================

    AmbiSpeaker.h
    Created: 30 Oct 2019 11:40:30am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"

class AmbiSpeaker : public AmbiPoint
{
public:
	virtual ~AmbiSpeaker() = default;
	AmbiSpeaker(AmbiSpeaker* other, bool copyImage = false);
	AmbiSpeaker(String id, Point3D<double> point, String name, Colour color = Colour(), double gain = 1.0);
	AmbiSpeaker(XmlElement* element);

	void setSubwooferFlag(bool flag);
	bool getSubwooferFlag();

	float getDisplayScaler() override;

private:
	bool isSubwoofer;
};
