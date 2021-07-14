/*
  ==============================================================================

    DelayHelper.cpp
    Created: 17 Dec 2017 11:42:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DelayHelper.h"
#include "AmbiBasicSettings.h"

double DelayHelper::getTotalDelayMs(AmbiPoint* pPoint) const
{
	return pPoint->getPoint()->getDistance() * SOUND_SPEED_MS_PER_M;
}

double DelayHelper::getDelayCompensationMs(double maxNormalizedDistance, AmbiPoint* pPoint) const
{
	return (maxNormalizedDistance - pPoint->getPoint()->getDistance())
		* SOUND_SPEED_MS_PER_M;
}

int DelayHelper::getDelayCompensationSamples(AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate)
{
	return int((maxNormalizedDistance - pPoint->getPoint()->getDistance())
		* SOUND_SPEED_S_PER_M
		* samplingRate);
}

float DelayHelper::getDelaySamples(double distance, double samplingRate)
{
	return float(distance * SOUND_SPEED_S_PER_M * samplingRate);
}
