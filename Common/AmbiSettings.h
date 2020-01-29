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
#define XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE "AmbiChannelWeightMode"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT_PLUGIN_ORDER "AmbiPluginOrder"
#define XML_VALUE "Value"

class AmbiSettings: public AmbiBasicSettings
{
public:
	AmbiSettings();
	AmbiSettings(double distanceScaler, bool directionFlip);

    enum AmbiWeightMode { STANDARD = 1, INPHASE = 2, MANUAL = 3 };
    
	double getAmbiChannelWeight(int ambiChannel);
	double* getAmbiOrderWeightPointer();
    AmbiWeightMode getWeightMode();
    void setWeightMode(AmbiWeightMode mode);
    
    void writeToPresetXmlElement(XmlElement* xmlElement) const;
    void loadFromPresetXml(XmlElement* xmlElement);

    bool getWarningFlag();
    
private:
	double fact(int n);
    void prepareInPhaseWeighting();
    void prepareStandardWeighting();
    void prepareManualWeighting();
    
    int ambiChannelOrder[NB_OF_AMBISONICS_CHANNELS];
	double manualOrderWeights[NB_OF_AMBISONICS_GAINS];
    
    double inPhaseWeights[NB_OF_AMBISONICS_GAINS];
    double standardWeights[NB_OF_AMBISONICS_GAINS];
    
    AmbiWeightMode weightMode;
    bool loadWarningFlag;
};
