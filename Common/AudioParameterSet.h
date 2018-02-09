/*
  ==============================================================================

    AudioParameterSet.h
    Created: 9 Feb 2018 12:29:36pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AudioParameterFloatAmbiAbs.h"

class AudioParameterSet
{
public:
	AudioParameterFloatAmbiAbs* pX = nullptr;
	AudioParameterFloatAmbiAbs* pY = nullptr;
	AudioParameterFloatAmbiAbs* pZ = nullptr;
	AudioParameterFloatAmbiAbs* pA = nullptr;
	AudioParameterFloatAmbiAbs* pE = nullptr;
	AudioParameterFloatAmbiAbs* pD = nullptr;

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
	
	void notifyA(double a) const
	{
		if (pA != nullptr) { pA->setUnscaledValue(float(a)); }
	}
	
	void notifyE(double e) const
	{
		if (pE != nullptr) { pE->setUnscaledValue(float(e)); }
	}

	void notifyD(double d) const
	{
		if (pD != nullptr) { pD->setUnscaledValue(float(d)); }
	}
};
