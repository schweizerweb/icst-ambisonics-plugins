/*
  ==============================================================================

    AudioParameterDistanceEncodingFloat.cpp
    Created: 1 Feb 2020 8:05:22pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "AudioParameterDistanceEncodingFloat.h"

AudioParameterDistanceEncodingFloat::AudioParameterDistanceEncodingFloat(const String& idToUse, const String& nameToUse, const String& labelToUse, Category categoryToUse, NormalisableRange<float> r, float def, DistanceEncodingParams* pDistanceEncodingParams, ParamType paramType)
    : AudioParameterFloatAmbiAbs(idToUse, nameToUse, labelToUse, categoryToUse),
    type(paramType),
    value(def),
    defaultValue(def),
    range(r),
    pDistanceEncodingParams(pDistanceEncodingParams)
{
}

AudioParameterDistanceEncodingFloat::~AudioParameterDistanceEncodingFloat()
{
}

AudioParameterDistanceEncodingFloat& AudioParameterDistanceEncodingFloat::operator= (float newValue)
{
    if (value != newValue)
    {
        setValueNotifyingHost(range.convertTo0to1(newValue));
    }

    return *this;
}

float AudioParameterDistanceEncodingFloat::getDefaultValue() const
{
    return range.convertTo0to1(defaultValue);
}

float AudioParameterDistanceEncodingFloat::getValueForText(const String& text) const
{
    return range.convertTo0to1(text.getFloatValue());
}

void AudioParameterDistanceEncodingFloat::setUnscaledValue(float newValue)
{
    if (value != newValue)
    {
        setValueNotifyingHost(range.convertTo0to1(newValue));
    }
}

float AudioParameterDistanceEncodingFloat::getValue() const
{
    return range.convertTo0to1(value);
}

void AudioParameterDistanceEncodingFloat::setValue(float newValue)
{
    float newValueScaled = range.convertFrom0to1(newValue);
    if (value != newValueScaled)
    {
        value = newValueScaled;
        switch (type)
        {
            case EncodingMode: pDistanceEncodingParams->setEncodingMode(DistanceEncodingParams::EncodingMode(value), false); break;
            case UnitCircleRadius: pDistanceEncodingParams->setUnitCircleRadius(value, false); break;
        }
    }
}
