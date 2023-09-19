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

class TrackColors
{
public:
	static Colour getColor(int trackNb)
	{
		int index = (trackNb -1) % numColors;
		switch(index)
		{
		case 0: return Colours::blue;
		case 1: return Colours::darkgreen;
		case 2: return Colours::darkred;
		case 3: return Colours::magenta;
		case 4: return Colours::cyan;
		case 5: return Colours::darkblue;
		case 6: return Colours::darkgrey;
		case 7: return Colours::darkorange;
		case 8: return Colours::darkgrey;
		case 9: return Colours::green;
		default: return  Colours::black;
		}
	}

	static Colour getSpeakerColor()
	{
		return Colours::darkblue;
	}
    
    static Colour getSpeakerColorAlt1()
    {
        return Colours::dodgerblue;
    }
    
    static Colour getSpeakerColorAlt2()
    {
        return Colours::indianred;
    }

    static Colour getSpeakerColorAlt3()
    {
        return Colours::darkgreen;
    }
    
    static Colour getSubwooferColor()
	{
		return Colours::darkred;
	}

    static const int numColors = 10;
};
