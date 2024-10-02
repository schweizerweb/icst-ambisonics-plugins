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
#include "../../Common/PresetHelper.h"


class FilterPresetHelper : public PresetHelper
{
public:
    FilterPresetHelper(File _presetDirectory, ActionListener* _pActionListener)
        : PresetHelper(_presetDirectory, _pActionListener)
    {
        
    }
    
    bool checkValid(File presetFile) override
    {
        FilterBankInfo testInfo;
        if(loadFromXmlFile(presetFile, &testInfo))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        buildDefaultPreset("LowPass 50 Hz",
R"(<?xml version="1.0" encoding="UTF-8"?>

<FilterPreset>
  <Settings FilterType="1" FilterFrequency="50.0" FilterQ="1.0" GainFactor="1.0"
            FilterType1="0" FilterFrequency1="200.0" FilterQ1="1.0" GainFactor1="1.0"
            FilterType2="0" FilterFrequency2="200.0" FilterQ2="1.0" GainFactor2="1.0"
            FilterType3="0" FilterFrequency3="200.0" FilterQ3="1.0" GainFactor3="1.0"/>
</FilterPreset>
)"
            );
        
        buildDefaultPreset("HighPass 120Hz",
R"(<?xml version="1.0" encoding="UTF-8"?>

<FilterPreset>
  <Settings FilterType="3" FilterFrequency="120.0" FilterQ="1.0" GainFactor="1.0"
            FilterType1="0" FilterFrequency1="200.0" FilterQ1="1.0" GainFactor1="1.0"
            FilterType2="0" FilterFrequency2="200.0" FilterQ2="1.0" GainFactor2="1.0"
            FilterType3="0" FilterFrequency3="200.0" FilterQ3="1.0" GainFactor3="1.0"/>
</FilterPreset>
)"
            );
    }
    
    bool loadFromXmlFile(const File file, FilterBankInfo* pFilterInfo)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("FilterPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "FilterPreset")
            return false;
        
        bool ok = pFilterInfo->loadFromXmlElement(rootElement->getChildByName("Settings"), true);
    
        return ok;
    }
    
    bool writeToXmlFile(const File file, FilterBankInfo* pFilterInfo)
    {
        XmlElement* rootElement = new XmlElement("FilterPreset");

        XmlElement* xmlSettings = new XmlElement("Settings");
        pFilterInfo->writeToXmlElement(xmlSettings, true);
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
