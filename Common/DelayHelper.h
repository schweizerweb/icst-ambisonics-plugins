/*
  ==============================================================================

    DelayHelper.h
    Created: 17 Dec 2017 11:42:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"
#include "AmbiSettings.h"

class DelayHelper
{
public:
	double getTotalDelayMs(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint) const;
	double getDelayCompensationMs(AmbiSettings* pAmbiSettings, double maxNormalizedDistance, AmbiPoint* pPoint) const;
	static int getDelayCompensationSamples(AmbiSettings* pAmbiSettings, AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate);
};
