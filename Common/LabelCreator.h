/*
  ==============================================================================

    LabelCreator.h
    Created: 15 Oct 2018 6:00:01am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class LabelCreator
{
public:
	static Image createNewLabel(String label, Colour newColor, float fontSize);
};