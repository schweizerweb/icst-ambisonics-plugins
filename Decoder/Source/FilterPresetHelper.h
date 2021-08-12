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
        FilterBankInfo filterInfo;
        
        File file = getPathForPresetName("LowPass 50 Hz");
        filterInfo.get(0)->filterType = FilterInfo::LowPass;
        filterInfo.get(0)->cutOffFrequencyHz = 50;
        
        writeToXmlFile(file, &filterInfo);
        presetFiles.addIfNotAlreadyThere(file);
        
        File fileHighPass = getPathForPresetName("HighPass 120 Hz");
        filterInfo.get(0)->filterType = FilterInfo::HighPass;
        filterInfo.get(0)->cutOffFrequencyHz = 120;
        
        writeToXmlFile(fileHighPass, &filterInfo);
        presetFiles.addIfNotAlreadyThere(fileHighPass);
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
