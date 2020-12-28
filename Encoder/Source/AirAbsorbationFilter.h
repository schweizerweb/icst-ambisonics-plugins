/*
  ==============================================================================

    AirAbsorbationFilter.h
    Created: 28 Dec 2020 1:50:25pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "DistanceEncodingParams.h"

class AirAbsorbationFilter
{
public:
    AirAbsorbationFilter();
    ~AirAbsorbationFilter();
    bool checkFilter(DistanceEncodingParams* pParams, double distance, dsp::ProcessSpec* pSpec);
    float processSample(float sample);

private:
    dsp::IIR::Filter<float> filter;
    EncoderConstants::AirAbsorbationMode currentMode;
    float currentIntensity;
    double currentDistance;
    double currentSampleRate;
};