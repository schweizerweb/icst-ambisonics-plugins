/*
  ==============================================================================

    RadarOptions.cpp
    Created: 24 Dec 2017 3:11:46pm
    Author:  chris

  ==============================================================================
*/

#include "RadarOptions.h"

RadarOptions::RadarOptions()
{
	nameFieldEditable = false;
	maxNumberEditablePoints = -1;
	audioParams = nullptr;
}

AudioParameterSet RadarOptions::getAudioParamForIndex(int index) const
{
	if(audioParams != nullptr && audioParams->size()>index)
	{
		return audioParams->getUnchecked(index);
	}

	return AudioParameterSet();
}
