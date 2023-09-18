/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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

    if(initRequired || !approximatelyEqual(pParams->getAirAbsorbtionIntensity(), currentIntensity) || !approximatelyEqual(distance, currentDistance) || !approximatelyEqual(pSpec->sampleRate, currentSampleRate))
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
