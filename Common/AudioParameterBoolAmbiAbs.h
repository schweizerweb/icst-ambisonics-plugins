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
	AudioParameterBoolAmbiAbs(const String& parameterID, const int /*version*/, const String& _name, const String& _label, Category _category)
    : AudioProcessorParameterWithID(parameterID, _name, AudioProcessorParameterWithIDAttributes()
                                    .withLabel (_label)
                                    .withCategory (_category))
	{
	}

	virtual void setUnscaledValue(bool value) = 0;
    virtual void setEnabled(bool enabled) = 0;
};
