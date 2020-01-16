/*
  ==============================================================================

    AmbiSettings.cpp
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiSettings.h"

AmbiSettings::AmbiSettings(): AmbiSettings(DEFAULT_DISTANCE_SCALER, false, DEFAULT_POINT_SCALER)
{
	
}

AmbiSettings::AmbiSettings(double distanceScaler, bool directionFlip, double pointScaler) : AmbiBasicSettings(distanceScaler, directionFlip, pointScaler)
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

double AmbiSettings::getAmbiChannelWeight(int ambiChannel)
{
	return *ambiChannelWeights[ambiChannel];
}

double* AmbiSettings::getAmbiOrderWeightPointer()
{
	return ambiOrderWeights;
}
