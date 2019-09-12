/*
  ==============================================================================

    DelayBuffer.cpp
    Created: 17 Dec 2017 10:14:04pm
    Author:  Christian Schweizer

  ==============================================================================
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
