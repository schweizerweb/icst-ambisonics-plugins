/*
  ==============================================================================

    AmbiSettings.cpp
    Created: 9 Oct 2017 4:02:43pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiSettings.h"

AmbiSettings::AmbiSettings(): AmbiSettings(DEFAULT_DISTANCE_SCALER, false)
{
	
}

AmbiSettings::AmbiSettings(double distanceScaler, bool directionFlip) : AmbiBasicSettings(distanceScaler, directionFlip)
{
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		ambiOrderWeights[i] = 1.0;

	// create mapping table
	int order = 0;
	for (int i = 0; i < NB_OF_AMBISONICS_CHANNELS; i++)
	{
		if (i == (order + 1)*(order + 1))
			order++;
		ambiChannelWeights[i] = &ambiOrderWeights[order];
	}
}

double AmbiSettings::getAmbiChannelWeight(int ambiChannel)
{
	return *ambiChannelWeights[ambiChannel];
}

double* AmbiSettings::getAmbiOrderWeightPointer()
{
	return ambiOrderWeights;
}

void AmbiSettings::loadFromPresetXml(XmlElement *xmlElement)
{
    // ambisonics settings
    XmlElement* xmlDistanceScaler = xmlElement->getChildByName(XML_TAG_PRESET_DISTANCESCALER);
    if (xmlDistanceScaler != nullptr)
    {
       setDistanceScaler(xmlDistanceScaler->getDoubleAttribute(XML_VALUE));
    }
    
    XmlElement* xmlFlipDirection = xmlElement->getChildByName(XML_TAG_PRESET_FLIPDIRECTION);
    if (xmlFlipDirection != nullptr)
    {
        setDirectionFlip(xmlFlipDirection->getBoolAttribute(XML_VALUE));
    }
    
    XmlElement* xmlAmbiChannelWeight = xmlElement->getChildByName(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    int index = 0;
    if (xmlAmbiChannelWeight != nullptr)
    {
        String attributeName;
        while (xmlAmbiChannelWeight->hasAttribute(attributeName = "Order" + String(index)))
        {
            ambiOrderWeights[index] = xmlAmbiChannelWeight->getDoubleAttribute(attributeName);
            index++;
        }
    }
}

void AmbiSettings::writeToPresetXmlElement(XmlElement *xmlElement) const
{
    XmlElement* xmlDistanceScaler = new XmlElement(XML_TAG_PRESET_DISTANCESCALER);
    xmlDistanceScaler->setAttribute(XML_VALUE, getDistanceScaler());
    xmlElement->addChildElement(xmlDistanceScaler);
    
    XmlElement* xmlFlipDirection = new XmlElement(XML_TAG_PRESET_FLIPDIRECTION);
    xmlFlipDirection->setAttribute(XML_VALUE, getDirectionFlip());
    xmlElement->addChildElement(xmlFlipDirection);

    XmlElement* xmlAmbiChannelWeight = new XmlElement(XML_TAG_PRESET_AMBICHANNELWEIGHT);
    for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
    {
        xmlAmbiChannelWeight->setAttribute("Order" + String(i), ambiOrderWeights[i]);
    }
    xmlElement->addChildElement(xmlAmbiChannelWeight);
}

void AmbiSettings::setInPhaseWeighting()
{
    for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
    {
        if (i < CURRENT_AMBISONICS_ORDER_NB_OF_GAINS)
        {
            double nom = fact(CURRENT_AMBISONICS_ORDER) * fact(CURRENT_AMBISONICS_ORDER + 1);
            double denom = fact(CURRENT_AMBISONICS_ORDER + i + 1)*fact(CURRENT_AMBISONICS_ORDER - i);
            ambiOrderWeights[i] = nom / denom;
        }
        else
        {
            ambiOrderWeights[i] = 0.0;
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
