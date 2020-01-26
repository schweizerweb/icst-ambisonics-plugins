/*
  ==============================================================================

    EncoderPreset.h
    Created: 19 Jan 2020 9:37:14pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AudioParameterSet.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PresetHelper.h"
#include "EncoderSettings.h"

class EncoderPresetHelper : public PresetHelper
{
public:
    EncoderPresetHelper(File presetDirectory, ActionListener* pActionListener) : PresetHelper(presetDirectory, pActionListener)
    {
    }
    
    bool loadFromXmlFile(const File file, Array<AudioParameterSet>* pAudioParams, AmbiSourceSet* pSourceSet, EncoderSettings* pEncoderSettings)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("EncoderPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "EncoderPreset")
            return false;
        
        pSourceSet->loadFromXml(rootElement->getChildByName("AmbiSourceSet"), pAudioParams);
        pSourceSet->resetIds();
        
        pEncoderSettings->loadFromPresetXml(rootElement->getChildByName("EncoderSettings"));

        return pSourceSet->size() > 0;
    }

    bool writeToXmlFile(const File file, AmbiSourceSet* pSourceSet, EncoderSettings* pEncoderSettings)
    {
        XmlElement* rootElement = new XmlElement("EncoderPreset");

        XmlElement* xmlSources = new XmlElement("AmbiSourceSet");
        pSourceSet->writeToXmlElement(xmlSources);
        rootElement->addChildElement(xmlSources);
        
        XmlElement* xmlSettings = new XmlElement("EncoderSettings");
        pEncoderSettings->writeToPresetXmlElement(xmlSettings);
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
    
    bool checkValid(File presetFile) override {
        AmbiSourceSet testSet;
        EncoderSettings testSettings;
        if(loadFromXmlFile(presetFile, nullptr, &testSet, &testSettings))
        {
            return true;
        }
        
        return false;
    }
    
};
