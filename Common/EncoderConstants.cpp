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
    
