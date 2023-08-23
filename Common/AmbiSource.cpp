/*
  ==============================================================================

    AmbiSource.cpp
    Created: 30 Oct 2019 11:39:59am
    Author:  chris

  ==============================================================================
*/

#include "AmbiSource.h"

AmbiSource::~AmbiSource()
{
}

AmbiSource::AmbiSource() : AmbiPoint(), rms(0.0f)
{
}

AmbiSource::AmbiSource(AmbiSource* other, bool copyImage) : AmbiPoint(other, copyImage), rms(other->rms)
{
}

AmbiSource::AmbiSource(String id, Point3D<double> point, String name, Colour color) : AmbiPoint(id, point, name, color), rms(0.0f)
{
}

AmbiSource::AmbiSource(XmlElement* _element, AudioParameterSet _audioParams) : AmbiPoint(_element, _audioParams), rms(0.0f)
{
}

AmbiSource::AmbiSource(AudioParameterSet _audioParams, String name) : AmbiPoint(_audioParams), rms(0.0f)
{
    setName(name);
}

void AmbiSource::setRms(float newRmsLevel, bool onlyIfGreater)
{
	if (!onlyIfGreater || newRmsLevel > rms)
		rms = newRmsLevel;
}

float AmbiSource::getRms() const
{
	return rms;
}

float AmbiSource::getDisplayScaler()
{
	return 1.0f + 10.0f * rms;
}

XmlElement* AmbiSource::getAsXmlElement(String tagName)
{
	return getBaseXmlElement(tagName);
}
