/*
  ==============================================================================

    FFTAnalyzer.h
    Created: 26 Dec 2020 9:12:54pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define INACTIVE -1
#define SCOPE_SIZE 1024

class FFTAnalyzer
{
public:
    FFTAnalyzer();
    ~FFTAnalyzer();

    juce_DeclareSingleton(FFTAnalyzer, false);

    void setActive(int channel);
    void disable();
    bool isActive(int channel) const;
    bool scopeRequest(float* pData, float* pFrequencies, int* pFFTSize);
    void pushNextSampleIntoFifo(float sample) noexcept;

private:
    int activeChannel;

    enum
    {
        fftOrder = 12,
        fftSize = 1 << fftOrder
    };

    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;

    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFTAnalyzer)
};