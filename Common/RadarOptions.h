/*
  ==============================================================================

    RadarOptions.h
    Created: 24 Dec 2017 3:11:46pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AudioParameterSet.h"

class RadarOptions
{
public:
	RadarOptions();
	AudioParameterSet getAudioParamForIndex(int index) const;

public:
	bool nameFieldEditable;
	int maxNumberEditablePoints;
	bool editablePointsAsSquare;

	Array<AudioParameterSet>* audioParams;
};
