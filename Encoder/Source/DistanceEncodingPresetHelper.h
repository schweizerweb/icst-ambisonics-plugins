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
        buildDefaultPreset("Default",
R"(<?xml version="1.0" encoding="UTF-8"?>

<DistanceEncodingPreset>
  <Settings DistEncMode="1" UnitCircleRadius="0.1000000014901161" DistEncDbUnit="10.0"
            DistEncDistanceAttenuation="5.0" DistEncCenterCurve="0.01999999955296516"
            DistEncAdvancedFactor="1.0" DistEncAdvancedExponent="1.0" AirAbsorbtionMode="0"
            AirAbsorbtionIntensity="1.0"/>
</DistanceEncodingPreset>)"
        );
        
        buildDefaultPreset("Demo Standard",
R"(<?xml version="1.0" encoding="UTF-8"?>

<DistanceEncodingPreset>
  <Settings DistEncMode="1" UnitCircleRadius="0.7208999991416931" DistEncDbUnit="10.0"
            DistEncDistanceAttenuation="5.0" DistEncCenterCurve="0.01999999955296516"
            DistEncAdvancedFactor="1.0" DistEncAdvancedExponent="1.0" AirAbsorbtionMode="1"
            AirAbsorbtionIntensity="4.5"/>
</DistanceEncodingPreset>)"
        );
        
        buildDefaultPreset("Demo Exponential",
R"(<?xml version="1.0" encoding="UTF-8"?>

<DistanceEncodingPreset>
  <Settings DistEncMode="3" UnitCircleRadius="0.09299999475479126" DistEncDbUnit="51.58309936523438"
            DistEncDistanceAttenuation="5.0" DistEncCenterCurve="0.007400000002235174"
            DistEncAdvancedFactor="1.0" DistEncAdvancedExponent="1.0" AirAbsorbtionMode="1"
            AirAbsorbtionIntensity="4.200000286102295"/>
</DistanceEncodingPreset>)"
        );
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
