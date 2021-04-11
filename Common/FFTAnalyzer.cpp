/*
  ==============================================================================

    FFTAnalyzer.cpp
    Created: 26 Dec 2020 9:12:54pm
    Author:  chris

  ==============================================================================
*/

#include "FFTAnalyzer.h"

juce_ImplementSingleton(FFTAnalyzer)

FFTAnalyzer::FFTAnalyzer() : forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    activeChannel = INACTIVE;
}

FFTAnalyzer::~FFTAnalyzer()
{
    clearSingletonInstance();
}

void FFTAnalyzer::setActive(int channel)
{
    if(activeChannel != channel)
    {
        nextFFTBlockReady = false;
        fifoIndex = 0;
        activeChannel = channel;
    }
}

void FFTAnalyzer::disable()
{
    activeChannel = INACTIVE;
}

bool FFTAnalyzer::isActive(int channel) const
{
    return activeChannel != INACTIVE && activeChannel == channel;
}

bool FFTAnalyzer::scopeRequest(float* pData, int* pFFTSize)
{
    if (nextFFTBlockReady)
    {
        window.multiplyWithWindowingTable(fftData, fftSize);
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);

        for (int i = 0; i < SCOPE_SIZE; ++i)
        {
            const int fftDataIndex = jlimit(0, fftSize / 2, (int)i);
            const float level = Decibels::gainToDecibels(fftData[fftDataIndex]) - Decibels::gainToDecibels(float(fftSize));

            pData[i] = level;
        }

        nextFFTBlockReady = false;
        *pFFTSize = fftSize;
        return true;
    }

    return false;
}

void FFTAnalyzer::pushNextSampleIntoFifo(float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}