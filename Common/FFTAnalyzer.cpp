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
