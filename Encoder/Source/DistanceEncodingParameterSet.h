/*
  ==============================================================================

    DistanceEncodingParameterSet.h
    Created: 1 Feb 2020 3:22:23pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "../../Common/AudioParameterFloatAmbiAbs.h"
#include "../../Common/AudioParameterIntAmbiAbs.h"

class DistanceEncodingParameterSet
{
public:
    juce_DeclareSingleton(DistanceEncodingParameterSet, false);

    AudioParameterFloatAmbiAbs* pEncodingMode = nullptr;
    AudioParameterFloatAmbiAbs* pUnitCircle = nullptr;
    
    void notifyMode(int mode);
    void notifyUnitCircle(float unitCircle);
};

