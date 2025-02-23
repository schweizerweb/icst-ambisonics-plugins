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

class MultiDecoderColors
{
public:
	static Colour getColor(int i)
	{
		int index = i % numColors;
		switch(index)
		{
		case 0: return Colours::blue;
		case 1: return Colours::green;
		case 2: return Colours::red;
		case 3: return Colours::orange;
		case 4: return Colours::cyan;
		case 5: return Colours::magenta;
		case 6: return Colours::grey;
		case 7: return Colours::yellow;
		case 8: return Colours::darkblue;
		case 9: return Colours::lightgreen;
		default: return  Colours::black;
		}
	}

    static const int numColors = 10;
};
