/*
  ==============================================================================

    AmbiSettings.h
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AmbiBasicSettings.h"
#define MAX_AMBISONICS_ORDER 7
#define NB_OF_AMBISONICS_GAINS (MAX_AMBISONICS_ORDER + 1)
#define NB_OF_AMBISONICS_CHANNELS (NB_OF_AMBISONICS_GAINS * NB_OF_AMBISONICS_GAINS)
#define CURRENT_AMBISONICS_ORDER_NB_OF_GAINS	int(sqrt(JucePlugin_MaxNumInputChannels))
#define CURRENT_AMBISONICS_ORDER	(CURRENT_AMBISONICS_ORDER_NB_OF_GAINS - 1)

#define XML_TAG_PRESET_DISTANCESCALER "DistanceScaler"
#define XML_TAG_PRESET_FLIPDIRECTION "DirectionFlip"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT "AmbiChannelWeight"
#define XML_VALUE "Value"

class AmbiSettings: public AmbiBasicSettings
{
public:
	AmbiSettings();
	AmbiSettings(double distanceScaler, bool directionFlip);

	double getAmbiChannelWeight(int ambiChannel);
	double* getAmbiOrderWeightPointer();
        
    void writeToPresetXmlElement(XmlElement* xmlElement) const;
    void loadFromPresetXml(XmlElement* xmlElement);

	
private:
	double* ambiChannelWeights[NB_OF_AMBISONICS_CHANNELS];
	double ambiOrderWeights[NB_OF_AMBISONICS_GAINS];
};
