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
