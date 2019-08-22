/*
  ==============================================================================

    SliderRange.h
    Created: 15 Dec 2017 10:25:37pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
class SliderRange
{
public:
	SliderRange() : SliderRange(0.0, 0.0, 0.0){}
	SliderRange(double minRange, double maxRange, double intervalRange)
	{
		min = minRange;
		max = maxRange;
		interval = intervalRange;
	}
	double min;
	double max;
	double interval;
};