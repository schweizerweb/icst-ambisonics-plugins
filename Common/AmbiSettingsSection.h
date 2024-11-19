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
#include "AmbiSettings.h"
#include "FilterBankInfo.h"

#define XML_ATTRIBUTE_SECTION_NAME "name"
#define XML_ATTRIBUTE_SECTION_MUTE "mute"
#define XML_ATTRIBUTE_SECTION_GAIN "gain"
#define XML_ATTRIBUTE_SECTION_COLOR "color"
#define XML_ATTRIBUTE_SECTION_SPEAKERMASK "speakerMask"
#define XML_TAG_SECTION_FILTER "Filter"

class AmbiSettingsSection
{
public:
    double gain;
    bool mute;
    Colour color;
    AmbiSettings ambiSettings;
    FilterBankInfo filterInfo;
    uint64_t speakerMask;
    String name;

    AmbiSettingsSection();
    void writeToPresetXmlElement(XmlElement* xmlElement);
    bool loadFromPresetXml(XmlElement* xmlElement, String defaultName = "-");
};
