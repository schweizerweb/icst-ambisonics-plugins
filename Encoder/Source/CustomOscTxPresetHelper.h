/*
  ==============================================================================

    CustomOscTxPresetHelper.h
    Created: 6 Sep 2022 11:16:29pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/PresetHelper.h"


class CustomOscTxPresetHelper : public PresetHelper
{
public:
    CustomOscTxPresetHelper(File _presetDirectory, ActionListener* _pActionListener)
        : PresetHelper(_presetDirectory, _pActionListener)
    {
        
    }
    
    bool checkValid(File presetFile) override
    {
        CustomOscTarget testInfo;
        if(loadFromXmlFile(presetFile, &testInfo))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        CustomOscTarget p;
        
        File file = getPathForPresetName("Default");
        
        writeToXmlFile(file, &p);
        presetFiles.addIfNotAlreadyThere(file);
    }
    
    bool loadFromXmlFile(const File file, CustomOscTarget* pOscTx)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("CustomOscTx");

        if (rootElement == nullptr || rootElement->getTagName() != "CustomOscTx")
            return false;
        
        bool ok = false;
        CustomOscTarget* i = new CustomOscTarget(rootElement->getChildByName("Settings"));
        if(i != nullptr)
        {
            ok = true;
            *pOscTx = *i;
        }
        
        delete i;
        return ok;
    }
    
    bool writeToXmlFile(const File file, CustomOscTarget* pOscTx)
    {
        XmlElement* rootElement = new XmlElement("CustomOscTx");

        XmlElement* xmlSettings = pOscTx->getAsXmlElement("Settings");
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
