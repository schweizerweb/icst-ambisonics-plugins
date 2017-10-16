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
class AmbiSettings
{
public:
	AmbiSettings();
	AmbiSettings(double distanceScaler);

	double getDistanceScaler() const;
	void setDistanceScaler(double newDistanceScaler);
	double getAmbiChannelWeight(int ambiChannel);
	double* getAmbiOrderWeightPointer();
	
private:
	double distanceScaler;
	double* ambiChannelWeights[NB_OF_AMBISONICS_CHANNELS];
	double ambiOrderWeights[NB_OF_AMBISONICS_GAINS];
};