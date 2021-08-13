/*
  ==============================================================================

    AmbiSettings.cpp
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "DecoderAmbiSettings.h"

DecoderAmbiSettings::DecoderAmbiSettings(): AmbiBasicSettings(DEFAULT_AMBISONICS_ORDER)
{
    // create mapping table
    int order = 0;
    for (int i = 0; i < MAX_NUM_INPUT_CHANNELS; i++)
    {
        if (i == (order + 1) * (order + 1))
            order++;
        ambiChannelOrder[i] = order;
    }

    prepareWeightings();
}

double DecoderAmbiSettings::getAmbiChannelWeight(int ambiChannel)
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

double* DecoderAmbiSettings::getManualOrderWeightPointer()
{
	return manualOrderWeights;
}

void DecoderAmbiSettings::loadFromPresetXml(XmlElement *xmlElement)
{
    // read weights first (since they will be applied by the setAmbisonicsOrder method)
    XmlElement* xmlAmbiChannelWeight = xmlElement->getChildByName(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    weightMode = AmbiWeightMode(xmlAmbiChannelWeight->getIntAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE, DecoderAmbiSettings::INPHASE));
    int index = 0;
    if (xmlAmbiChannelWeight != nullptr && weightMode == MANUAL)
    {
        String attributeName;
        while (xmlAmbiChannelWeight->hasAttribute(attributeName = "Order" + String(index)))
        {
            manualOrderWeights[index] = xmlAmbiChannelWeight->getDoubleAttribute(attributeName);
            index++;
        }
    }

    XmlElement* xmlAmbisonicsOrder = xmlElement->getChildByName(XML_TAG_PRESET_AMBISONICS_ORDER);
    if (xmlAmbisonicsOrder != nullptr)
    {
        setAmbisonicsOrder(xmlAmbisonicsOrder->getIntAttribute(XML_VALUE));
    }
    else
    {
        setAmbisonicsOrder(DEFAULT_AMBISONICS_ORDER);
    }
}

void DecoderAmbiSettings::setAmbisonicsOrder(int order)
{
    AmbiBasicSettings::setAmbisonicsOrder(order);
    prepareWeightings();
    setWeightMode(weightMode);
}

void DecoderAmbiSettings::writeToPresetXmlElement(XmlElement *xmlElement) const
{
    XmlElement* xmlAmbisonicsOrder = new XmlElement(XML_TAG_PRESET_AMBISONICS_ORDER);
    xmlAmbisonicsOrder->setAttribute(XML_VALUE, getAmbisonicsOrder());
    xmlElement->addChildElement(xmlAmbisonicsOrder);

    XmlElement* xmlAmbiChannelWeight = new XmlElement(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    xmlAmbiChannelWeight->setAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE, int(weightMode));
    if(weightMode == MANUAL)
    {
        for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
        {
            xmlAmbiChannelWeight->setAttribute("Order" + String(i), manualOrderWeights[i]);
        }
    }
    xmlElement->addChildElement(xmlAmbiChannelWeight);
}

void DecoderAmbiSettings::prepareWeightings()
{
    // manual
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        manualOrderWeights[i] = 1.0;
    }

    // standard
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        standardWeights[i] = 1.0;
    }

    // in-phase
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < getAmbisonicsOrder() + 1)
        {
            double nom = fact(getAmbisonicsOrder()) * fact(getAmbisonicsOrder() + 1);
            double denom = fact(getAmbisonicsOrder() + i + 1)*fact(getAmbisonicsOrder() - i);
            inPhaseWeights[i] = nom / denom;
        }
        else
        {
            inPhaseWeights[i] = 0.0;
        }
    }

    // max-re
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < getAmbisonicsOrder() + 1)
        {
            maxreWeights[i] = cos(MathConstants<double>::pi * i / (2 * (getAmbisonicsOrder() + 1)));
        }
        else
        {
            maxreWeights[i] = 0.0;
        }
    }
}


double DecoderAmbiSettings::fact(int n)
{
    if (n == 0)
        return 1;
    double ret = n;
    for (int i = n - 1; i > 1; i--)
        ret *= i;
    return ret;
}


DecoderAmbiSettings::AmbiWeightMode DecoderAmbiSettings::getWeightMode()
{
    return weightMode;
}

void DecoderAmbiSettings::setWeightMode(AmbiWeightMode mode)
{
    weightMode = mode;
    
    // copy values to the manual value array for display purpose
    if (mode == INPHASE)
        memcpy(&manualOrderWeights, &inPhaseWeights, MAX_NB_OF_AMBISONICS_GAINS * sizeof(double));
    else if (mode == BASIC)
        memcpy(&manualOrderWeights, &standardWeights, MAX_NB_OF_AMBISONICS_GAINS * sizeof(double));
    else if (mode == MAXRE)
        memcpy(&manualOrderWeights, &maxreWeights, MAX_NB_OF_AMBISONICS_GAINS * sizeof(double));
}
