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



#include "AmbiSource.h"

AmbiSource::~AmbiSource()
{
}

AmbiSource::AmbiSource() : AmbiPoint(), rms(0.0f)
{
}

AmbiSource::AmbiSource(AmbiSource* other, bool copyImage) : AmbiPoint(other, copyImage), rms(other->rms)
{
}

AmbiSource::AmbiSource(String _id, Point3D<double> _point, String _name, Colour _color) : AmbiPoint(_id, _point, _name, _color), rms(0.0f)
{
}

AmbiSource::AmbiSource(XmlElement* _element, AudioParameterSet _audioParams) : AmbiPoint(_element, _audioParams), rms(0.0f)
{
}

AmbiSource::AmbiSource(AudioParameterSet _audioParams, String _name) : AmbiPoint(_audioParams), rms(0.0f)
{
    setName(_name);
}

void AmbiSource::setRms(float newRmsLevel, bool onlyIfGreater)
{
	if (!onlyIfGreater || newRmsLevel > rms)
		rms = newRmsLevel;
}

float AmbiSource::getRms() const
{
	return rms;
}

float AmbiSource::getDisplayScaler()
{
	return 1.0f + 10.0f * rms;
}

XmlElement* AmbiSource::getAsXmlElement(String tagName)
{
	return getBaseXmlElement(tagName);
}
