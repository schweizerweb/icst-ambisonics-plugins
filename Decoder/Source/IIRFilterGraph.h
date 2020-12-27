/*
  ==============================================================================

    IIRFilterGraph.h
    Created: 12 Nov 2019 4:11:24pm
    Author:  chris

  ==============================================================================
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
    IIRFilterGraph(FilterBankInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification);
    ~IIRFilterGraph();

	void paintData(Graphics&) override;
    void setFFTResult(float* data, float* fftFrequencies, int size, int newFftSize);
    void disableFFT();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterGraph)

private:
	FilterBankInfo* pFilterInfo;
	Array<double> frequencies;
	double sampleRate;
	double* magnitudes[MAX_FILTER_COUNT];
    float* fftResultData;
    float* fftResultFrequencies;
    int fftResultDataSize;
    int fftSize;
};
