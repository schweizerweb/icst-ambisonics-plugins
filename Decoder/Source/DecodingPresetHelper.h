/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once
#include "../../Common/PresetHelper.h"

#define DEFAULT_DECODING_PRESET_NAME "default"

class DecodingPresetHelper : public PresetHelper
{
public:
    DecodingPresetHelper(File _presetDirectory, ActionListener* _pActionListener) : PresetHelper(_presetDirectory, _pActionListener)
    {
    }

    String UniqueActionMessageSelectPreset() override { return "SelectDecodingPreset"; }
    String UniqueActionMessageSavePreset() override { return "SaveDecodingPreset"; }

    bool checkValid(juce::File presetFile) override
    {
        std::unique_ptr<AmbiSettingsCollection> testSet(new AmbiSettingsCollection());
        if(loadFromXmlFile(presetFile, testSet.get()))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        // default
        File file = getPathForPresetName(DEFAULT_DECODING_PRESET_NAME);
        AmbiSettingsCollection s;
        s.setMultiDecoderFlag(false);
        s.singleDecoder->setAmbiOrder(1);
        s.singleDecoder->setWeightMode(AmbiSettings::AmbiWeightMode::INPHASE);
        writeToXmlFile(file, &s);
        presetFiles.addIfNotAlreadyThere(file);

		notifyPresetListChanged();
    }
    
    void loadDefaultPreset(AmbiSettingsCollection* pSettings)
    {
        for(File f : presetFiles)
        {
            if(f.getFileNameWithoutExtension() == DEFAULT_DECODING_PRESET_NAME)
            {
                loadFromXmlFile(f, pSettings);
                break;
            }
        }
    }
    
    bool loadFromXmlFile(const File file, AmbiSettingsCollection* pSettings)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("AmbisonicsDecodingPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "AmbisonicsDecodingPreset")
            return false;
        
        pSettings->loadFromPresetXml(rootElement.get());
       
        return true;
    }
    
    bool writeToXmlFile(const File file, AmbiSettingsCollection* pSettings)
    {
        XmlElement* rootElement = new XmlElement("AmbisonicsDecodingPreset");

        pSettings->writeToPresetXmlElement(rootElement);
        
        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
