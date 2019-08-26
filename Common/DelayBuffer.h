/*
  ==============================================================================

    DelayBuffer.h
    Created: 17 Dec 2017 10:14:03pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class DelayBuffer
{
public:
	DelayBuffer()
		: currentPosition(0)
	{
	}

	~DelayBuffer()
	{
	}

	void checkAndAdjustSize(int requiredDelay);
	float processNextSample(float input);

private:
	AudioBuffer<float> buffer;
	int currentPosition;
};
