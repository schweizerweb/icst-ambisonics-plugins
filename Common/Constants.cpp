/*
  ==============================================================================

    Constants.cpp
    Created: 27 Sep 2018 1:25:28pm
    Author:  chris

  ==============================================================================
*/

#include "Constants.h"

const float Constants::AzimuthRadMin = float(0);
const float Constants::AzimuthRadMax = float(2.0*PI);
const float Constants::ElevationRadMin = float(-PI / 2.0);
const float Constants::ElevationRadMax = float(PI / 2.0);
const float Constants::DistanceMin = 0.0f;
const float Constants::DistanceMax = 1.0f;
const double Constants::AzimuthGradMin = 0.0;
const double Constants::AzimuthGradMax = 360.0;
const double Constants::ElevationGradMin = -90.0;
const double Constants::ElevationGradMax = 90.0;
const double Constants::GainDbMin = -128.0;
const double Constants::GainDbMax = 12.0;
const float Constants::XMin = -1.0;
const float Constants::XMax = 1.0;
const float Constants::YMin = -1.0;
const float Constants::YMax = 1.0;
const float Constants::ZMin = -1.0;
const float Constants::ZMax = 1.0;


double Constants::RadToGrad(double rad)
{
	return rad * 180.0 / PI;
}

double Constants::GradToRad(double grad)
{
	return grad * PI / 180.0;
}
