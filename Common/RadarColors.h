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
    Colour getExtendedHandleColor() const { return extendedHandleColor[colorScheme]; }
    Colour getExtendedHandleSelectionColor() const { return extendedHandleSelectionColor[colorScheme]; }
    void setColorScheme(int scheme) { colorScheme = jmin(scheme, NUMBER_OF_SCHEMES - 1); }

private:
	Colour radarBackground[NUMBER_OF_SCHEMES] { Colours::lightgrey, Colours::lightgoldenrodyellow };
	Colour radarLineColor[NUMBER_OF_SCHEMES] { Colours::darkgrey, Colours::darkorange };
	Colour infoTextColor[NUMBER_OF_SCHEMES] { Colours::darkgrey, Colours::olivedrab };
	Colour pointSelectionColor[NUMBER_OF_SCHEMES] { Colours::white, Colours::yellow };
    Colour radarAxisColor[NUMBER_OF_SCHEMES] { Colours::darkgreen, Colours::darkblue };
    Colour radarValidRangeColor[NUMBER_OF_SCHEMES] { Colours::lightgreen.withAlpha(0.2f), Colours::lightpink.withAlpha(0.2f) };
    Colour extendedHandleColor[NUMBER_OF_SCHEMES] { Colours::lightcoral, Colours::lightblue };
    Colour extendedHandleSelectionColor[NUMBER_OF_SCHEMES] { Colours::red, Colours::blue };
    
	int colorScheme;
};
