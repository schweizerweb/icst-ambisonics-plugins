/*
  ==============================================================================

    AudioParameterFloatAmbi.h
    Created: 15 Jan 2018 6:21:08am
    Author:  Christian Schweizer

  ==============================================================================
*/
#pragma once

#include "JuceHeader.h"
#include "../../Common/AudioParameterFloatAmbiAbs.h"
#include "../../Common/AmbiDataSet.h"

class AudioParameterFloatAmbi : public AudioParameterFloatAmbiAbs
{
public:
	enum ParamType { X, Y, Z };

	AudioParameterFloatAmbi(const String& idToUse, const String& nameToUse, const String& labelToUse, Category categoryToUse, 
		NormalisableRange<float> r, float def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType);
	~AudioParameterFloatAmbi();

	float getValue() const override;
	void setValue(float newValue) override;
	float get() const noexcept { return getValue(); }
	operator float() const noexcept { return getValue(); }
	AudioParameterFloatAmbi& operator= (float newValue);

	float getDefaultValue() const override;
	float getValueForText(const String& text) const override;
	void setUnscaledValue(float newValue) override;

private:
	ParamType type;

	float value;
	float defaultValue;
	NormalisableRange<float> range;

	AmbiDataSet* pAmbiPoints;
	int ambiIndex;
};
