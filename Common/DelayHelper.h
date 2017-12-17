/*
  ==============================================================================

    DelayHelper.h
    Created: 17 Dec 2017 11:42:36pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AmbiPoint.h"
#include "AmbiSettings.h"

class DelayHelper
{
public:
	double getMaxNormalizedDistance(Array<AmbiPoint>* pPoints) const;
	static double getTotalDelayMs(AmbiSettings* pAmbiSettings, Array<AmbiPoint>* pPoints, int wantedIndex);
	double getDelayCompensationMs(AmbiSettings* pAmbiSettings, Array<AmbiPoint>* pPoints, int wantedIndex) const;
	static int getDelayCompensationSamples(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate);
};
