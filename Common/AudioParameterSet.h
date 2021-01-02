/*
  ==============================================================================

    AudioParameterSet.h
    Created: 9 Feb 2018 12:29:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "AudioParameterFloatAmbiAbs.h"
#include "Constants.h"

class AudioParameterSet
{
public:
	AudioParameterFloatAmbiAbs* pX = nullptr;
	AudioParameterFloatAmbiAbs* pY = nullptr;
	AudioParameterFloatAmbiAbs* pZ = nullptr;
    AudioParameterFloatAmbiAbs* pGain = nullptr;
	
	void notifyX(double x) const
	{
		if (pX != nullptr) { pX->setUnscaledValue(float(x)); }
	}

	void notifyY(double y) const
	{
		if (pY != nullptr) { pY->setUnscaledValue(float(y)); }
	}

	void notifyZ(double z) const
	{
		if (pZ != nullptr) { pZ->setUnscaledValue(float(z)); }
	}
    
    void notifyGain(double gain)
    {
        if (pGain != nullptr) { pGain->setUnscaledValue((float)Decibels::gainToDecibels(gain)); }
    }
    
    void setEnabled(bool enable)
    {
        if(pX != nullptr) pX->setEnabled(enable);
        if(pY != nullptr) pY->setEnabled(enable);
        if(pZ != nullptr) pZ->setEnabled(enable);
    }

    void notifyStartGesture() const
    {
        if (pX != nullptr) pX->beginChangeGesture();
        if (pY != nullptr) pY->beginChangeGesture();
        if (pZ != nullptr) pZ->beginChangeGesture();
	}

    void notifyEndGesture() const
	{
        if (pX != nullptr) pX->endChangeGesture();
        if (pY != nullptr) pY->endChangeGesture();
        if (pZ != nullptr) pZ->endChangeGesture();
	}
};
