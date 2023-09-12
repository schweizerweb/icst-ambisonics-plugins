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
	AmbiSource(String _id, Point3D<double> _point, String _name, Colour _color);
	AmbiSource(XmlElement* _element, AudioParameterSet _audioParams);
    AmbiSource(AudioParameterSet _audioParams, String _name);

	void setRms(float newRmsLevel, bool onlyIfGreater = false);
	float getRms() const;

	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;

    bool allowMute() const override { return true; }
    bool allowSolo() const override { return true; }
    
private:
	float rms;
};
