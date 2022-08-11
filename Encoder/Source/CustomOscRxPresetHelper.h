/*
  ==============================================================================

    CustomOscRxPresetHelper.h
    Created: 4 Aug 2022 10:46:25am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/PresetHelper.h"


class CustomOscRxPresetHelper : public PresetHelper
{
public:
    CustomOscRxPresetHelper(File presetDirectory, ActionListener* pActionListener)
        : PresetHelper(presetDirectory, pActionListener)
    {
        
    }
    
    bool checkValid(File presetFile) override
    {
        CustomOscInput testInfo;
        if(loadFromXmlFile(presetFile, &testInfo))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        CustomOscInput p;
        
        File file = getPathForPresetName("Default");
        
        writeToXmlFile(file, &p);
        presetFiles.addIfNotAlreadyThere(file);
    }
    
    bool loadFromXmlFile(const File file, CustomOscInput* pOscRx)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("CustomOscRx");

        if (rootElement == nullptr || rootElement->getTagName() != "CustomOscRx")
            return false;
        
        bool ok = false;
        CustomOscInput* i = new CustomOscInput(rootElement->getChildByName("Settings"));
        if(i != nullptr)
        {
            ok = true;
            *pOscRx = *i;
        }
        
        delete i;
        return ok;
    }
    
    bool writeToXmlFile(const File file, CustomOscInput* pOscRx)
    {
        XmlElement* rootElement = new XmlElement("CustomOscRx");

        XmlElement* xmlSettings = pOscRx->getAsXmlElement("Settings");
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
