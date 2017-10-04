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

#define XML_TAG_PRESET_ROOT "SpeakerPreset"
#define XML_TAG_PRESET_POINTS "Points"
#define XML_TAG_PRESET_POINT "Point"
#define XML_ATTRIBUTE_PRESET_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_X "X"
#define XML_ATTRIBUTE_PRESET_POINT_Y "Y"
#define XML_ATTRIBUTE_PRESET_POINT_Z "Z"
#define XML_ATTRIBUTE_PRESET_POINT_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_COLOR "Color"

class PresetInfo
{
public:
	bool LoadFromFile(File file);
	bool SaveToFile(File file);
	OwnedArray<AmbiPoint>* getPoints();
	String getName() const;
	void setName(String newName);

private:
	OwnedArray<AmbiPoint> points;
	String name;
};