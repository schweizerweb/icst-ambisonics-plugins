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



#pragma once
#include "JuceHeader.h"

#define INACTIVE -1
#define SCOPE_SIZE 2048

class FFTAnalyzer
{
public:
    FFTAnalyzer();
    ~FFTAnalyzer();

    juce_DeclareSingleton(FFTAnalyzer, false)

    void setActive(int channel);
    void disable();
    bool isActive(int channel) const;
    bool scopeRequest(float* pData, int* pFFTSize);
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
