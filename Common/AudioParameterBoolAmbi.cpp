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



#include "AudioParameterBoolAmbi.h"

AudioParameterBoolAmbi::AudioParameterBoolAmbi(const String& idToUse, const int versionToUse, const String& nameToUse, const String& labelToUse, bool def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType)
	: AudioParameterBoolAmbiAbs(idToUse, versionToUse, nameToUse, labelToUse, AudioProcessorParameter::genericParameter),
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

    const bool newValueScaled = !exactlyEqual(newValue, 0.0f);
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
