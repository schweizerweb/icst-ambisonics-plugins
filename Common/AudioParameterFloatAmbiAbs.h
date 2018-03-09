/*
  ==============================================================================

    AudioParameterFloatAmbiAbs.h
    Created: 18 Jan 2018 5:18:10pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class AudioParameterFloatAmbiAbs : public AudioProcessorParameterWithID
{
public:
	AudioParameterFloatAmbiAbs(const String& parameterID, const String& name, const String& label, Category category)
		: AudioProcessorParameterWithID(parameterID, name, label, category)
	{
	}

	virtual void setUnscaledValue(float value) = 0;
};
