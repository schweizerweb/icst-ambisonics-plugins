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
#define DEFAULT_FILTER_TYPE None
#define DEFAULT_BYPASS_FILTER true

class FilterInfo
{
public:
	FilterInfo(): filterType(DEFAULT_FILTER_TYPE), cutOffFrequencyHz(DEFAULT_FREQUENCY), qValue(DEFAULT_Q), gainFactor(DEFAULT_GAIN_FACTOR)
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
    
    bool frequencyRequired() const
    {
        return filterType != None;
    }
    
    float defaultFrequency()
    {
        switch(filterType)
        {
            case LowPass: return 35.0f;
            case BandPass: return 50.0f;
            case HighPass: return 13000.0f;
            case FirstOrderLowPass: return 35.0f;
            case FirstOrderHighPass: return 13000.0f;
            case Notch: return 300.0f;
            case LowShelf: return 150.0f;
            case HighShelf: return 3200.0f;
            case Peak: return 2000.0f;
            default: return 200.0f;
        }
    }
    
    float defaultQ()
    {
        switch(filterType)
        {
            case LowPass: return 0.75f;
            case BandPass: return 15.0f;
            case HighPass: return 0.5f;
            case Notch: return 1.25f;
            case LowShelf: return 1.8f;
            case HighShelf: return 1.8f;
            case Peak: return 10.0f;
            default: return 1.0f;
        }
    }
    
    float defaultGainFactor()
    {
        switch(filterType)
        {
            case LowShelf: return 0.1f;
            case HighShelf: return 0.1f;
            case Peak: return 2.0f;
            default: return 1.0f;
        }
    }
    
	bool isLowPass() const
	{
		return filterType == LowPass || filterType == FirstOrderLowPass;
	}
    
    bool loadFromXmlElement(XmlElement* xmlElement)
    {
        filterType = (FilterType)xmlElement->getIntAttribute("filterType", DEFAULT_FILTER_TYPE);
        cutOffFrequencyHz = (float)xmlElement->getDoubleAttribute("cutOffFrequencyHz", DEFAULT_FREQUENCY);
        qValue = (float)xmlElement->getDoubleAttribute("qValue", DEFAULT_Q);
        gainFactor = (float)xmlElement->getDoubleAttribute("gainFactor", DEFAULT_GAIN_FACTOR);
        
        return true;
    }
    
    bool writeToXmlElement(XmlElement* xmlElement)
    {
        xmlElement->setAttribute("filterType", (int)filterType);
        xmlElement->setAttribute("cutOffFrequencyHz", cutOffFrequencyHz);
        xmlElement->setAttribute("qValue", qValue);
        xmlElement->setAttribute("gainFactor", gainFactor);
        
        return true;
    }

    void copyFrom(FilterInfo* info)
    {
        filterType = info->filterType;
        cutOffFrequencyHz = info->cutOffFrequencyHz;
        qValue = info->qValue;
        gainFactor = info->gainFactor;
    }
    
    bool equals(FilterInfo* info)
    {
        return info != nullptr
        && info->filterType == filterType
        && info->cutOffFrequencyHz == cutOffFrequencyHz
        && info->qValue == qValue
        && info->gainFactor == gainFactor;
    }

	enum FilterType { None, LowPass, BandPass, HighPass, FirstOrderLowPass, FirstOrderHighPass, Notch, LowShelf, HighShelf, Peak } filterType;
	float cutOffFrequencyHz;
	float qValue;
    float gainFactor;
};
