/*
  ==============================================================================

    EncoderPreset.h
    Created: 19 Jan 2020 9:37:14pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AudioParams.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PresetHelper.h"
#include "../../Common/TrackColors.h"
#include "../../Common/ScalingInfo.h"
#include "EncoderSettings.h"

#define DEFAULT_PRESET_NAME "Single Source"


class EncoderPresetHelper : public PresetHelper
{
public:
    EncoderPresetHelper(File presetDirectory, ActionListener* pActionListener, ScalingInfo* pScaling) : PresetHelper(presetDirectory, pActionListener), pScalingInfo(pScaling)
    {
    }
    
    bool loadFromXmlFile(const File file, AudioParams* pAudioParams, AmbiSourceSet* pSourceSet, EncoderSettings* pEncoderSettings)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("EncoderPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "EncoderPreset")
            return false;
        
        pEncoderSettings->loadFromPresetXml(rootElement->getChildByName("EncoderSettings"));
        // apply scaler if audio params attached
        if(pAudioParams != nullptr)
            pScalingInfo->SetScaler(pEncoderSettings->getDistanceScaler());
        
        pSourceSet->loadFromXml(rootElement->getChildByName("AmbiSourceSet"), pAudioParams);
        pSourceSet->resetIds();
        
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
        std::unique_ptr<AmbiSourceSet> testSet(new AmbiSourceSet(pScalingInfo));
        EncoderSettings testSettings;
        if(loadFromXmlFile(presetFile, nullptr, testSet.get(), &testSettings))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override {
        std::unique_ptr<AmbiSourceSet> sources(new AmbiSourceSet(pScalingInfo));
        EncoderSettings settings;
        
        sources->addNew(Uuid().toString(), Point3D<double>(0.0, 0.0, 0.0), "1", TrackColors::getColor(1));
        settings.distanceEncodingFlag = true;
        settings.distanceEncodingParams.setEncodingMode(EncoderConstants::Standard);
        settings.distanceEncodingParams.setUnitCircleRadius(0.1f);
        
        File file = getPathForPresetName(DEFAULT_PRESET_NAME);
        
        writeToXmlFile(file, sources.get(), &settings);
        presetFiles.addIfNotAlreadyThere(file);
    }
    
    bool loadDefaultPreset(AudioParams* pAudioParams, AmbiSourceSet* pSourceSet, EncoderSettings* pEncoderSettings)
    {
        bool found = false;
        for(File f : presetFiles)
        {
            if(f.getFileNameWithoutExtension() == DEFAULT_PRESET_NAME)
            {
                loadFromXmlFile(f, pAudioParams, pSourceSet, pEncoderSettings);
                found = true;
                break;
            }
        }

        return found;
    }
    
private:
    ScalingInfo* pScalingInfo;
};
