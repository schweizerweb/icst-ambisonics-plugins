/*
  ==============================================================================

    EncoderConstants.cpp
    Created: 1 Feb 2020 8:20:29pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "EncoderConstants.h"

const float EncoderConstants::UnitCircleRadiusMin = 0.01f;
const float EncoderConstants::UnitCircleRadiusMax = 1.0f;
const float EncoderConstants::UnitCircleRadiusResolution = 0.0001f;

const float EncoderConstants::DbUnitMin = 0.01f;
const float EncoderConstants::DbUnitMax = 100.0f;
const float EncoderConstants::DbUnitResolution = 0.0001f;

const float EncoderConstants::DistanceAttenuationMin = 0.01f;
const float EncoderConstants::DistanceAttenuationMax = 20.0f;
const float EncoderConstants::DistanceAttenuationResolution = 0.0001f;

const float EncoderConstants::CenterCurveMin = 0.0f;
const float EncoderConstants::CenterCurveMax = 1.0f;
const float EncoderConstants::CenterCurveResolution = 0.0001f;

const float EncoderConstants::AdvancedFactorMin = 0.0f;
const float EncoderConstants::AdvancedFactorMax = 5.0f;
const float EncoderConstants::AdvancedFactorResolution = 0.0001f;

const float EncoderConstants::AdvancedExponentMin = 0.0f;
const float EncoderConstants::AdvancedExponentMax = 20.0f;
const float EncoderConstants::AdvancedExponentResolution = 0.0001f;

const float EncoderConstants::MasterGainMin = -12.0f;
const float EncoderConstants::MasterGainMax = 12.0f;
const float EncoderConstants::MasterGainResolution = 0.1f;

const float EncoderConstants::AirAbsorbtionIntensityMin = 0.1f;
const float EncoderConstants::AirAbsorbtionIntensityMax = 100.0f;
const float EncoderConstants::AirAbsorbtionIntensityResolution = 0.1f;

const StringArray EncoderConstants::encodingModeStrings = {"None", "Standard", "Advanced", "Exponential", "Inverse Proportional"};
const StringArray EncoderConstants::airAbsorbtionModeStrings = {"Off", "Low-Pass"};
    
