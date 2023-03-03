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
	double getTotalDelayMs(double distance) const;
	double getDelayCompensationMs(double distance, double maxNormalizedDistance) const;
	static int getDelayCompensationSamples(double distance, double maxNormalizedDistance, double samplingRate);
	static float getDelaySamples(double distance, double samplingRate);
};
