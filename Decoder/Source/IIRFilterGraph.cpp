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
#include "../../Common/Constants.h"

//==============================================================================

IIRFilterGraph::IIRFilterGraph(std::vector<FilterBankInfo*> _pFilterInfo, dsp::ProcessSpec* pFilterSpecification, std::vector<juce::Colour*> _pColors, OwnedArray<SingleFilterSettingsComponent>* pSingleFilterControls): usedFilterCount(1), pFilterControls(pSingleFilterControls), pFilterInfo(_pFilterInfo), pColors(_pColors), fftResultData(nullptr), fftResultDataSize(0), fftSize(0), fftScaler(0)
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

	setDisplayRange(LogarithmicFrequency, Range<double>(20, pFilterSpecification->sampleRate / 2.0), Linear, Range<double>(Constants::FilterGainDbMin, Constants::FilterGainDbMax));
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
	
	for (size_t iFilter = 0; iFilter < pFilterInfo.size() && iFilter < (size_t)usedFilterCount; iFilter++)
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
    rebuildHandles();
    repaint();
}

void IIRFilterGraph::rebuildHandles()
{
    handles.clear();

    for (size_t bankIndex = 0; bankIndex < pFilterInfo.size(); ++bankIndex)
    {
        auto& bank = *pFilterInfo[bankIndex];

        for (int i = 0; i < MAX_FILTER_COUNT; ++i)
        {
            if (bank.isActive (i))
            {
                auto handle = std::make_unique<FilterHandleComponent> (*this, bank, i);
                addAndMakeVisible (*handle);
                handles.push_back (std::move (handle));
            }
        }
    }

    updateHandlePositions();
}

juce::Point<float> IIRFilterGraph::valueToPoint (float frequencyHz, float gainDb) const
{
    const auto x = mapValues (frequencyHz, gainDb);
    return x.toFloat();
}

float IIRFilterGraph::xToFrequency (float screenX) const
{
    double xNorm = (screenX - graphArea->getX()) / (double) graphArea->getWidth();
    xNorm = juce::jlimit (0.0, 1.0, xNorm);
    
    if (getScalingModeX() == LogarithmicFrequency)
        return (float) std::pow (10.0, displayRangeX->getStart() + xNorm * displayRangeX->getLength());
    
    return (float) (displayRangeX->getStart() + xNorm * displayRangeX->getLength());
}

float IIRFilterGraph::yToGainDb (float screenY) const
{
    double yNorm = 1.0 - ((screenY - graphArea->getY()) / (double) graphArea->getHeight());
    yNorm = juce::jlimit (0.0, 1.0, yNorm);
    
    if (getScalingModeY() == LogarithmicFrequency)
        return (float) std::pow (10.0, displayRangeY->getStart() + yNorm * displayRangeY->getLength());
    
    return (float) (displayRangeY->getStart() + yNorm * displayRangeY->getLength());
}

void IIRFilterGraph::dragHandleTo (FilterBankInfo& bank, int filterIndex, juce::Point<float> localPoint)
{
    auto* f = bank.get (filterIndex);
    if (f == nullptr)
        return;

    f->cutOffFrequencyHz = juce::jlimit (20.0f, (float) (sampleRate * 0.5f), xToFrequency (localPoint.x));
    f->gainFactor = juce::Decibels::decibelsToGain (f->gainRequired() ? yToGainDb (localPoint.y) : 0.0f);

    if(pFilterControls != nullptr && filterIndex < pFilterControls->size())
        pFilterControls->getUnchecked(filterIndex)->updateUi();
    
    updateHandlePositions();
    repaint();
}

void IIRFilterGraph::updateHandlePositions()
{
    int handleIdx = 0;

    for (size_t bankIndex = 0; bankIndex < pFilterInfo.size(); ++bankIndex)
    {
        auto& bank = *pFilterInfo[bankIndex];

        for (int i = 0; i < MAX_FILTER_COUNT; ++i)
        {
            auto* f = bank.get (i);
            if (f == nullptr || f->filterType == FilterInfo::None)
                continue;

            if (handleIdx >= (int) handles.size())
                return;

            auto p = valueToPoint (f->cutOffFrequencyHz, f->gainRequired() ? Decibels::gainToDecibels(f->gainFactor) : 0.0);
            auto bounds = juce::Rectangle<int> ((int) std::round (p.x) - 7,
                                                (int) std::round (p.y) - 7,
                                                14, 14);

            handles[(size_t) handleIdx]->setBounds (bounds);

            if(pFilterControls != nullptr && i < pFilterControls->size())
            {
                auto* c = pFilterControls->getUnchecked(i);
                handles[(size_t) handleIdx]->setVisual (c->getColor(), 1.0f);
                handles[(size_t) handleIdx]->setFilterComponent(c);
            }
            else
            {
                juce::Colour c = (bankIndex < pColors.size() && pColors[bankIndex] != nullptr)
                ? *pColors[bankIndex]
                : juce::Colours::lightgreen;
                
                handles[(size_t) handleIdx]->setVisual (c, bank.getFilterBypass());
            }
            
            
            ++handleIdx;
        }
    }
}
