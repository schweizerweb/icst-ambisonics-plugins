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

class FilterColors
{
public:
	static Colour getColor(int i)
	{
		int index = i % numColors;
		switch(index)
		{
            case 0: return Colours::steelblue;
            case 1: return Colours::forestgreen;
            case 2: return Colours::darkcyan;
            case 3: return Colours::mediumpurple;
            case 4: return Colours::sienna;
            case 5: return Colours::olive;
            case 6: return Colours::dodgerblue;
            case 7: return Colours::rosybrown;
            case 8: return Colours::slategrey;
            case 9:
            default: return juce::Colours::darkkhaki;
		}
	}

    static const int numColors = 10;
};
