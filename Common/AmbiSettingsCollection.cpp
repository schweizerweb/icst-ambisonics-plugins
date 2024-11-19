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



#include "AmbiSettingsCollection.h"
#include "MultiDecoderColors.h"

AmbiSettingsCollection::AmbiSettingsCollection()
{
    loadWarningFlag = false;
    nbUsedDecoders = 3;
    multiDecoderFlag = false;
    singleDecoder.reset(new AmbiSettings());
}

void AmbiSettingsCollection::loadFromPresetXml(XmlElement *xmlElement)
{
    loadWarningFlag = false;
    // ambisonics settings
    
    multiDecoderFlag = xmlElement->getBoolAttribute(XML_ATTRIBUTE_MULTI_DECODER_FLAG, false);
    nbUsedDecoders = xmlElement->getIntAttribute(XML_ATTRIBUTE_MULTI_DECODER_SECTION_COUNT, 1);
    loadWarningFlag |= singleDecoder->loadFromPresetXml(xmlElement);
    for (int i = 0; i < MAX_NB_OF_DECODER_SECTIONS; i++)
    {
        XmlElement* xmlSection = xmlElement->getChildByName(XML_TAG_DECODER_SECTION+String(i));
        if (xmlSection != nullptr && i < nbUsedDecoders)
        {
            loadWarningFlag |= multiDecoderSections[i].loadFromPresetXml(xmlSection, String(i+1));
        }
        else
        {
            multiDecoderSections[i].ambiSettings.setAmbiOrder(DEAFULT_AMBI_ORDER);
            multiDecoderSections[i].ambiSettings.setWeightMode(DEFAULT_WEIGHT_MODE);
            multiDecoderSections[i].name = String(i+1);
            multiDecoderSections[i].color = MultiDecoderColors::getColor(i);
            multiDecoderSections[i].gain = DEFAULT_GAIN_FACTOR;
            multiDecoderSections[i].mute = i != 0;
            multiDecoderSections[i].speakerMask = 0xffffffffffffffff;
            multiDecoderSections[i].filterInfo.init();
        }
    }
}

void AmbiSettingsCollection::writeToPresetXmlElement(XmlElement *xmlElement)
{
    xmlElement->setAttribute(XML_ATTRIBUTE_MULTI_DECODER_FLAG, multiDecoderFlag);
    xmlElement->setAttribute(XML_ATTRIBUTE_MULTI_DECODER_SECTION_COUNT, nbUsedDecoders);
    singleDecoder->writeToPresetXmlElement(xmlElement);

    for (int i = 0; i < MAX_NB_OF_DECODER_SECTIONS && i < nbUsedDecoders; i++)
    {
        XmlElement* xmlSection = new XmlElement(XML_TAG_DECODER_SECTION+String(i));
        multiDecoderSections[i].writeToPresetXmlElement(xmlSection);
        xmlElement->addChildElement(xmlSection);
    }
}


bool AmbiSettingsCollection::getWarningFlag()
{
    return loadWarningFlag;
}

bool AmbiSettingsCollection::getMultiDecoderFlag() const
{
    return multiDecoderFlag;
}

void AmbiSettingsCollection::setMultiDecoderFlag(bool isMulti)
{
    multiDecoderFlag = isMulti;
}

int AmbiSettingsCollection::getUsedDecoderCount() const
{
    return nbUsedDecoders;
}
void AmbiSettingsCollection::setUsedDecoderCount(int count)
{
    nbUsedDecoders = count;
}

void AmbiSettingsCollection::ensureMaxAmbiOrder(int maxOrder)
{
    if (singleDecoder->getAmbiOrder() > maxOrder)
    {
        singleDecoder->setAmbiOrder(maxOrder);
    }

    // TODO: other channels

}

int AmbiSettingsCollection::getMaxUsedChannelCount() const
{
    if (getMultiDecoderFlag())
    {
        int max = 0;
        for (int i = 0; i < getUsedDecoderCount(); i++)
        {
            max = jmax(max, multiDecoderSections[i].ambiSettings.getAmbiChannelCount());
        }

        return max;
    }
    else
    {
        return singleDecoder->getAmbiChannelCount();
    }
}
