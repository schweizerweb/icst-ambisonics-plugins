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

const float EncoderConstants::EncodingModeMin = 0;
const float EncoderConstants::EncodingModeMax = 4;  // has to be changed if enum DistanceEncodingParams::EncodingMode changes!
