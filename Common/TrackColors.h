/*
  ==============================================================================

    TrackColors.h
    Created: 20 Mar 2017 8:07:27am
    Author:  christian.schweizer

  ==============================================================================
*/

#ifndef TRACKCOLORS_H_INCLUDED
#define TRACKCOLORS_H_INCLUDED

#include "JuceHeader.h"

class TrackColors
{
public:
	TrackColors()
	{
		trackColors.add(Colours::red);
		trackColors.add(Colours::blue);
		trackColors.add(Colours::darkgreen);
		trackColors.add(Colours::darkred);
		trackColors.add(Colours::magenta);
		trackColors.add(Colours::cyan);
		trackColors.add(Colours::darkblue);
		trackColors.add(Colours::darkgrey);
		trackColors.add(Colours::darkorange);
		trackColors.add(Colours::lightgrey);
		trackColors.add(Colours::green);
	}

	Colour getColor(int trackNb)
	{
		return trackColors[trackNb % trackColors.size()];
	}

private:
	Array<Colour> trackColors;

};

#endif  // TRACKCOLORS_H_INCLUDED
