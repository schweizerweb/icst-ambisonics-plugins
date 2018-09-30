/*
  ==============================================================================

    RadarOptions.cpp
    Created: 24 Dec 2017 3:11:46pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "RadarOptions.h"

RadarOptions::RadarOptions()
{
	nameFieldEditable = false;
	maxNumberEditablePoints = -1;
	audioParams = nullptr;
	displayTimeout = 1000;
	editablePointsAsSquare = false;
	showDisplayOnlyPoints = true;
	showEditablePoints = true;
}

AudioParameterSet RadarOptions::getAudioParamForIndex(int index) const
{
	if(audioParams != nullptr && audioParams->size()>index)
	{
		return audioParams->getUnchecked(index);
	}

	return AudioParameterSet();
}
