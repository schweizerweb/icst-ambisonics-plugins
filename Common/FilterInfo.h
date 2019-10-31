/*
  ==============================================================================

    FilterInfo.h
    Created: 30 Oct 2019 4:28:04pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#define DEFAULT_Q	1.0
#define DEFAULT_FREQUENCY 200

class FilterInfo
{
public:
	FilterInfo(): filterType(None), cutOffFrequencyHz(DEFAULT_FREQUENCY), qValue(DEFAULT_Q)
	{
	}

	enum FilterType { None, LowPass, BandPass, HighPass } filterType;
	float cutOffFrequencyHz;
	float qValue;
	void copyFrom(FilterInfo* info);
};
