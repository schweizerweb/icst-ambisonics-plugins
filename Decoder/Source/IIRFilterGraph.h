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
#include "../../Common/FilterBankInfo.h"
#include "../../Common/SimpleGraph.h"

#define MIN_FREQUENCY	1
#define FREQUENCY_STEP 1.01

//==============================================================================
/*
*/
class IIRFilterGraph    : public SimpleGraph
{
public:
    IIRFilterGraph(std::vector<FilterBankInfo*> pFilterInfo, dsp::ProcessSpec* pFilterSpecification, std::vector<juce::Colour*> pColors = std::vector<juce::Colour*>());
    ~IIRFilterGraph() override;

	void paintData(Graphics&) override;
    void setFFTResult(float* data, int size, int newFftSize);
    void setFFTParams(bool enable, double scaler = 0);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterGraph)

private:
	std::vector<FilterBankInfo*> pFilterInfo;
	std::vector<juce::Colour*> pColors;
	Array<double> frequencies;
	double sampleRate;
	double* magnitudes[MAX_FILTER_COUNT];
    float* fftResultData;
    int fftResultDataSize;
    int fftSize;
    double fftScaler;
};
