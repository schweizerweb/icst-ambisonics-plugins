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



#pragma once
#define PI 3.1415926535897932384626433832795
#define SOUND_SPEED_M_PER_S    343.0
#define SOUND_SPEED_MS_PER_M (1000.0 / SOUND_SPEED_M_PER_S)
#define SOUND_SPEED_S_PER_M (1.0 / SOUND_SPEED_M_PER_S)

class Constants
{
public:
	const static float AzimuthRadMin;
	const static float AzimuthRadMax;
	const static float ElevationRadMax;
	const static float ElevationRadMin;
	const static float DistanceMin;
	const static double AzimuthGradMin;
	const static double AzimuthGradMax;
	const static double ElevationGradMin;
	const static double ElevationGradMax;
	const static double GainDbMin;
	const static double GainDbMax;
	
    const static float CompressedMin;
    const static float CompressedMax;
    
    const static float StretchMin;
    const static float StretchMax;
	const static int audioParamVersion;

	static double RadToGrad(double rad);
	static double GradToRad(double grad);
	static int NormalizeAzimuthGrad(int grad);

	static bool isDevelopmentVersion();
    static bool isNonVisibleVersionPrerelease();
};
