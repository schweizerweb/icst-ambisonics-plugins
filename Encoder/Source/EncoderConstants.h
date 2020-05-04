/*
  ==============================================================================

    EncoderConstants.h
    Created: 1 Feb 2020 8:18:59pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class EncoderConstants
{
public:
    const static float UnitCircleRadiusMin;
    const static float UnitCircleRadiusMax;
    const static float UnitCircleRadiusResolution;
    
    const static float DbUnitMin;
    const static float DbUnitMax;
    const static float DbUnitResolution;
    
    const static float DistanceAttenuationMin;
    const static float DistanceAttenuationMax;
    const static float DistanceAttenuationResolution;
    
    const static float CenterCurveMin;
    const static float CenterCurveMax;
    const static float CenterCurveResolution;
    
    const static float AdvancedFactorMin;
    const static float AdvancedFactorMax;
    const static float AdvancedFactorResolution;
    
    const static float AdvancedExponentMin;
    const static float AdvancedExponentMax;
    const static float AdvancedExponentResolution;
    
    // make sure enum EncodingMode and StringArray encodingModeString are in sync!
    enum EncodingMode { None = 0, Standard = 1, Advanced = 2, Exponential = 3, InverseProportional = 4 };
    const static StringArray encodingModeStrings;
    
};
