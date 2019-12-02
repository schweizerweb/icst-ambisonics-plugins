/*
  ==============================================================================

    IIRFilterGraph.h
    Created: 12 Nov 2019 4:11:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../Common/FilterInfo.h"
#include "../../Common/SimpleGraph.h"

#define MIN_FREQUENCY	1
#define MAX_FREQUENCY	10000
#define FREQUENCY_STEP 1.01

//==============================================================================
/*
*/
class IIRFilterGraph    : public SimpleGraph
{
public:
    IIRFilterGraph(FilterInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification);
    ~IIRFilterGraph();

	void paintData(Graphics&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterGraph)

private:
	FilterInfo* pFilterInfo;
	Array<double> frequencies;
	double sampleRate;
	double* magnitudes;
};
