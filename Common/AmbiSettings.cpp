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



#include "AmbiSettings.h"

AmbiSettings::AmbiSettings()
{
    // create mapping table
	int order = 0;
	for (int i = 0; i < MAX_NB_OF_AMBISONICS_CHANNELS; i++)
	{
		if (i == (order + 1)*(order + 1))
			order++;
		ambiChannelOrder[i] = order;
	}
    ambiOrder = 1;
    prepareAutoWeightings();
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

bool AmbiSettings::loadFromPresetXml(XmlElement *xmlElement)
{
    bool loadWarningFlag = false;
    // ambisonics settings
    
    setAmbiOrder(xmlElement->getIntAttribute(XML_ATTRIBUTE_AMBI_ORDER, DEAFULT_AMBI_ORDER));
    XmlElement* xmlAmbiChannelWeight = xmlElement->getChildByName(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    weightMode = AmbiWeightMode(xmlAmbiChannelWeight->getIntAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE, DEFAULT_WEIGHT_MODE));
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
        loadWarningFlag = (presetOrder != ambiOrder);
    }
    else
    {
        setWeightMode(weightMode);
    }

    return loadWarningFlag;
}

void AmbiSettings::writeToPresetXmlElement(XmlElement *xmlElement) const
{
    xmlElement->setAttribute(XML_ATTRIBUTE_AMBI_ORDER, ambiOrder);
    XmlElement* xmlAmbiChannelWeight = new XmlElement(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    xmlAmbiChannelWeight->setAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_MODE, int(weightMode));
    
    if(weightMode == MANUAL)
    {
        for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
        {
            xmlAmbiChannelWeight->setAttribute("Order" + String(i), manualOrderWeights[i]);
        }
        xmlAmbiChannelWeight->setAttribute(XML_TAG_PRESET_AMBICHANNELWEIGHT_PLUGIN_ORDER, ambiOrder);
    }
    xmlElement->addChildElement(xmlAmbiChannelWeight);
}

void AmbiSettings::prepareManualWeighting()
{
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        manualOrderWeights[i] = 1.0;
    }
}

void AmbiSettings::prepareAutoWeightings()
{
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < getGainCount())
        {
            standardWeights[i] = 1.0;
        }
        else
        {
            standardWeights[i] = 0.0;
        }
    }

    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < getGainCount())
        {
            double nom = fact(ambiOrder) * fact(ambiOrder + 1);
            double denom = fact(ambiOrder + i + 1)*fact(ambiOrder - i);
            inPhaseWeights[i] = nom / denom;
        }
        else
        {
            inPhaseWeights[i] = 0.0;
        }
    }

    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < getGainCount())
        {
            maxreWeights[i] = cos(MathConstants<double>::pi * i / (2 * (ambiOrder + 1)));
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

void AmbiSettings::setWeightMode(AmbiSettings::AmbiWeightMode mode)
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

int AmbiSettings::getAmbiOrder()
{
    return ambiOrder;
}

int AmbiSettings::getGainCount()
{
    return ambiOrder + 1;
}

int AmbiSettings::getAmbiChannelCount() const
{
    return (ambiOrder + 1) * (ambiOrder + 1);
}

void AmbiSettings::setAmbiOrder(int order)
{
    ambiOrder = order;
    prepareAutoWeightings();
    setWeightMode(weightMode);
}
