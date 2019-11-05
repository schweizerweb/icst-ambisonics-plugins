/*
  ==============================================================================

    TrackColors.h
    Created: 20 Mar 2017 8:07:27am
    Author:  Christian Schweizer

  ==============================================================================
*/

#ifndef TRACKCOLORS_H_INCLUDED
#define TRACKCOLORS_H_INCLUDED

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
		return Colours::darkgrey;
	}

	static const int numColors = 10;
};

#endif  // TRACKCOLORS_H_INCLUDED
