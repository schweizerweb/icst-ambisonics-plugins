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
	AudioParameterFloatAmbiAbs(const String& parameterID, const int version, const String& name, const String& label, Category category)
    : AudioProcessorParameterWithID(ParameterID { parameterID, version }, name, AudioProcessorParameterWithIDAttributes().withLabel(label).withCategory(category))
	{
	}

	virtual void setUnscaledValue(float value) = 0;
    virtual void setEnabled(bool enabled) = 0;
};
