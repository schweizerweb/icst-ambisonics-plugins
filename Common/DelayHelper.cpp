/*
  ==============================================================================

    DelayHelper.cpp
    Created: 17 Dec 2017 11:42:36pm
    Author:  chris

  ==============================================================================
*/

#include "DelayHelper.h"

double DelayHelper::getMaxNormalizedDistance(Array<AmbiPoint>* pPoints) const
{
	double maxDist = 0.0;
	for (int i = 0; i < pPoints->size(); i++)
	{
		maxDist = jmax(maxDist, pPoints->getReference(i).getPoint()->getDistance());
	}

	return maxDist;
}

double DelayHelper::getTotalDelayMs(AmbiSettings* pAmbiSettings, Array<AmbiPoint>* pPoints, int wantedIndex)
{
	return pPoints->getReference(wantedIndex).getPoint()->getDistance() * pAmbiSettings->getDistanceScaler() * SOUND_SPEED_MS_PER_M;
}

double DelayHelper::getDelayCompensationMs(AmbiSettings* pAmbiSettings, Array<AmbiPoint>* pPoints, int wantedIndex) const
{
	return (getMaxNormalizedDistance(pPoints) - pPoints->getReference(wantedIndex).getPoint()->getDistance())
		* pAmbiSettings->getDistanceScaler()
		* SOUND_SPEED_MS_PER_M;
}

int DelayHelper::getDelayCompensationSamples(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate)
{
	return (maxNormalizedDistance - pPoint->getPoint()->getDistance())
		* pAmbiSettings->getDistanceScaler()
		* SOUND_SPEED_S_PER_M
		* samplingRate;
}
