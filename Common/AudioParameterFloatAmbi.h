/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once

#include "JuceHeader.h"
#include "../../Common/AudioParameterFloatAmbiAbs.h"
#include "../../Common/AmbiDataSet.h"

class AudioParameterFloatAmbi : public AudioParameterFloatAmbiAbs
{
public:
	enum ParamType { X, Y, Z, Gain, GX, GY, GZ };

	AudioParameterFloatAmbi(const String& idToUse, const int versionToUse, const String& nameToUse, const String& labelToUse, Category categoryToUse, 
		NormalisableRange<float> r, float def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType);
	~AudioParameterFloatAmbi() override;

    void setEnabled(bool enabled) override;
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
    
    bool enabled;
};
