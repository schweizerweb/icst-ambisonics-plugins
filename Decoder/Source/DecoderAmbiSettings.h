/*
  ==============================================================================

    AmbiSettings.h
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiBasicSettings.h"

#define MAX_NB_OF_AMBISONICS_GAINS 8 /*7th order*/
#define MAX_NUM_INPUT_CHANNELS 64
#define MAX_NUM_OUTPUT_CHANNELS 64

#define XML_TAG_PRESET_AMBISONICS_ORDER "AmbisonicsOrder"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT "AmbiChannelWeight"
#define XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE "AmbiChannelWeightMode"
#define XML_VALUE "Value"

class DecoderAmbiSettings: public AmbiBasicSettings
{
public:
    DecoderAmbiSettings();
    
    enum AmbiWeightMode { BASIC = 1, INPHASE = 2, MANUAL = 3, MAXRE = 4 };
    
	double getAmbiChannelWeight(int ambiChannel);
	double* getManualOrderWeightPointer();
    AmbiWeightMode getWeightMode();
    void setWeightMode(AmbiWeightMode mode);
    
    void writeToPresetXmlElement(XmlElement* xmlElement) const;
    void loadFromPresetXml(XmlElement* xmlElement);
    void setAmbisonicsOrder(int order) override;

private:
	double fact(int n);
    void prepareWeightings();
    
    int ambiChannelOrder[MAX_NUM_INPUT_CHANNELS];
	double manualOrderWeights[MAX_NB_OF_AMBISONICS_GAINS];
    
    double inPhaseWeights[MAX_NB_OF_AMBISONICS_GAINS];
    double maxreWeights[MAX_NB_OF_AMBISONICS_GAINS];
    double standardWeights[MAX_NB_OF_AMBISONICS_GAINS];
    
    AmbiWeightMode weightMode;
};
