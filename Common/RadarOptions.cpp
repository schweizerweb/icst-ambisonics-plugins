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
	dawParameter = nullptr;
	setTrackColorAccordingToName = true;
}

AudioParameterSet RadarOptions::getAudioParamForIndex(int index, bool isGroup) const
{
    if(isGroup && audioParams != nullptr && audioParams->groupParams.size()>index)
    {
        return audioParams->groupParams.getUnchecked(index);
    }
	if(!isGroup && audioParams != nullptr && audioParams->sourceParams.size()>index)
	{
		return audioParams->sourceParams.getUnchecked(index);
	}

	return AudioParameterSet();
}
