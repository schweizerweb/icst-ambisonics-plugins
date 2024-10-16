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



#include "Constants.h"
#include "BuildInfo.h"
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

String Constants::getUiVersionString(bool addLeadingSpace)
{
    if(Constants::isNonVisibleVersionPrerelease())
    {
        return "";
    }

    String versionString;
    if(String(ProjectInfo::versionString).startsWith("0.0.0."))
    {
#if BUILD_NUMBER > 0
        versionString = "Dev" + String(BUILD_NUMBER);
#else
        versionString = "self-built";
#endif
    }
    else
    {
        versionString = String(ProjectInfo::versionString);
    }
    
    return addLeadingSpace
            ? (" " + versionString)
            : versionString;
}

String Constants::getBuildInfo()
{
    return String(COMMIT_ID) == String("")
        ? ""
        : ("Version: " + getUiVersionString(false) + "\r\nBuild#: " + String(BUILD_NUMBER) + "\r\nLast commit: " + COMMIT_ID + "\r\nBuild Date: " + __DATE__ + " " + __TIME__);
}