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



#pragma once
#include "AudioParameterFloatAmbiAbs.h"
#include "AudioParameterBoolAmbiAbs.h"
#include "ScalingInfo.h"
#include "Constants.h"

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
    AudioParameterFloat* pStretch = nullptr;
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
    
    void notifyQ(double q0, double q1, double q2, double q3)
    {
        makeValid(&q0, -1.0, 1.0);
        if(pQ0 != nullptr)
        {
            *pQ0 = float(q0);
        }
        
        makeValid(&q1, -1.0, 1.0);
        if(pQ1 != nullptr)
        {
            *pQ1 = float(q1);
        }
        
        makeValid(&q2, -1.0, 1.0);
        if(pQ2 != nullptr)
        {
            *pQ2 = float(q2);
        }
        
        makeValid(&q3, -1.0, 1.0);
        if(pQ3 != nullptr)
        {
            *pQ3 = float(q3);
        }
    }
    
    double notifyStretch(double stretch)
    {
        makeValid(&stretch, Constants::StretchMin, Constants::StretchMax);
        if(pStretch != nullptr)
        {
            *pStretch = float(stretch);
        }
        
        return stretch;
    }
    
    void notifyGain(double gain)
    {
        if (pGain != nullptr) { pGain->setUnscaledValue((float)Decibels::gainToDecibels(gain)); }
    }
    
    void notifyMute(bool mute)
    {
        if (pMute != nullptr) { pMute->setUnscaledValue(mute); }
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
