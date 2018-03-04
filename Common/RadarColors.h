/*
  ==============================================================================

    RadarColors.h
    Created: 25 Jul 2017 6:12:01am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#define NUMBER_OF_SCHEMES 2

class RadarColors
{
public:
	RadarColors(int colorScheme = 0) : colorScheme(colorScheme)
	{
		jassert(colorScheme < NUMBER_OF_SCHEMES);
	}

	Colour getRadarBackground() const { return radarBackground[colorScheme]; }
	Colour getRadarLineColor() const { return radarLineColor[colorScheme]; }
	Colour getInfoTextColor() const { return infoTextColor[colorScheme]; }
	Colour getPointSelectionColor() const { return pointSelectionColor[colorScheme]; }

private:
	Colour radarBackground[NUMBER_OF_SCHEMES] { Colours::lightgrey, Colours::lightgoldenrodyellow };
	Colour radarLineColor[NUMBER_OF_SCHEMES] { Colours::darkgrey, Colours::darkorange };
	Colour infoTextColor[NUMBER_OF_SCHEMES] { Colours::darkgrey, Colours::olivedrab };
	Colour pointSelectionColor[NUMBER_OF_SCHEMES] { Colours::white, Colours::yellow };
	int colorScheme;
};