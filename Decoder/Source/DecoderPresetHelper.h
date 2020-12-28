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

    struct SSpeaker
    {
        double x;
        double y;
        double z;
        String name;
        bool isSubwoofer = false;
    };

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
        const double cos30 = 0.8660253882408143;
        const double sin45 = 0.7071067811865475;
        createPreset(DEFAULT_PRESET_NAME, 2);
        createPreset("Square (4)", 4);
        createPreset("Hexagon (6)", 6);
        createPreset("Octagon (8)", 8);
        createPreset("Hexadecagon (16)", 16);

        createPreset("3D-Hexagon", { 
            {-0.5, cos30, 0.0, "1"},
            { 0.5, cos30, 0.0, "2"},
            { 1.0, 0.0, 0.0, "3"},
            {0.5, -cos30, 0.0, "4"},
            {-0.5, -cos30, 0.0, "5"},
            {-1.0, 0.0, 0.0, "6"},
            {0.0, sin45, sin45, "7"},
            {0.6123724356957946, 0.3535533905932738, sin45, "8"},
            {0.6123724356957947, -0.3535533905932736, sin45, "9"},
            {0.0, -sin45, sin45, "10"},
            {-0.6123724356957939, -0.3535533905932747, sin45, "11"},
            {-0.6123724356957945, 0.3535533905932737, sin45, "12"}
        });

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
            projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI / numberOfSpeakers));
            for (int i = 0; i < numberOfSpeakers; i++)
            {
                speakerSet.addNew(Uuid().toString(), Point3D<double>(projectedPoint.getX(), projectedPoint.getY(), 0.0), String(i + 1), TrackColors::getSpeakerColor());
                projectedPoint = projectedPoint.rotatedAboutOrigin(-float(PI * 2 / numberOfSpeakers));
            }
        }
        
        writeToXmlFile(file, &speakerSet, &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }

    void createPreset(String name, std::initializer_list<SSpeaker> speakerList)
    {
        AmbiSettings defaultAmbiSettings = getDefaultAmbiSettings();
        AmbiSpeakerSet speakerSet;

        File file = getPathForPresetName(name);

        for (const SSpeaker speaker : speakerList)
        {
            speakerSet.addNew(Uuid().toString(), Point3D<double>(speaker.x, speaker.y, speaker.z), speaker.name, TrackColors::getSpeakerColor());
            if(speaker.isSubwoofer)
            {
                FilterInfo* s = speakerSet.get(speakerSet.size() - 1)->getFilterInfo()->get(0);
                s->filterType = FilterInfo::LowPass;
                s->cutOffFrequencyHz = 50;
                s->qValue = 0.8165930509567261;
            }
        }
        
        writeToXmlFile(file, &speakerSet, &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }
};
