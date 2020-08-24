/*
  ==============================================================================

    VarDelayBuffer.h
    Created: 18 Nov 2019 8:57:03am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class VarDelayBuffer
{
public:
	void process(int requiredBufferSize, float* buffer, int numSamples);

private:
	Array<float> audioBuffer;
	//CatmullRomInterpolator interpolator;
	LagrangeInterpolator interpolator;
};
