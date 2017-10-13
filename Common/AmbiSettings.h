/*
  ==============================================================================

    AmbiSettings.h
    Created: 9 Oct 2017 4:02:43pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define SOUND_SPEED_M_PER_MS	0.33
#define DEFAULT_DISTANCE_SCALER	1.0
#define MAX_NB_AMBISONICS_CHANNELS 64	// up to 7th order
class AmbiSettings
{
public:
	AmbiSettings();
	AmbiSettings(double distanceScaler);

	double getDistanceScaler() const;
	void setDistanceScaler(double newDistanceScaler);
	double* getAmbiChannelWeightPointer();

private:
	double distanceScaler;
	double ambiChannelWeights[MAX_NB_AMBISONICS_CHANNELS];
};