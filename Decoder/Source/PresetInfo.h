/*
  ==============================================================================

    PresetInfo.h
    Created: 30 Jul 2017 5:12:01pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../Common/AmbiSettings.h"
#include "../../Common/AmbiSpeaker.h"

#define XML_VALUE "Value"
#define XML_TAG_PRESET_ROOT "AmbisonicsPreset"
#define XML_TAG_PRESET_GENERAL "General"
#define XML_TAG_PRESET_POINTS "Points"
#define XML_TAG_PRESET_POINT "Point"
#define XML_TAG_PRESET_DISTANCESCALER "DistanceScaler"
#define XML_TAG_PRESET_FLIPDIRECTION "DirectionFlip"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT "AmbiChannelWeight"
#define XML_ATTRIBUTE_PRESET_NAME "Name"

class PresetInfo
{
public:
	PresetInfo();
	~PresetInfo();
	bool LoadFromXmlRoot(XmlElement* root);
	bool LoadFromFile(File file);
	void CreateXmlRoot(XmlElement* xmlRoot);
	bool SaveToFile(File file);
	OwnedArray<AmbiSpeaker>* getPoints();
	String getName() const;
	void setName(String newName);
	AmbiSettings* getAmbiSettings();

private:
	OwnedArray<AmbiSpeaker> points;
	AmbiSettings ambiSettings;
	String name;
};