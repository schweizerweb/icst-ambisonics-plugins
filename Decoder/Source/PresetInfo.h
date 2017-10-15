/*
  ==============================================================================

    PresetInfo.h
    Created: 30 Jul 2017 5:12:01pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/AmbiSettings.h"

#define XML_VALUE "Value"
#define XML_TAG_PRESET_ROOT "AmbisonicsPreset"
#define XML_TAG_PRESET_GENERAL "General"
#define XML_TAG_PRESET_POINTS "Points"
#define XML_TAG_PRESET_POINT "Point"
#define XML_TAG_PRESET_DISTANCESCALER "DistanceScaler"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT "AmbiChannelWeight"
#define XML_ATTRIBUTE_PRESET_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_X "X"
#define XML_ATTRIBUTE_PRESET_POINT_Y "Y"
#define XML_ATTRIBUTE_PRESET_POINT_Z "Z"
#define XML_ATTRIBUTE_PRESET_POINT_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_COLOR "Color"
#define XML_ATTRIBUTE_PRESET_POINT_GAIN "Gain"

class PresetInfo
{
public:
	PresetInfo();
	bool LoadFromFile(File file);
	bool SaveToFile(File file);
	OwnedArray<AmbiPoint>* getPoints();
	String getName() const;
	void setName(String newName);
	AmbiSettings* getAmbiSettings();

private:
	OwnedArray<AmbiPoint> points;
	ScopedPointer<AmbiSettings> ambiSettings;
	String name;
};