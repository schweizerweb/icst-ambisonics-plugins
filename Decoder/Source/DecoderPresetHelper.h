/*
  ==============================================================================

    DecoderPresetHelper.h
    Created: 26 Jan 2020 9:49:43pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "../../Common/PresetHelper.h"
#include "../../Common/TrackColors.h"

#define DEFAULT_PRESET_NAME "Stereo (2)"

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
    
    void restoreDefaultsInternal() override
    {
        createPreset(DEFAULT_PRESET_NAME, 2);
        createPreset("Square (4)", 4);
        createPreset("Hexagon (6)", 6);
        createPreset("Octagon (8)", 8);
        createPreset("Hexadecagon (16)", 16);
        createPreset("Triacontakaidigon (32)", 32);
        
        notifyPresetListChanged();
    }
    
    void loadDefaultPreset(AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        for(File f : presetFiles)
        {
            if(f.getFileNameWithoutExtension() == DEFAULT_PRESET_NAME)
            {
                loadFromXmlFile(f, pSpeakerSet, pAmbiSettings);
                break;
            }
        }
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
        
        settings.setDistanceScaler(DEFAULT_DISTANCE_SCALER);
        settings.setWeightMode(AmbiSettings::INPHASE);
            
        return settings;
    }
    
    void createPreset(String name, int numberOfSpeakers)
    {
        AmbiSettings defaultAmbiSettings = getDefaultAmbiSettings();
        AmbiSpeakerSet speakerSet;
        
        File file = getPathForPresetName(name);
        
        if(numberOfSpeakers == 2)
        {
            speakerSet.addNew(Uuid().toString(), Point3D<double>(-1.0, 0.0, 0.0), "L", TrackColors::getSpeakerColor());
            speakerSet.addNew(Uuid().toString(), Point3D<double>(1.0, 0.0, 0.0), "R", TrackColors::getSpeakerColor());
        }
        else
        {
            Point<float> projectedPoint(0.0, 1.0);
            projectedPoint = projectedPoint.rotatedAboutOrigin(-float(PI / numberOfSpeakers));
            for (int i = 0; i < numberOfSpeakers; i++)
            {
                speakerSet.addNew(Uuid().toString(), Point3D<double>(projectedPoint.getX(), projectedPoint.getY(), 0.0), String(i + 1), TrackColors::getSpeakerColor());
                projectedPoint = projectedPoint.rotatedAboutOrigin(-float(PI * 2 / numberOfSpeakers));
            }
        }
        
        writeToXmlFile(file, &speakerSet, &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }
};
