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
#include "Constants.h"

class AudioParameterFloatAmbiAbs : public AudioProcessorParameterWithID
{
public:
	AudioParameterFloatAmbiAbs(const String& parameterID, const int /*version*/, const String& _name, const String& _label, Category _category)
    : AudioProcessorParameterWithID(ParameterID(parameterID, Constants::audioParamVersion), _name, AudioProcessorParameterWithIDAttributes()
                                    .withLabel (_label)
                                    .withCategory (_category))
	{
	}

	virtual void setUnscaledValue(float value) = 0;
  virtual void setEnabled(bool enabled) = 0;
};
