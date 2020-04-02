/*
  ==============================================================================

    AmbiBasicSettings.cpp
    Created: 18 Nov 2019 9:35:45am
    Author:  chris

  ==============================================================================
*/

#include "AmbiBasicSettings.h"
#include "Constants.h"
#include "Globals.h"

AmbiBasicSettings::AmbiBasicSettings(double distanceScaler): distanceScaler(distanceScaler)
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
