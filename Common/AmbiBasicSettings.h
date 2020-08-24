/*
  ==============================================================================

    AmbiBasicSettings.h
    Created: 18 Nov 2019 9:35:45am
    Author:  chris

  ==============================================================================
*/

#pragma once
#define SOUND_SPEED_M_PER_S	343.0
#define SOUND_SPEED_MS_PER_M (1000.0 / SOUND_SPEED_M_PER_S)
#define SOUND_SPEED_S_PER_M (1.0 / SOUND_SPEED_M_PER_S)

#define DEFAULT_DISTANCE_SCALER	1.0

class AmbiBasicSettings
{
public:
	AmbiBasicSettings(double distanceScaler);

	double getDistanceScaler() const;
	void setDistanceScaler(double newDistanceScaler);

private:
	double distanceScaler;
};
