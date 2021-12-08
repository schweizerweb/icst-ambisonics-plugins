/*
  ==============================================================================

    AudioParameterBoolAmbi.h
    Created: 8 Dec 2021 5:12:33pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../Common/AudioParameterBoolAmbiAbs.h"
#include "../../Common/AmbiDataSet.h"

class AudioParameterBoolAmbi : public AudioParameterBoolAmbiAbs
{
public:
	enum ParamType { Mute };

	AudioParameterBoolAmbi(const String& idToUse, const String& nameToUse, const String& labelToUse, bool def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType);
	~AudioParameterBoolAmbi();

    void setEnabled(bool enabled) override;
	float getValue() const override;
	void setValue(float newValue) override;
	bool get() const noexcept { return getValue(); }
	operator bool() const noexcept { return getValue(); }
	AudioParameterBoolAmbi& operator= (bool newValue);

	float getDefaultValue() const override;
	float getValueForText(const String& text) const override;
	void setUnscaledValue(bool newValue) override;

private:
	ParamType type;

	bool value;
	bool defaultValue;
	
	AmbiDataSet* pAmbiPoints;
	int ambiIndex;
    
    bool enabled;
};
