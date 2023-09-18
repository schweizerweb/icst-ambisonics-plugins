/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once
#include "JuceHeader.h"
#define MAX_AMBISONICS_ORDER 7
#define MAX_NB_OF_AMBISONICS_GAINS (MAX_AMBISONICS_ORDER + 1)
#define MAX_NB_OF_AMBISONICS_CHANNELS (MAX_NB_OF_AMBISONICS_GAINS * MAX_NB_OF_AMBISONICS_GAINS)
#define MAX_NUM_CHANNELS    64

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
    int getAmbiChannelCount() const;
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
