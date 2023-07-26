/*
  ==============================================================================

    AmbiSettings.h
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#define MAX_AMBISONICS_ORDER 7
#define MAX_NB_OF_AMBISONICS_GAINS (MAX_AMBISONICS_ORDER + 1)
#define MAX_NB_OF_AMBISONICS_CHANNELS (MAX_NB_OF_AMBISONICS_GAINS * MAX_NB_OF_AMBISONICS_GAINS)

#define XML_TAG_PRESET_AMBICHANNELWEIGHT "AmbiChannelWeight"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE "AmbiChannelWeightMode"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT_PLUGIN_ORDER "AmbiPluginOrder"
#define XML_VALUE "Value"

class AmbiSettings
{
public:
	AmbiSettings();
	
    enum AmbiWeightMode { BASIC = 1, INPHASE = 2, MANUAL = 3, MAXRE = 4 };
    
	double getAmbiChannelWeight(int ambiChannel);
	double* getAmbiOrderWeightPointer();
    AmbiWeightMode getWeightMode();
    void setWeightMode(AmbiWeightMode mode);
    
    void writeToPresetXmlElement(XmlElement* xmlElement) const;
    void loadFromPresetXml(XmlElement* xmlElement);

    int getGainCount();
    int getAmbiOrder();
    void setAmbiOrder(int order);
    
    bool getWarningFlag();
    
private:
	double fact(int n);
    void prepareAutoWeightings();
    void prepareManualWeighting();
    
    int ambiChannelOrder[MAX_NB_OF_AMBISONICS_CHANNELS];
	double manualOrderWeights[MAX_NB_OF_AMBISONICS_GAINS];
    
    double inPhaseWeights[MAX_NB_OF_AMBISONICS_GAINS];
    double maxreWeights[MAX_NB_OF_AMBISONICS_GAINS];
    double standardWeights[MAX_NB_OF_AMBISONICS_GAINS];

    int ambiOrder;
    
    AmbiWeightMode weightMode;
    bool loadWarningFlag;
};
