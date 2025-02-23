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



#include "AmbiSettingsSection.h"
#include "MultiDecoderColors.h"

AmbiSettingsSection::AmbiSettingsSection(): gain(0), mute(false), speakerMask(0), color(Colours::red)
{
}

bool AmbiSettingsSection::loadFromPresetXml(XmlElement *xmlElement, String defaultName)
{
    bool warningFlag = false;
    
    name = xmlElement->getStringAttribute(XML_ATTRIBUTE_SECTION_NAME, defaultName);
    mute = xmlElement->getBoolAttribute(XML_ATTRIBUTE_SECTION_MUTE, false);
    gain = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_SECTION_GAIN, 1.0);
    color = Colour::fromString(xmlElement->getStringAttribute(XML_ATTRIBUTE_SECTION_COLOR, ""));
    speakerMask = String(xmlElement->getStringAttribute(XML_ATTRIBUTE_SECTION_SPEAKERMASK)).getLargeIntValue();
    XmlElement* filterElement = xmlElement->getChildByName(XML_TAG_SECTION_FILTER);
    if (filterElement != nullptr)
    {
        filterInfo.loadFromXmlElement(filterElement);
    }
    
    warningFlag = ambiSettings.loadFromPresetXml(xmlElement);

    return warningFlag;
}

void AmbiSettingsSection::setDefault(int index)
{
    ambiSettings.setAmbiOrder(DEAFULT_AMBI_ORDER);
    ambiSettings.setWeightMode(DEFAULT_WEIGHT_MODE);
    name = String(index + 1);
    color = MultiDecoderColors::getColor(index);
    gain = DEFAULT_GAIN_FACTOR;
    mute = index != 0;
    speakerMask = 0xffffffffffffffff;
    filterInfo.init();
}

void AmbiSettingsSection::writeToPresetXmlElement(XmlElement *xmlElement)
{
    xmlElement->setAttribute(XML_ATTRIBUTE_SECTION_NAME, name);
    xmlElement->setAttribute(XML_ATTRIBUTE_SECTION_MUTE, mute);
    xmlElement->setAttribute(XML_ATTRIBUTE_SECTION_GAIN, gain);
    xmlElement->setAttribute(XML_ATTRIBUTE_SECTION_COLOR, color.toString());
    xmlElement->setAttribute(XML_ATTRIBUTE_SECTION_SPEAKERMASK, String(speakerMask));
    XmlElement* xmlFilter = new XmlElement(XML_TAG_SECTION_FILTER);
    filterInfo.writeToXmlElement(xmlFilter);
    xmlElement->addChildElement(xmlFilter);

    ambiSettings.writeToPresetXmlElement(xmlElement);
}

