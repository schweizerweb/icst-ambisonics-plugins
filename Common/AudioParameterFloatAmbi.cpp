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



#include "AudioParameterFloatAmbi.h"

AudioParameterFloatAmbi::AudioParameterFloatAmbi(const String& idToUse, const int versionToUse, const String& nameToUse, const String& labelToUse, Category categoryToUse, NormalisableRange<float> r, float def, AmbiDataSet* pPointArray, int pointIndex, ParamType paramType)
	: AudioParameterFloatAmbiAbs(idToUse, versionToUse, nameToUse, labelToUse, categoryToUse), 
	type(paramType),
	value(def),
	defaultValue(def),
	range(r),
	pAmbiPoints(pPointArray),
	ambiIndex(pointIndex)
{
    enabled = true;
}

AudioParameterFloatAmbi::~AudioParameterFloatAmbi()
{
}

AudioParameterFloatAmbi& AudioParameterFloatAmbi::operator= (float newValue)
{
	if (!approximatelyEqual(value, newValue))
	{
		setValueNotifyingHost(range.convertTo0to1(newValue));
	}

	return *this;
}

void AudioParameterFloatAmbi::setEnabled(bool en)
{
    enabled = en;
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
	if (!approximatelyEqual(value, newValue))
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
    if(!enabled)
    {
	    return;
	}
	
	float newValueScaled = range.convertFrom0to1(newValue);
	if (!approximatelyEqual(value, newValueScaled))
	{
		value = newValueScaled;
		if (pAmbiPoints->size() > ambiIndex)
		{
			switch (type)
			{
			case X: pAmbiPoints->setX(ambiIndex, value, false); break;
			case Y: pAmbiPoints->setY(ambiIndex, value, false); break;
			case Z: pAmbiPoints->setZ(ambiIndex, value, false); break;
			case Gain: pAmbiPoints->setGain(ambiIndex, Decibels::decibelsToGain(value), false); break;
            case GX: pAmbiPoints->setGroupX(ambiIndex, value, false); break;
            case GY: pAmbiPoints->setGroupY(ambiIndex, value, false); break;
            case GZ: pAmbiPoints->setGroupZ(ambiIndex, value, false); break;
			}
		}
	}
}
