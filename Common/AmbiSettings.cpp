/*
  ==============================================================================

    AmbiSettings.cpp
    Created: 9 Oct 2017 4:02:43pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiSettings.h"

AmbiSettings::AmbiSettings(): AmbiSettings(DEFAULT_DISTANCE_SCALER, false)
{
	
}

AmbiSettings::AmbiSettings(double distanceScaler, bool directionFlip): distanceScaler(distanceScaler), directionFlip(directionFlip)
{
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		ambiOrderWeights[i] = 1.0;

	// create mapping table
	int order = 0;
	for (int i = 0; i < NB_OF_AMBISONICS_CHANNELS; i++)
	{
		if (i == (order + 1)*(order + 1))
			order++;
		ambiChannelWeights[i] = &ambiOrderWeights[order];
	}
}

double AmbiSettings::getDistanceScaler() const
{
	return distanceScaler;
}

void AmbiSettings::setDistanceScaler(double newDistanceScaler)
{
	distanceScaler = newDistanceScaler;
}

double AmbiSettings::getAmbiChannelWeight(int ambiChannel)
{
	return *ambiChannelWeights[ambiChannel];
}

double* AmbiSettings::getAmbiOrderWeightPointer()
{
	return ambiOrderWeights;
}

bool AmbiSettings::getDirectionFlip() const
{
	return directionFlip;
}

void AmbiSettings::setDirectionFlip(bool flip)
{
	directionFlip = flip;
}
