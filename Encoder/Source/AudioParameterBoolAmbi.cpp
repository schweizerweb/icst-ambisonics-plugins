/*
  ==============================================================================

    AudioParameterBoolAmbi.cpp
    Created: 8 Dec 2021 5:12:33pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "AudioParameterBoolAmbi.h"
AudioParameterBoolAmbi::AudioParameterBoolAmbi(const String& idToUse, const String& nameToUse, const String& labelToUse, bool def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType)
	: AudioParameterBoolAmbiAbs(idToUse, nameToUse, labelToUse, AudioProcessorParameter::genericParameter),
	type(paramType),
	value(def),
	defaultValue(def),
	pAmbiPoints(pPointArray),
	ambiIndex(pointIndex)
{
    enabled = true;
}

AudioParameterBoolAmbi::~AudioParameterBoolAmbi()
{
}

AudioParameterBoolAmbi& AudioParameterBoolAmbi::operator= (bool newValue)
{
	if (value != newValue)
	{
		setValueNotifyingHost(newValue);
	}

	return *this;
}

void AudioParameterBoolAmbi::setEnabled(bool en)
{
    enabled = en;
}

float AudioParameterBoolAmbi::getDefaultValue() const
{
	return defaultValue;
}

float AudioParameterBoolAmbi::getValueForText(const String& text) const
{
	return (float)text.getIntValue();
}

void AudioParameterBoolAmbi::setUnscaledValue(bool newValue)
{
	if (value != newValue)
	{
		setValueNotifyingHost(newValue);
	}
}

float AudioParameterBoolAmbi::getValue() const
{
	return value;
}

void AudioParameterBoolAmbi::setValue(float newValue)
{
    if(!enabled)
        return;

    const bool newValueScaled = newValue != 0.0f;
	if (value != newValueScaled)
	{
		value = newValueScaled;
		if (pAmbiPoints->size() > ambiIndex)
		{
			switch (type)
			{
			case Mute: pAmbiPoints->setMute(ambiIndex, value, false); break;
			}
		}
	}
}
