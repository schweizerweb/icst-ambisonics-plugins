/*
  ==============================================================================

    Constants.h
    Created: 27 Sep 2018 12:37:18pm
    Author:  Christian Schweizer

  ==============================================================================
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
