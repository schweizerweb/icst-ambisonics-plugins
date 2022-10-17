/*
  ==============================================================================

    AudioParameterSet.h
    Created: 9 Feb 2018 12:29:36pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "AudioParameterFloatAmbiAbs.h"
#include "AudioParameterBoolAmbiAbs.h"
#include "ScalingInfo.h"

class AudioParameterSet
{
public:
	AudioParameterFloatAmbiAbs* pX = nullptr;
	AudioParameterFloatAmbiAbs* pY = nullptr;
	AudioParameterFloatAmbiAbs* pZ = nullptr;
    AudioParameterFloat* pQ0 = nullptr;
    AudioParameterFloat* pQ1 = nullptr;
    AudioParameterFloat* pQ2 = nullptr;
    AudioParameterFloat* pQ3 = nullptr;
    AudioParameterFloatAmbiAbs* pGain = nullptr;
    AudioParameterBoolAmbiAbs* pMute = nullptr;
    
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
    
    double notifyQ0(double q) const
    {
        makeValid(&q, -1.0, 1.0);
        if(pQ0 != nullptr)
        {
            pQ0->setValueNotifyingHost(q);
        }
        
        return q;
    }
    
    double notifyQ1(double q) const
    {
        makeValid(&q, -1.0, 1.0);
        if(pQ1 != nullptr)
        {
            pQ1->setValueNotifyingHost(q);
        }
        
        return q;
    }
    
    double notifyQ2(double q) const
    {
        makeValid(&q, -1.0, 1.0);
        if(pQ2 != nullptr)
        {
            pQ2->setValueNotifyingHost(q);
        }
        
        return q;
    }
    
    double notifyQ3(double q) const
    {
        makeValid(&q, -1.0, 1.0);
        if(pQ3 != nullptr)
        {
            pQ3->setValueNotifyingHost(q);
        }
        
        return q;
    }
    
    void notifyGain(double gain)
    {
        if (pGain != nullptr) { pGain->setUnscaledValue((float)Decibels::gainToDecibels(gain)); }
    }
    
    void notifyMute(bool mute)
    {
        if (pMute != nullptr) { pMute->setUnscaledValue((float)mute); }
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
