/*
  ==============================================================================

    Constants.cpp
    Created: 27 Sep 2018 1:25:28pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "Constants.h"
#include "JuceHeader.h"

const float Constants::AzimuthRadMin = float(0);
const float Constants::AzimuthRadMax = float(2.0*PI);
const float Constants::ElevationRadMin = float(-PI / 2.0);
const float Constants::ElevationRadMax = float(PI / 2.0);
const float Constants::DistanceMin = 0.0f;
const float Constants::DistanceMax = 1000000.0f;
const double Constants::AzimuthGradMin = 0.0;
const double Constants::AzimuthGradMax = 360.0;
const double Constants::ElevationGradMin = -90.0;
const double Constants::ElevationGradMax = 90.0;
const double Constants::GainDbMin = -128.0;
const double Constants::GainDbMax = 12.0;
const float Constants::XMin = -1000000.0;
const float Constants::XMax = 1000000.0;
const float Constants::YMin = -1000000.0;
const float Constants::YMax = 1000000.0;
const float Constants::ZMin = -1000000.0;
const float Constants::ZMax = 1000000.0;

const double Constants::CompressedMin = -PI/2;
const double Constants::CompressedMax = PI/2;

double Constants::RadToGrad(double rad)
{
	return rad * 180.0 / PI;
}

double Constants::GradToRad(double grad)
{
	return grad * PI / 180.0;
}

int Constants::NormalizeAzimuthGrad(int grad)
{
	return (grad % 360 + 360) % 360;
}

double Constants::GainFactorToDb(double factor)
{
     return 10.0 * log10(factor);
}

double Constants::GainDbToFactor(double db)
{
    return pow(10.0, 0.1 * db);
}

bool Constants::isDevelopmentVersion()
{
	return String(ProjectInfo::versionString) == "0.0.0" || !String(ProjectInfo::versionString).containsOnly("0123456789.");
}
