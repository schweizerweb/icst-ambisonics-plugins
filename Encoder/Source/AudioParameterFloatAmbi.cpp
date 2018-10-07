/*
  ==============================================================================

    AudioParameterFloatAmbi.cpp
    Created: 15 Jan 2018 6:21:08am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AudioParameterFloatAmbi.h"

AudioParameterFloatAmbi::AudioParameterFloatAmbi(const String& idToUse, const String& nameToUse, const String& labelToUse, Category categoryToUse, NormalisableRange<float> r, float def, OwnedArray<AmbiPoint>* pPointArray, int pointIndex, ParamType paramType)
	: AudioParameterFloatAmbiAbs(idToUse, nameToUse, labelToUse, categoryToUse), 
	type(paramType),
	value(def),
	defaultValue(def),
	range(r),
	pAmbiPoints(pPointArray),
	ambiIndex(pointIndex)
{
}

AudioParameterFloatAmbi::~AudioParameterFloatAmbi()
{
}

AudioParameterFloatAmbi& AudioParameterFloatAmbi::operator= (float newValue)
{
	if (value != newValue)
	{
		setValueNotifyingHost(range.convertTo0to1(newValue));
	}

	return *this;
}

float AudioParameterFloatAmbi::getDefaultValue() const
{
	return range.convertTo0to1(defaultValue);
}

float AudioParameterFloatAmbi::getValueForText(const String& text) const
{
	return range.convertTo0to1(text.getFloatValue());
}

void AudioParameterFloatAmbi::setUnscaledValue(float newValue)
{
	if (value != newValue)
	{
		setValueNotifyingHost(range.convertTo0to1(newValue));
	}
}

float AudioParameterFloatAmbi::getValue() const
{
	return range.convertTo0to1(value);
}

void AudioParameterFloatAmbi::setValue(float newValue)
{
	float newValueScaled = range.convertFrom0to1(newValue);
	if (value != newValueScaled)
	{
		value = newValueScaled;
		if (pAmbiPoints->size() > ambiIndex)
		{
			switch (type)
			{
			case X: pAmbiPoints->getUnchecked(ambiIndex)->getPoint()->setX(value, false); break;
			case Y: pAmbiPoints->getUnchecked(ambiIndex)->getPoint()->setY(value, false); break;
			case Z: pAmbiPoints->getUnchecked(ambiIndex)->getPoint()->setZ(value, false); break;
			}
		}
	}
}
