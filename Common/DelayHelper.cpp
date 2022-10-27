/*
  ==============================================================================

    DelayHelper.cpp
    Created: 17 Dec 2017 11:42:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DelayHelper.h"
#include "Constants.h"

double DelayHelper::getTotalDelayMs(double distance) const
{
	return distance * SOUND_SPEED_MS_PER_M;
}

double DelayHelper::getDelayCompensationMs(double distance, double maxNormalizedDistance) const
{
	return (maxNormalizedDistance - distance)
		* SOUND_SPEED_MS_PER_M;
}

int DelayHelper::getDelayCompensationSamples(double distance, double maxNormalizedDistance, double samplingRate)
{
	return int((maxNormalizedDistance - distance)
		* SOUND_SPEED_S_PER_M
		* samplingRate);
}

float DelayHelper::getDelaySamples(double distance, double samplingRate)
{
	return float(distance * SOUND_SPEED_S_PER_M * samplingRate);
}
