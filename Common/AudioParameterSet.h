/*
  ==============================================================================

    AudioParameterSet.h
    Created: 9 Feb 2018 12:29:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "AudioParameterFloatAmbiAbs.h"
#include "ScalingInfo.h"

class AudioParameterSet
{
public:
	AudioParameterFloatAmbiAbs* pX = nullptr;
	AudioParameterFloatAmbiAbs* pY = nullptr;
	AudioParameterFloatAmbiAbs* pZ = nullptr;
    AudioParameterFloatAmbiAbs* pGain = nullptr;
    ScalingInfo* pScaling = nullptr;    // make sure to set this pointer to the scaling info of the plugin instance!
    
	double notifyX(double x) const
	{
        if(pScaling != nullptr)
        {
            makeValid(&x, pScaling->CartesianMin(), pScaling->CartesianMax());
            double compressedX = pScaling->compress(x);
        
            if (pX != nullptr)
            {
                pX->setUnscaledValue(float(compressedX));
            }
        }
        
        return x;
	}

	double notifyY(double y) const
	{
        if(pScaling != nullptr)
        {
            makeValid(&y, pScaling->CartesianMin(), pScaling->CartesianMax());
            double compressedY = pScaling->compress(y);
        
            if (pY != nullptr)
            {
                pY->setUnscaledValue(float(compressedY));
            }
        }
        
        return y;
	}

	double notifyZ(double z) const
	{
        if(pScaling != nullptr)
        {
            makeValid(&z, pScaling->CartesianMin(), pScaling->CartesianMax());
            double compressedZ = pScaling->compress(z);
        
            if (pZ != nullptr)
            {
                pZ->setUnscaledValue(float(compressedZ));
            }
        }
        
        return z;
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
    
    void makeValid(double* pValue, const double min, const double max) const
    {
        *pValue = jmin(*pValue, max);
        *pValue = jmax(*pValue, min);
    }
};
