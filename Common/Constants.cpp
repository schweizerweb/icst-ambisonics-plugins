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
const double Constants::AzimuthGradMin = 0.0;
const double Constants::AzimuthGradMax = 360.0;
const double Constants::ElevationGradMin = -90.0;
const double Constants::ElevationGradMax = 90.0;
const double Constants::GainDbMin = -88.0;
const double Constants::GainDbMax = 12.0;

const float Constants::CompressedMin = -1.0f;
const float Constants::CompressedMax = 1.0f;

const float Constants::StretchMin = 0.0;
const float Constants::StretchMax = 100.0;
const int Constants::audioParamVersion = 1;

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

bool Constants::isDevelopmentVersion()
{
	return String(ProjectInfo::versionString).startsWith("0") || String(ProjectInfo::versionString).matchesWildcard("*.*.*.*", true);
}

bool Constants::isNonVisibleVersionPrerelease()
{
    return String(ProjectInfo::versionString).endsWith("-t");
}
