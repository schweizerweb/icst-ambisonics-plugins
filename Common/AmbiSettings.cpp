/*
  ==============================================================================

    AmbiSettings.cpp
    Created: 9 Oct 2017 4:02:43pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiSettings.h"

AmbiSettings::AmbiSettings(): distanceScaler(DEFAULT_DISTANCE_SCALER)
{
}

AmbiSettings::AmbiSettings(double distanceScaler): distanceScaler(distanceScaler)
{
}

double AmbiSettings::getDistanceScaler() const
{
	return distanceScaler;
}

void AmbiSettings::setDistanceScaler(double newDistanceScaler)
{
	distanceScaler = newDistanceScaler;
}
