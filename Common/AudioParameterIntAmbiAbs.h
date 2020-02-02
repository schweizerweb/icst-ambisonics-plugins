/*
  ==============================================================================

    AudioParameterIntAmbiAbs.h
    Created: 1 Feb 2020 3:42:02pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class AudioParameterIntAmbiAbs : public AudioProcessorParameterWithID
{
public:
	AudioParameterIntAmbiAbs(const String& parameterID, const String& name, const String& label, Category category)
		: AudioProcessorParameterWithID(parameterID, name, label, category)
	{
	}

	virtual void setUnscaledValue(int value) = 0;
};
