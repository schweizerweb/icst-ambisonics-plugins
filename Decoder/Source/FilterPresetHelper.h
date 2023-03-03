/*
  ==============================================================================

    FilterPresetHelper.h
    Created: 15 May 2020 9:47:54pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/PresetHelper.h"


class FilterPresetHelper : public PresetHelper
{
public:
    FilterPresetHelper(File presetDirectory, ActionListener* pActionListener)
        : PresetHelper(presetDirectory, pActionListener)
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
        
        bool ok = pFilterInfo->loadFromXmlElement(rootElement->getChildByName("Settings"));
    
        return ok;
    }
    
    bool writeToXmlFile(const File file, FilterBankInfo* pFilterInfo)
    {
        XmlElement* rootElement = new XmlElement("FilterPreset");

        XmlElement* xmlSettings = new XmlElement("Settings");
        pFilterInfo->writeToXmlElement(xmlSettings);
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
