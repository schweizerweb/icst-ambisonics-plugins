/*
  ==============================================================================

    TrackColorSelector.h
    Created: 5 Nov 2019 12:03:35pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "TrackColors.h"

class TrackColorSelector : public ColourSelector
{
public:

	TrackColorSelector() : ColourSelector(showColourspace)
	{
		for (int i = 0; i < TrackColors::numColors; i++)
		{
			colours.add(TrackColors::getColor(i + 1));
		}
	}
  
  ~TrackColorSelector() {}
  
  // ---------------------------------------------------------------------------
  
  
  int getNumSwatches() const override { return TrackColors::numColors; }
  
  Colour getSwatchColour(int index) const override { return colours[index]; }
  
  void setSwatchColour (int index, const Colour &newColour) override
  {
    colours[index] = newColour;
  }
       
  
private:
  
  Array<Colour> colours;
    
};
