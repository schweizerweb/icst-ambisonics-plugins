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
    
    const static float ExperimentalFactorMin;
    const static float ExperimentalFactorMax;
    const static float ExperimentalFactorResolution;
    
    const static float ExperimentalPowerMin;
    const static float ExperimentalPowerMax;
    const static float ExperimentalPowerResolution;
    
    // make sure enum EncodingMode and StringArray encodingModeString are in sync!
    enum EncodingMode { None, Standard, Exponential, InverseProportional, Experimental };
    const static StringArray encodingModeStrings;
    
};
