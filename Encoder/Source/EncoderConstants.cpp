/*
  ==============================================================================

    EncoderConstants.cpp
    Created: 1 Feb 2020 8:20:29pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "EncoderConstants.h"

const float EncoderConstants::UnitCircleRadiusMin = float(0.01);
const float EncoderConstants::UnitCircleRadiusMax = float(1.0);
const float EncoderConstants::UnitCircleRadiusResolution = float(0.01);

const float EncoderConstants::DbUnitMin = float(0.01);
const float EncoderConstants::DbUnitMax = float(100);
const float EncoderConstants::DbUnitResolution = float(0.01);

const float EncoderConstants::DistanceAttenuationMin = 0.01;
const float EncoderConstants::DistanceAttenuationMax = 20;
const float EncoderConstants::DistanceAttenuationResolution = 0.01;

const float EncoderConstants::CenterCurveMin = 0;
const float EncoderConstants::CenterCurveMax = 1;
const float EncoderConstants::CenterCurveResolution = 0.0001;

const float EncoderConstants::AdvancedFactorMin = 0;
const float EncoderConstants::AdvancedFactorMax = 5;
const float EncoderConstants::AdvancedFactorResolution = 0.01;

const float EncoderConstants::AdvancedExponentMin = 0;
const float EncoderConstants::AdvancedExponentMax = 20;
const float EncoderConstants::AdvancedExponentResolution = 0.01;

const StringArray EncoderConstants::encodingModeStrings = {"None", "Standard", "Advanced", "Exponential", "Inverse Proportional"};
    
