/*
  ==============================================================================

    Constants.h
    Created: 27 Sep 2018 12:37:18pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#define PI 3.1415926535897932384626433832795

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

	static double RadToGrad(double rad);
	static double GradToRad(double grad);
	static int NormalizeAzimuthGrad(int grad);

	static bool isDevelopmentVersion();
};
