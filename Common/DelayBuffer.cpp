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



#include "DelayBuffer.h"

void DelayBuffer::checkAndAdjustSize(int requiredDelay)
{
	if (buffer.getNumSamples() != requiredDelay)
	{
		AudioBuffer<float> newBuffer;
		newBuffer.setSize(1, requiredDelay, false, true);
		
		// keep old data if available
		for (int i = 0; i < buffer.getNumSamples() && i < newBuffer.getNumSamples(); i++)
		{
			newBuffer.setSample(0, i, buffer.getSample(0, (currentPosition + i) % buffer.getNumSamples()));
		}
		
		buffer = newBuffer;
		currentPosition = 0;
	}
}

float DelayBuffer::processNextSample(float input)
{
	if (buffer.getNumSamples() == 0)
		return input;

	float output = buffer.getSample(0, currentPosition);
	buffer.setSample(0, currentPosition, input);

	currentPosition = (currentPosition + 1) % buffer.getNumSamples();

	return output;
}
