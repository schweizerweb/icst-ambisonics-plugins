/*
  ==============================================================================

    DistanceEncodingParameterSet.cpp
    Created: 1 Feb 2020 4:10:47pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "DistanceEncodingParameterSet.h"

juce_ImplementSingleton(DistanceEncodingParameterSet);

void DistanceEncodingParameterSet::notifyUnitCircle(float unitCircle)
{
    if (pUnitCircle != nullptr) { pUnitCircle->setUnscaledValue(float(unitCircle)); }
}

void DistanceEncodingParameterSet::notifyMode(int mode)
{
    if (pEncodingMode != nullptr) { pEncodingMode->setUnscaledValue(int(mode)); }
}
