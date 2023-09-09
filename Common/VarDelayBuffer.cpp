/*
  ==============================================================================

    VarDelayBuffer.cpp
    Created: 18 Nov 2019 8:57:03am
    Author:  chris

  ==============================================================================
*/

#include "VarDelayBuffer.h"

VarDelayBuffer::VarDelayBuffer(): audioBuffer(nullptr), bufferLength(0), iWrite(0), lastDelayInSamples(0)
{
}

VarDelayBuffer::~VarDelayBuffer()
{
	if (audioBuffer != nullptr)
		free(audioBuffer);
}

void VarDelayBuffer::initialize(int bufferSize)
{
	if (audioBuffer != nullptr)
		free(audioBuffer);

	audioBuffer = (float*)calloc((size_t)bufferSize, sizeof(float));
	bufferLength = bufferSize;
	iWrite = 0;
	lastDelayInSamples = 0.0f;
	first = true;
}

void VarDelayBuffer::process(float newDelayInSamples, const float* inBuffer, float* outBuffer, int numSamples)
{
	if(first)
	{
		lastDelayInSamples = newDelayInSamples;
		first = false;
	}

	if(newDelayInSamples > (float)bufferLength)
	{
		newDelayInSamples = float(bufferLength);
	}

    float slope = (float(newDelayInSamples) - lastDelayInSamples) / float(numSamples);
	for (int i = 0; i < numSamples; i++)
	{
		audioBuffer[iWrite] = inBuffer[i];		// write input sound into the buffer

		float pRead = (float)iWrite - (lastDelayInSamples + (float)i * slope);			// float Leseposition 
		int iRead = (int)pRead;
		float fracRead = pRead - (float)iRead;
		if (iRead < 0) {
			iRead += bufferLength;
		}

		int iReadPlus1 = iRead + 1;
		if (iReadPlus1 >= bufferLength) {
			iReadPlus1 -= bufferLength;
		}

		outBuffer[i] = audioBuffer[iRead] + fracRead * (audioBuffer[iReadPlus1] - audioBuffer[iRead]);  	// lineare Interpolation
		
		iWrite++;
		if (iWrite >= bufferLength) {
			iWrite = 0;
		}
	}

	lastDelayInSamples = newDelayInSamples;
}

void VarDelayBuffer::check(int bufferSize)
{
	if(bufferSize != bufferLength)
	{
		initialize(bufferSize);
	}
}
