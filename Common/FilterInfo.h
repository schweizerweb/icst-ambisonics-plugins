/*
  ==============================================================================

    FilterInfo.h
    Created: 30 Oct 2019 4:28:04pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#define DEFAULT_Q	1.0
#define DEFAULT_FREQUENCY 200
#define DEFAULT_GAIN_FACTOR 1

class FilterInfo
{
public:
	FilterInfo(): filterType(None), cutOffFrequencyHz(DEFAULT_FREQUENCY), qValue(DEFAULT_Q), gainFactor(DEFAULT_GAIN_FACTOR)
	{
	}

	dsp::IIR::Coefficients<float>::Ptr getCoefficients(double sampleRate) const
	{
		switch (filterType)
		{
		case LowPass:
			return dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutOffFrequencyHz, qValue);
		case BandPass:
			return dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, cutOffFrequencyHz, qValue);
		case HighPass:
			return dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, cutOffFrequencyHz, qValue);
		case FirstOrderLowPass:
			return dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(sampleRate, cutOffFrequencyHz);
		case FirstOrderHighPass:
			return dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(sampleRate, cutOffFrequencyHz);
		case Notch:
			return dsp::IIR::Coefficients<float>::makeNotch(sampleRate, cutOffFrequencyHz, qValue);
        case LowShelf:
            return dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, cutOffFrequencyHz, qValue, gainFactor);
        case HighShelf:
            return dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, cutOffFrequencyHz, qValue, gainFactor);
        case Peak:
            return dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, cutOffFrequencyHz, qValue, gainFactor);
		case None:
		default:
			return nullptr;
		}
	}

	bool qRequired() const
	{
		return filterType == LowPass || filterType == BandPass || filterType == HighPass || filterType == Notch || filterType == LowShelf || filterType == HighShelf || filterType == Peak;
	}

    bool gainRequired() const
    {
        return filterType == HighShelf || filterType == LowShelf || filterType == Peak;
    }
    
	bool isLowPass() const
	{
		return filterType == LowPass || filterType == FirstOrderLowPass;
	}

	enum FilterType { None, LowPass, BandPass, HighPass, FirstOrderLowPass, FirstOrderHighPass, Notch, LowShelf, HighShelf, Peak } filterType;
	float cutOffFrequencyHz;
	float qValue;
    float gainFactor;
	void copyFrom(FilterInfo* info);
};
