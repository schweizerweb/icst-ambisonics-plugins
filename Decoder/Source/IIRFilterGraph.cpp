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

IIRFilterGraph::IIRFilterGraph(FilterBankInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification): pFilterInfo(pFilterInfo), fftResultDataSize(0), fftResultData(nullptr), fftResultFrequencies(nullptr), fftSize(0)
{
    sampleRate = pFilterSpecification->sampleRate;
	double currentFrequency = MIN_FREQUENCY;
	while(currentFrequency < pFilterSpecification->sampleRate / 2.0)
	{
		frequencies.add(currentFrequency);
		currentFrequency *= FREQUENCY_STEP;
	}

	for (int i = 0; i < MAX_FILTER_COUNT; i++)
	{
		magnitudes[i] = static_cast<double*>(calloc(frequencies.size(), sizeof(double)));
	}

	setDisplayRange(LogarithmicFrequency, Range<double>(20, pFilterSpecification->sampleRate / 2.0), Linear, Range<double>(-40, 40));
	fullGridFlag = true;
	labelAxisY = "Gain [dB]";
	labelAxisX = "Frequency [Hz]";
}

IIRFilterGraph::~IIRFilterGraph()
{
	for (int i = 0; i < MAX_FILTER_COUNT; i++)
	{
		free(magnitudes[i]);
	}

	if (fftResultData != nullptr)
	{
		free(fftResultData);
	}

	if(fftResultFrequencies != nullptr)
	{
		free(fftResultFrequencies);
	}
}

void IIRFilterGraph::paintData(Graphics& g)
{
	// draw curve
	Path path;

	if(!pFilterInfo->anyActive())
	{
		path.startNewSubPath(mapValues(frequencies.getFirst(), 0.0).toFloat());
		path.lineTo(mapValues(frequencies.getLast(), 0.0).toFloat());
	}
	else
	{
		std::vector<dsp::IIR::Coefficients<float>::Ptr> coeffs;
		pFilterInfo->getCoefficients(sampleRate, &coeffs);
		int activeFilterCount = int(coeffs.size());
		for (int iCoeff = 0; iCoeff < activeFilterCount && iCoeff < MAX_FILTER_COUNT; iCoeff++)
		{
			coeffs[iCoeff]->getMagnitudeForFrequencyArray(frequencies.getRawDataPointer(), magnitudes[iCoeff], frequencies.size(), sampleRate);
		}

		for (int i = 0; i < frequencies.size(); i++)
		{
			double magnitudeSum = 1.0;
			for (int iMag = 0; iMag < activeFilterCount; iMag++)
				magnitudeSum *= magnitudes[iMag][i];

			Point<float> displayPoint = mapValues(frequencies[i], Decibels::gainToDecibels(magnitudeSum)).toFloat();
			
			if (i == 0)
				path.startNewSubPath(displayPoint);
			else
				path.lineTo(displayPoint);
		}
	}

	g.setColour(Colours::lightgreen);
	g.strokePath(path, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));

	if(fftResultDataSize > 0)
	{
		g.setColour(Colours::blueviolet.withAlpha(0.5f));
	    for(int i = 0; i < fftResultDataSize; i++)
	    {
			double frequency = (double)fftResultFrequencies[i] * (sampleRate / fftSize);
			Point<float> displayPoint = mapValues(frequency, fftResultData[i]).toFloat();
			if (displayPoint.getX() > 0)
			{
				g.drawLine(displayPoint.getX(), displayPoint.getY(), displayPoint.getX(), graphArea->getBottom());
			}
	    }
	}
}

void IIRFilterGraph::setFFTResult(float* data, float* frequencies, int size, int fftSize)
{
	this->fftSize = fftSize;
	if(size != fftResultDataSize)
	{
		if (fftResultData != nullptr)
			free(fftResultData);
		if (fftResultFrequencies != nullptr)
			free(fftResultFrequencies);

		fftResultData = static_cast<float*>(calloc(size, sizeof(float)));
		fftResultFrequencies = static_cast<float*>(calloc(size, sizeof(float)));
		fftResultDataSize = size;
	}

	memcpy_s(fftResultData, size * sizeof(float), data, size * sizeof(float));
	memcpy_s(fftResultFrequencies, size * sizeof(float), frequencies, size * sizeof(float));
	repaint();
}

void IIRFilterGraph::disableFFT()
{
	fftResultDataSize = 0;
	repaint();
}
