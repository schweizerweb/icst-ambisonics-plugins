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
#include "AmbiSettingsSection.h"

#define MAX_NB_OF_DECODER_SECTIONS 3
#define XML_ATTRIBUTE_MULTI_DECODER_FLAG "multiDecoder"
#define XML_ATTRIBUTE_MULTI_DECODER_SECTION_COUNT "sectionCount"
#define XML_TAG_DECODER_SECTION "DecoderSection"

class AmbiSettingsCollection
{
public:
    AmbiSettingsCollection();

    bool getWarningFlag();

    bool getMultiDecoderFlag() const;
    void setMultiDecoderFlag(bool isMulti);

    int getUsedDecoderCount() const;
    void setUsedDecoderCount(int count);

    void ensureMaxAmbiOrder(int maxOrder);
    int getMaxUsedChannelCount() const;

    void writeToPresetXmlElement(XmlElement* xmlElement);
    void loadFromPresetXml(XmlElement* xmlElement);

    std::unique_ptr<AmbiSettings> singleDecoder;
    AmbiSettingsSection multiDecoderSections[MAX_NB_OF_DECODER_SECTIONS];

private:
    bool loadWarningFlag;

    bool multiDecoderFlag;
    int nbUsedDecoders;
};
