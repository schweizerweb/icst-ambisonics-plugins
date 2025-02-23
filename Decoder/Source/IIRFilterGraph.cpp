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



#include "../JuceLibraryCode/JuceHeader.h"
#include "IIRFilterGraph.h"

//==============================================================================

IIRFilterGraph::IIRFilterGraph(std::vector<FilterBankInfo*> _pFilterInfo, dsp::ProcessSpec* pFilterSpecification, std::vector<juce::Colour*> _pColors): pFilterInfo(_pFilterInfo), pColors(_pColors), fftResultData(nullptr), fftResultDataSize(0), fftSize(0), fftScaler(0), usedFilterCount(1)
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
		magnitudes[i] = static_cast<double*>(calloc((size_t)frequencies.size(), sizeof(double)));
	}

	setDisplayRange(LogarithmicFrequency, Range<double>(20, pFilterSpecification->sampleRate / 2.0), Linear, Range<double>(-50, 50));
	fullGridFlag = true;
	labelAxisY = "Gain [dB]";
	labelAxisX = "Frequency [Hz]";
	usedFilterCount = (int)pFilterInfo.size();
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
}

void IIRFilterGraph::paintData(Graphics& g)
{
	// draw curve
	
	for (int iFilter = 0; iFilter < pFilterInfo.size() && iFilter < usedFilterCount; iFilter++)
	{
		FilterBankInfo* pFilter = pFilterInfo[iFilter];
		Path path;

		if (!pFilter->anyActive())
		{
			path.startNewSubPath(mapValues(frequencies.getFirst(), 0.0).toFloat());
			path.lineTo(mapValues(frequencies.getLast(), 0.0).toFloat());
		}
		else
		{
			std::vector<dsp::IIR::Coefficients<float>::Ptr> coeffs;
			pFilter->getCoefficients(sampleRate, &coeffs);
			int activeFilterCount = int(coeffs.size());
			for (int iCoeff = 0; iCoeff < activeFilterCount && iCoeff < MAX_FILTER_COUNT; iCoeff++)
			{
				coeffs[(size_t)iCoeff]->getMagnitudeForFrequencyArray(frequencies.getRawDataPointer(), magnitudes[iCoeff], (size_t)frequencies.size(), sampleRate);
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

		Colour color;
		if (pColors.size() > iFilter)
		{
			color = *(pColors[iFilter]);
		}
		else
		{
			color = Colours::lightgreen;
		}

		if (pFilter->getFilterBypass())
		{
			color = color.withAlpha(0.3f);
		}

		g.setColour(color);

		g.strokePath(path, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));
	}


	// FFT
	
	if(fftResultDataSize > 0)
	{
        const double sampleRateVsFFTSize = (sampleRate / fftSize);
		g.setColour(Colours::blueviolet.withAlpha(0.5f));
	    for(int i = 0; i < fftResultDataSize; i++)
	    {
			Point<float> topLeft = mapValues((i - 0.5) * sampleRateVsFFTSize, fftResultData[i] + fftScaler).toFloat();
			Point<float> bottomRight = mapValues((i + 0.5) * sampleRateVsFFTSize, displayRangeY->getStart()).toFloat();
            if (bottomRight.getY() > topLeft.getY())
			{
				g.fillRect(Rectangle<float>(topLeft, bottomRight));
			}
	    }
	}
}

void IIRFilterGraph::setFFTResult(float* data, int size, int newFftSize)
{
	this->fftSize = newFftSize;
	if(size != fftResultDataSize)
	{
		if (fftResultData != nullptr)
			free(fftResultData);
		
		fftResultData = static_cast<float*>(calloc((size_t)size, sizeof(float)));
		fftResultDataSize = size;
	}

	if (fftResultData != nullptr)
	{
		memcpy(fftResultData, data, (size_t)size * sizeof(float));
		repaint();
	}
}

void IIRFilterGraph::setFFTParams(bool enable, double scaler)
{
	if(enable)
	{
		fftScaler = scaler;
	}
	else
	{
		fftResultDataSize = 0;
	}

	repaint();
}

void IIRFilterGraph::setUsedFiltersCount(int filterCount)
{
	usedFilterCount = filterCount;
	repaint();
}
