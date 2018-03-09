/*
  ==============================================================================

    DelayHelper.h
    Created: 17 Dec 2017 11:42:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AmbiPoint.h"
#include "AmbiSettings.h"

class DelayHelper
{
public:
	double getMaxNormalizedDistance(OwnedArray<AmbiPoint>* pPoints) const;
	static double getTotalDelayMs(AmbiSettings* pAmbiSettings, OwnedArray<AmbiPoint>* pPoints, int wantedIndex);
	double getDelayCompensationMs(AmbiSettings* pAmbiSettings, OwnedArray<AmbiPoint>* pPoints, int wantedIndex) const;
	static int getDelayCompensationSamples(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate);
};
