/*
  ==============================================================================

    DelayHelper.h
    Created: 17 Dec 2017 11:42:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"

class DelayHelper
{
public:
	double getTotalDelayMs(AmbiPoint* pPoint) const;
	double getDelayCompensationMs(double maxNormalizedDistance, AmbiPoint* pPoint) const;
	static int getDelayCompensationSamples(AmbiPoint* pPoint, double maxNormalizedDistance, double samplingRate);
	static float getDelaySamples(double distance, double samplingRate);
};
