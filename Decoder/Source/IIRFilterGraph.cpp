/*
  ==============================================================================

    IIRFilterGraph.cpp
    Created: 12 Nov 2019 4:11:24pm
    Author:  chris

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "IIRFilterGraph.h"

//==============================================================================

IIRFilterGraph::IIRFilterGraph(FilterInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification): pFilterInfo(pFilterInfo)
{
    sampleRate = pFilterSpecification->sampleRate;
	double currentFrequency = MIN_FREQUENCY;
	while(currentFrequency < MAX_FREQUENCY)
	{
		frequencies.add(currentFrequency);
		currentFrequency *= FREQUENCY_STEP;
	}
	
	magnitudes = static_cast<double*>(calloc(frequencies.size(), sizeof(double)));

	setDisplayRange(Logarithmic, Range<double>(20, MAX_FREQUENCY), Linear, Range<double>(-20, 20));
	fullGridFlag = true;
	labelAxisY = "Gain [dB]";
	labelAxisX = "Frequency [Hz]";
}

IIRFilterGraph::~IIRFilterGraph()
{
	free(magnitudes);
}

void IIRFilterGraph::paint (Graphics& g)
{
	SimpleGraph::paint(g);
	
	// draw curve
	dsp::IIR::Coefficients<float>::Ptr coeff = pFilterInfo->getCoefficients(sampleRate);
	Path path;

	if(coeff == nullptr)
	{
		path.startNewSubPath(mapValues(frequencies.getFirst(), 0.0).toFloat());
		path.lineTo(mapValues(frequencies.getLast(), 0.0).toFloat());
	}
	else
	{
		coeff->getMagnitudeForFrequencyArray(frequencies.getRawDataPointer(), magnitudes, frequencies.size(), sampleRate);

		for (int i = 0; i < frequencies.size(); i++)
		{
			Point<float> displayPoint = mapValues(frequencies[i], Decibels::gainToDecibels(magnitudes[i])).toFloat();
			
			if (i == 0)
				path.startNewSubPath(displayPoint);
			else
				path.lineTo(displayPoint);
		}
	}

	g.setColour(Colours::darkgreen);
	g.strokePath(path, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));
}
