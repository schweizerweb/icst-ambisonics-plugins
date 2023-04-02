/*
  ==============================================================================

    AudioParameterBoolAmbiAbs.h
    Created: 8 Dec 2021 5:10:47pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#pragma once
#include "JuceHeader.h"

class AudioParameterBoolAmbiAbs : public AudioProcessorParameterWithID
{
public:
	AudioParameterBoolAmbiAbs(const String& parameterID, const int /*version*/, const String& name, const String& label, Category category)
    : AudioProcessorParameterWithID(parameterID, name, AudioProcessorParameterWithIDAttributes()
                                    .withLabel (label)
                                    .withCategory (category))
	{
	}

	virtual void setUnscaledValue(bool value) = 0;
    virtual void setEnabled(bool enabled) = 0;
};
