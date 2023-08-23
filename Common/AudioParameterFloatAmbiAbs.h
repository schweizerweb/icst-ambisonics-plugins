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
	AudioParameterFloatAmbiAbs(const String& parameterID, const int /*version*/, const String& _name, const String& _label, Category _category)
    : AudioProcessorParameterWithID(parameterID, _name, AudioProcessorParameterWithIDAttributes()
                                    .withLabel (_label)
                                    .withCategory (_category))
	{
	}

	virtual void setUnscaledValue(float value) = 0;
    virtual void setEnabled(bool enabled) = 0;
};
