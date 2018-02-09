/*
  ==============================================================================

    DelayBuffer.h
    Created: 17 Dec 2017 10:14:03pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class DelayBuffer
{
public:
	DelayBuffer()
		: buffer(new AudioBuffer<float>),
		  currentPosition(0)
	{
	}

	~DelayBuffer()
	{
		buffer = nullptr;
	}

	void checkAndAdjustSize(int requiredDelay);
	float processNextSample(float input);

private:
	ScopedPointer<AudioBuffer<float>> buffer;
	int currentPosition;
};
