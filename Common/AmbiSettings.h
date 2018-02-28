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
#define MAX_AMBISONICS_ORDER 7
#define NB_OF_AMBISONICS_GAINS (MAX_AMBISONICS_ORDER + 1)
#define NB_OF_AMBISONICS_CHANNELS (NB_OF_AMBISONICS_GAINS * NB_OF_AMBISONICS_GAINS)
#define CURRENT_AMBISONICS_ORDER_NB_OF_GAINS	int(sqrt(JucePlugin_MaxNumInputChannels))
#define CURRENT_AMBISONICS_ORDER	(CURRENT_AMBISONICS_ORDER_NB_OF_GAINS - 1)
class AmbiSettings
{
public:
	AmbiSettings();
	AmbiSettings(double distanceScaler, bool directionFlip);

	double getDistanceScaler() const;
	void setDistanceScaler(double newDistanceScaler);
	double getAmbiChannelWeight(int ambiChannel);
	double* getAmbiOrderWeightPointer();
	bool getDirectionFlip() const;
	void setDirectionFlip(bool flip);

private:
	double distanceScaler;
	bool directionFlip;
	double* ambiChannelWeights[NB_OF_AMBISONICS_CHANNELS];
	double ambiOrderWeights[NB_OF_AMBISONICS_GAINS];
};