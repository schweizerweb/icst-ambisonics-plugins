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
	RadarColors(): colorScheme(0)
	{
	}

	Colour getRadarBackground() const { return radarBackground[colorScheme]; }
	Colour getRadarLineColor() const { return radarLineColor[colorScheme]; }
	Colour getInfoTextColor() const { return infoTextColor[colorScheme]; }
	Colour getPointSelectionColor() const { return pointSelectionColor[colorScheme]; }
    Colour getRadarAxisColor() const { return radarAxisColor[colorScheme]; }
    Colour getValidRangeColor() const { return radarValidRangeColor[colorScheme]; }
	void setColorScheme(int scheme) { colorScheme = jmin(scheme, NUMBER_OF_SCHEMES - 1); }

private:
	Colour radarBackground[NUMBER_OF_SCHEMES] { Colours::lightgrey, Colours::lightgoldenrodyellow };
	Colour radarLineColor[NUMBER_OF_SCHEMES] { Colours::darkgrey, Colours::darkorange };
	Colour infoTextColor[NUMBER_OF_SCHEMES] { Colours::darkgrey, Colours::olivedrab };
	Colour pointSelectionColor[NUMBER_OF_SCHEMES] { Colours::white, Colours::yellow };
    Colour radarAxisColor[NUMBER_OF_SCHEMES] { Colours::darkgreen, Colours::darkblue };
    Colour radarValidRangeColor[NUMBER_OF_SCHEMES] { Colours::lightgreen.withAlpha(0.2f), Colours::lightpink.withAlpha(0.2f) };
	int colorScheme;
};
