/*
  ==============================================================================

    AirAbsorbtionFilter.cpp
    Created: 28 Dec 2020 1:50:25pm
    Author:  chris

  ==============================================================================
*/

#include "AirAbsorbtionFilter.h"

AirAbsorbtionFilter::AirAbsorbtionFilter(): currentMode(EncoderConstants::Off), currentIntensity(1),
                                              currentDistance(0), currentSampleRate(0)
{
}

AirAbsorbtionFilter::~AirAbsorbtionFilter()
{
}

bool AirAbsorbtionFilter::checkFilter(DistanceEncodingParams* pParams, double distance, dsp::ProcessSpec* pSpec)
{
    // checks the filter settings and updates the filter in case values changed
    // returns true if filtering has to be performed

    bool initRequired = false;
    EncoderConstants::AirAbsorbtionMode newMode = pParams->getAirAbsorbtionMode();
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

    if(initRequired || pParams->getAirAbsorbtionIntensity() != currentIntensity || distance != currentDistance || pSpec->sampleRate != currentSampleRate)
    {
        currentIntensity = pParams->getAirAbsorbtionIntensity();
        currentDistance = distance;
        currentSampleRate = pSpec->sampleRate;

        // simplified air absorption formula by Martin Neukom
        float frequency = (float)jlimit(10.0, currentSampleRate / 2.0, 20000 * exp(-0.1 * currentDistance * currentIntensity));
        
        if (initRequired)
        {
            filter.prepare(*pSpec);
            filter.reset();
        }

        filter.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(currentSampleRate, frequency);
    }

    return true;
}

float AirAbsorbtionFilter::processSample(float sample)
{
    return filter.processSample(sample);
}
