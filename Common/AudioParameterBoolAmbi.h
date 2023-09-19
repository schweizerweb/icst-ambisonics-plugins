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
#include "../../Common/AudioParameterBoolAmbiAbs.h"
#include "../../Common/AmbiDataSet.h"

class AudioParameterBoolAmbi : public AudioParameterBoolAmbiAbs
{
public:
	enum ParamType { Mute };

	AudioParameterBoolAmbi(const String& idToUse, const int versionToUse, const String& nameToUse, const String& labelToUse, bool def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType);
	~AudioParameterBoolAmbi() override;

    void setEnabled(bool enabled) override;
	float getValue() const override;
	void setValue(float newValue) override;
	bool get() const noexcept { return value; }
	operator bool() const noexcept { return value; }
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
