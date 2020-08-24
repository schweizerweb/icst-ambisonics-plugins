/*
  ==============================================================================

    DelayHelper.cpp
    Created: 17 Dec 2017 11:42:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DelayHelper.h"

double DelayHelper::getTotalDelayMs(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint) const
{
	return pPoint->getPoint()->getDistance() * pAmbiSettings->getDistanceScaler() * SOUND_SPEED_MS_PER_M;
}

double DelayHelper::getDelayCompensationMs(AmbiSettings* pAmbiSettings, double maxNormalizedDistance, AmbiPoint* pPoint) const
{
	return (maxNormalizedDistance - pPoint->getPoint()->getDistance())
		* pAmbiSettings->getDistanceScaler()
		* SOUND_SPEED_MS_PER_M;
}

int DelayHelper::getDelayCompensationSamples(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate)
{
	return int((maxNormalizedDistance - pPoint->getPoint()->getDistance())
		* pAmbiSettings->getDistanceScaler()
		* SOUND_SPEED_S_PER_M
		* samplingRate);
}

int DelayHelper::getDelaySamples(double distance, double samplingRate)
{
	return int(distance * SOUND_SPEED_S_PER_M * samplingRate);
}
