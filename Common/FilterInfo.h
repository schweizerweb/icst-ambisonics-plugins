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

class FilterInfo
{
public:
	FilterInfo(): filterType(None), cutOffFrequencyHz(DEFAULT_FREQUENCY), qValue(DEFAULT_Q)
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
		case None:
		default:
			return nullptr;
		}
	}

	bool qRequired() const
	{
		return filterType == LowPass || filterType == BandPass || filterType == HighPass || filterType == Notch;
	}

	bool isLowPass() const
	{
		return filterType == LowPass || filterType == FirstOrderLowPass;
	}

	enum FilterType { None, LowPass, BandPass, HighPass, FirstOrderLowPass, FirstOrderHighPass, Notch } filterType;
	float cutOffFrequencyHz;
	float qValue;
	void copyFrom(FilterInfo* info);
};
