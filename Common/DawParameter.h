/*
  ==============================================================================

    DawParameter.h
    Created: 15 Nov 2019 11:00:50am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class DawParameter
{
public:
	DawParameter(): updateTrackPropertiesWorking(false)
	{
	}

	AudioProcessor::TrackProperties lastTrackProperties;
	bool updateTrackPropertiesWorking;
};