/*
  ==============================================================================

    AmbiBasicSettings.cpp
    Created: 18 Nov 2019 9:35:45am
    Author:  chris

  ==============================================================================
*/

#include "AmbiBasicSettings.h"

AmbiBasicSettings::AmbiBasicSettings(double distanceScaler, bool directionFlip, double pointScaler): distanceScaler(distanceScaler), directionFlip(directionFlip), pointScaler(pointScaler)
{
}

double AmbiBasicSettings::getDistanceScaler() const
{
	return distanceScaler;
}

void AmbiBasicSettings::setDistanceScaler(double newDistanceScaler)
{
	distanceScaler = newDistanceScaler;
}

bool AmbiBasicSettings::getDirectionFlip() const
{
	return directionFlip;
}

void AmbiBasicSettings::setDirectionFlip(bool flip)
{
	directionFlip = flip;
}
