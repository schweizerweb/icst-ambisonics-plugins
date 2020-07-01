/*
  ==============================================================================

    DistanceEncodingPresetHelper.h
    Created: 1 Jul 2020 11:16:36pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/PresetHelper.h"


class DistanceEncodingPresetHelper : public PresetHelper
{
public:
    DistanceEncodingPresetHelper(File presetDirectory, ActionListener* pActionListener)
        : PresetHelper(presetDirectory, pActionListener)
    {
        
    }
    
    bool checkValid(File presetFile) override
    {
        DistanceEncodingParams testInfo;
        if(loadFromXmlFile(presetFile, &testInfo))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        DistanceEncodingParams p;
        
        File file = getPathForPresetName("Default");
        
        writeToXmlFile(file, &p);
        presetFiles.addIfNotAlreadyThere(file);
    }
    
    bool loadFromXmlFile(const File file, DistanceEncodingParams* pEncodingParams)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("DistanceEncodingPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "DistanceEncodingPreset")
            return false;
        
        bool ok = pEncodingParams->loadFromXmlElement(rootElement->getChildByName("Settings"));
    
        return ok;
    }
    
    bool writeToXmlFile(const File file, DistanceEncodingParams* pEncodingParams)
    {
        XmlElement* rootElement = new XmlElement("DistanceEncodingPreset");

        XmlElement* xmlSettings = new XmlElement("Settings");
        pEncodingParams->writeToXmlElement(xmlSettings);
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
