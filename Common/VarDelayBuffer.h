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
    VarDelayBuffer();
    ~VarDelayBuffer();
    void initialize(int bufferSize);
    void process(float newDelayInSamples, const float* inBuffer, float* outBuffer, int numSamples);
    void check(int bufferSize);

private:
	float* audioBuffer;
    int bufferLength;
    int iWrite;
    float lastDelayInSamples;
    bool first;
};
