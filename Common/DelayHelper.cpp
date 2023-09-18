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



#include "DelayHelper.h"
#include "Constants.h"

double DelayHelper::getTotalDelayMs(double distance) const
{
	return distance * SOUND_SPEED_MS_PER_M;
}

double DelayHelper::getDelayCompensationMs(double distance, double maxNormalizedDistance) const
{
	return (maxNormalizedDistance - distance)
		* SOUND_SPEED_MS_PER_M;
}

int DelayHelper::getDelayCompensationSamples(double distance, double maxNormalizedDistance, double samplingRate)
{
	return int((maxNormalizedDistance - distance)
		* SOUND_SPEED_S_PER_M
		* samplingRate);
}

float DelayHelper::getDelaySamples(double distance, double samplingRate)
{
	return float(distance * SOUND_SPEED_S_PER_M * samplingRate);
}
