/*
  ==============================================================================

    AmbiSource.h
    Created: 30 Oct 2019 11:39:59am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"

class AmbiSource : public AmbiPoint
{
public:
	virtual ~AmbiSource();
	AmbiSource();
	AmbiSource(AmbiSource* other, bool copyImage = false);
	AmbiSource(String id, Point3D<double> point, String name, Colour color);
	AmbiSource(XmlElement* element, AudioParameterSet audioParams);
    AmbiSource(AudioParameterSet audioParams, String name);

	void setRms(float newRmsLevel, bool onlyIfGreater = false);
	float getRms() const;

	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;

private:
	float rms;
};
