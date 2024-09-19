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

AmbiSettingsCollection::AmbiSettingsCollection()
{
    loadWarningFlag = false;
    singleDecoder.reset(new AmbiSettings());
}

void AmbiSettingsCollection::loadFromPresetXml(XmlElement *xmlElement)
{
    loadWarningFlag = false;
    // ambisonics settings
    
    multiDecoderFlag = xmlElement->getBoolAttribute(XML_ATTRIBUTE_MULTI_DECODER, false);
    loadWarningFlag |= singleDecoder->loadFromPresetXml(xmlElement);
}

void AmbiSettingsCollection::writeToPresetXmlElement(XmlElement *xmlElement) const
{
    xmlElement->setAttribute(XML_ATTRIBUTE_MULTI_DECODER, multiDecoderFlag);
    singleDecoder->writeToPresetXmlElement(xmlElement);
    //xmlElement->addChildElement(xmlAmbiChannelWeight);
}


bool AmbiSettingsCollection::getWarningFlag()
{
    return loadWarningFlag;
}

bool AmbiSettingsCollection::getMultiDecoderFlag()
{
    return multiDecoderFlag;
}

void AmbiSettingsCollection::setMultiDecoderFlag(bool isMulti)
{
    multiDecoderFlag = isMulti;
}

void AmbiSettingsCollection::ensureMaxAmbiOrder(int maxOrder)
{
    if (singleDecoder->getAmbiOrder() > maxOrder)
    {
        singleDecoder->setAmbiOrder(maxOrder);
    }

    // TODO: other channels

}