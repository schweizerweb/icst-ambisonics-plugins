/*
  ==============================================================================

    VarDelayBuffer.cpp
    Created: 18 Nov 2019 8:57:03am
    Author:  chris

  ==============================================================================
*/

#include "VarDelayBuffer.h"

void VarDelayBuffer::process(int requiredBufferSize, float* buffer, int numSamples)
{
	if(audioBuffer.size() != requiredBufferSize)
	{
		int samplesToAdd = requiredBufferSize + numSamples - audioBuffer.size();
		samplesToAdd = jmin(samplesToAdd, int(numSamples * 1.5));
		samplesToAdd = jmax(samplesToAdd, int(numSamples / 1.5f));
		if (samplesToAdd > 0)
		{
			float* tempBuffer = (float*)calloc(samplesToAdd, sizeof(float));
			interpolator.process(double(numSamples) / samplesToAdd, buffer, tempBuffer, samplesToAdd);
			audioBuffer.addArray(tempBuffer, samplesToAdd);
			free(tempBuffer);
		}
	}
	else
	{
		audioBuffer.addArray(buffer, numSamples);
	}

	// remove the oldest elements from the buffer and write them to the current audio buffer
	memcpy(buffer, audioBuffer.data(), numSamples * sizeof(float));
	audioBuffer.removeRange(0, numSamples);
}
