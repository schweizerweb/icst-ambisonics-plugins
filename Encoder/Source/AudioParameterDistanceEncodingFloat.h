/*
  ==============================================================================

    AudioParameterDistanceEncodingFloat.h
    Created: 1 Feb 2020 8:05:22pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AudioParameterFloatAmbiAbs.h"
#include "DistanceEncodingParams.h"

class AudioParameterDistanceEncodingFloat : public AudioParameterFloatAmbiAbs
{
public:
	enum ParamType { EncodingMode, UnitCircleRadius };

	AudioParameterDistanceEncodingFloat(const String& idToUse, const String& nameToUse, const String& labelToUse, Category categoryToUse,
		NormalisableRange<float> r, float def, DistanceEncodingParams* pDistanceEncodingParams, ParamType paramType);
	~AudioParameterDistanceEncodingFloat();

	float getValue() const override;
	void setValue(float newValue) override;
	float get() const noexcept { return getValue(); }
	operator float() const noexcept { return getValue(); }
	AudioParameterDistanceEncodingFloat& operator= (float newValue);

	float getDefaultValue() const override;
	float getValueForText(const String& text) const override;
	void setUnscaledValue(float newValue) override;

private:
	ParamType type;

	float value;
	float defaultValue;
	NormalisableRange<float> range;

    DistanceEncodingParams* pDistanceEncodingParams;
};
