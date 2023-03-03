/*
  ==============================================================================

    AmbiSettings.cpp
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiSettings.h"

AmbiSettings::AmbiSettings()
{
    // create mapping table
	int order = 0;
	for (int i = 0; i < NB_OF_AMBISONICS_CHANNELS; i++)
	{
		if (i == (order + 1)*(order + 1))
			order++;
		ambiChannelOrder[i] = order;
	}
    
    loadWarningFlag = false;
    prepareInPhaseWeighting();
    prepareMaxreWeighting();
    prepareStandardWeighting();
    prepareManualWeighting();
}

double AmbiSettings::getAmbiChannelWeight(int ambiChannel)
{
    switch(weightMode)
    {
        case INPHASE:
            return inPhaseWeights[ambiChannelOrder[ambiChannel]];
        case BASIC:
            return standardWeights[ambiChannelOrder[ambiChannel]];
        case MAXRE:
            return maxreWeights[ambiChannelOrder[ambiChannel]];
        case MANUAL:
        default:
            return manualOrderWeights[ambiChannelOrder[ambiChannel]];
    }
}

double* AmbiSettings::getAmbiOrderWeightPointer()
{
	return manualOrderWeights;
}

void AmbiSettings::loadFromPresetXml(XmlElement *xmlElement)
{
    loadWarningFlag = false;
    // ambisonics settings
    
    XmlElement* xmlAmbiChannelWeight = xmlElement->getChildByName(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    weightMode = AmbiWeightMode(xmlAmbiChannelWeight->getIntAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE, AmbiSettings::INPHASE));
    int index = 0;
    if (xmlAmbiChannelWeight != nullptr && weightMode == MANUAL)
    {
        String attributeName;
        while (xmlAmbiChannelWeight->hasAttribute(attributeName = "Order" + String(index)))
        {
            manualOrderWeights[index] = xmlAmbiChannelWeight->getDoubleAttribute(attributeName);
            index++;
        }
    
        int presetOrder = xmlAmbiChannelWeight->getIntAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_PLUGIN_ORDER, -1);
        loadWarningFlag = (presetOrder != CURRENT_AMBISONICS_ORDER);
    }
    else
    {
        setWeightMode(weightMode);
    }
}

void AmbiSettings::writeToPresetXmlElement(XmlElement *xmlElement) const
{
    XmlElement* xmlAmbiChannelWeight = new XmlElement(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    xmlAmbiChannelWeight->setAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE, int(weightMode));
    if(weightMode == MANUAL)
    {
        for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
        {
            xmlAmbiChannelWeight->setAttribute("Order" + String(i), manualOrderWeights[i]);
        }
        xmlAmbiChannelWeight->setAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_PLUGIN_ORDER, CURRENT_AMBISONICS_ORDER);
    }
    xmlElement->addChildElement(xmlAmbiChannelWeight);
}

void AmbiSettings::prepareManualWeighting()
{
    for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
    {
        manualOrderWeights[i] = 1.0;
    }
}

void AmbiSettings::prepareStandardWeighting()
{
    for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
    {
        standardWeights[i] = 1.0;
    }
}

void AmbiSettings::prepareInPhaseWeighting()
{
    for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < CURRENT_AMBISONICS_ORDER_NB_OF_GAINS)
        {
            double nom = fact(CURRENT_AMBISONICS_ORDER) * fact(CURRENT_AMBISONICS_ORDER + 1);
            double denom = fact(CURRENT_AMBISONICS_ORDER + i + 1)*fact(CURRENT_AMBISONICS_ORDER - i);
            inPhaseWeights[i] = nom / denom;
        }
        else
        {
            inPhaseWeights[i] = 0.0;
        }
    }
}

void AmbiSettings::prepareMaxreWeighting()
{
    for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < CURRENT_AMBISONICS_ORDER_NB_OF_GAINS)
        {
            maxreWeights[i] = cos(MathConstants<double>::pi * i / (2 * (CURRENT_AMBISONICS_ORDER + 1)));
        }
        else
        {
            maxreWeights[i] = 0.0;
        }
    }
}


double AmbiSettings::fact(int n)
{
    if (n == 0)
        return 1;
    double ret = n;
    for (int i = n - 1; i > 1; i--)
        ret *= i;
    return ret;
}


AmbiSettings::AmbiWeightMode AmbiSettings::getWeightMode()
{
    return weightMode;
}

bool AmbiSettings::getWarningFlag()
{
    return loadWarningFlag;
}

void AmbiSettings::setWeightMode(AmbiSettings::AmbiWeightMode mode)
{
    weightMode = mode;
    
    // copy values to the manual value array for display purpose
    if (mode == INPHASE)
        memcpy(&manualOrderWeights, &inPhaseWeights, NB_OF_AMBISONICS_GAINS * sizeof(double));
    else if (mode == BASIC)
        memcpy(&manualOrderWeights, &standardWeights, NB_OF_AMBISONICS_GAINS * sizeof(double));
    else if (mode == MAXRE)
        memcpy(&manualOrderWeights, &maxreWeights, NB_OF_AMBISONICS_GAINS * sizeof(double));
}
