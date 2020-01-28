/*
  ==============================================================================

    DecoderPresetHelper.h
    Created: 26 Jan 2020 9:49:43pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "../../Common/PresetHelper.h"

class DecoderPresetHelper : public PresetHelper
{
public:
    DecoderPresetHelper(File presetDirectory, ActionListener* pActionListener) : PresetHelper(presetDirectory, pActionListener)
    {
    }
    
    bool checkValid(juce::File presetFile) override
    {
        AmbiSpeakerSet testSet;
        AmbiSettings testSettings;
        if(loadFromXmlFile(presetFile, &testSet, &testSettings))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaults() override
    {
        createPreset("Stereo", 2);
        createPreset("4", 4);
        
        notifyPresetListChanged();
    }
    
    bool loadFromXmlFile(const File file, AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("AmbisonicsPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "AmbisonicsPreset")
            return false;
        
        pSpeakerSet->loadFromXml(rootElement->getChildByName("Points"));
    
        pAmbiSettings->loadFromPresetXml(rootElement->getChildByName("General"));

        return pSpeakerSet->size() > 0;
    }
    
    bool writeToXmlFile(const File file, AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        XmlElement* rootElement = new XmlElement("AmbisonicsPreset");

        XmlElement* xmlSpeakers = new XmlElement("Points");
        pSpeakerSet->writeToXmlElement(xmlSpeakers);
        rootElement->addChildElement(xmlSpeakers);
        
        XmlElement* xmlSettings = new XmlElement("General");
        pAmbiSettings->writeToPresetXmlElement(xmlSettings);
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
    
    AmbiSettings getDefaultAmbiSettings()
    {
        AmbiSettings settings;
        return settings;
    }
    
    void createPreset(String name, int numberOfSpeakers)
    {
        AmbiSettings defaultAmbiSettings = getDefaultAmbiSettings();
        AmbiSpeakerSet speakerSet;
        
        File file = getPathForPresetName(name);
        
        if(numberOfSpeakers == 2)
        {
            speakerSet.addNew(Uuid().toString(), Point3D<double>(0.0, -1.0, 0.0), "L", Colours::black);
            speakerSet.addNew(Uuid().toString(), Point3D<double>(0.0, 1.0, 0.0), "R", Colours::black);
        }
        else
        {
            
        }
        
        writeToXmlFile(file, &speakerSet, &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }
};
