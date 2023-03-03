/*
  ==============================================================================

    RadarOptions.h
    Created: 24 Dec 2017 3:11:46pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AudioParams.h"
#include "DawParameter.h"
#include "ScalingInfo.h"
#include "ZoomSettings.h"

class RadarOptions
{
public:
	RadarOptions();
	AudioParameterSet getAudioParamForIndex(int index, bool isGroup) const;

public:
	int maxNumberEditablePoints;
	bool editablePointsAsSquare;
	int displayTimeout;
	bool showEditablePoints;
	bool showDisplayOnlyPoints;
	bool setTrackColorAccordingToName;
    bool checkNameFieldEditable;
    
    AudioParams* audioParams;
	DawParameter* dawParameter;
    ScalingInfo* scalingInfo;
    ZoomSettings* zoomSettings;
};
