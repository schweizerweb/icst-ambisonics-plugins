/*
  ==============================================================================

    AirAbsorbationFilter.cpp
    Created: 28 Dec 2020 1:50:25pm
    Author:  chris

  ==============================================================================
*/

#include "AirAbsorbationFilter.h"

AirAbsorbationFilter::AirAbsorbationFilter(): currentMode(EncoderConstants::Off), currentIntensity(1),
                                              currentDistance(0), currentSampleRate(0)
{
}

AirAbsorbationFilter::~AirAbsorbationFilter()
{
}

bool AirAbsorbationFilter::checkFilter(DistanceEncodingParams* pParams, double distance, dsp::ProcessSpec* pSpec)
{
    // checks the filter settings and updates the filter in case values changed
    // returns true if filtering has to be performed

    bool initRequired = false;
    EncoderConstants::AirAbsorbationMode newMode = pParams->getAirAbsorbationMode();
    if(newMode != currentMode)
    {
        currentMode = newMode;
        if(currentMode != EncoderConstants::Off)
        {
            initRequired = true;
        }
    }

    if (currentMode == EncoderConstants::Off)
        return false;

    if(initRequired || pParams->getAirAbsorbationIntensity() != currentIntensity || distance != currentDistance || pSpec->sampleRate != currentSampleRate)
    {
        currentIntensity = pParams->getAirAbsorbationIntensity();
        currentDistance = distance;
        currentSampleRate = pSpec->sampleRate;
        float frequency = float(jmax(currentSampleRate / 2.0 - currentDistance * currentIntensity * 5000.0, 20.0));
        float q = 1.0f;

        if (initRequired)
        {
            filter.prepare(*pSpec);
            filter.reset();
        }

        filter.coefficients = dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, frequency, q);
    }

    return true;
}

float AirAbsorbationFilter::processSample(float sample)
{
    return filter.processSample(sample);
}
