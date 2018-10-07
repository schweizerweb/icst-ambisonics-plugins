/*
  ==============================================================================

    AudioParameterSet.h
    Created: 9 Feb 2018 12:29:36pm
    Author:  Christian Schweizer

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
};
