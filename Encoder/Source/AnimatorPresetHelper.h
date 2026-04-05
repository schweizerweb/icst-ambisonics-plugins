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
#include "JuceHeader.h"
#include "../../Common/PresetHelper.h"
#include "AnimatorDataset.h"


class AnimatorPresetHelper : public PresetHelper
{
public:
    AnimatorPresetHelper(File _presetDirectory, ActionListener* _pActionListener)
        : PresetHelper(_presetDirectory, _pActionListener)
    {
        
    }
    
    String UniqueActionMessageSelectPreset() override { return "SelectAnimatorPreset"; }
    String UniqueActionMessageSavePreset() override { return "SaveAnimatorPreset"; }

    bool checkValid(File presetFile) override
    {
        AnimatorDataset testInfo;
        if(loadFromXmlFile(presetFile, &testInfo))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        buildDefaultPreset("default", "");
    }
    
    bool loadFromXmlFile(const File file, AnimatorDataset* pDataset)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("AnimatorPresetDemo");

        if (rootElement == nullptr || rootElement->getTagName() != "AnimatorPresetDemo")
            return false;
        
        bool ok = pDataset->loadFromXml(rootElement.get());
    
        return ok;
    }
    
    bool writeToXmlFile(const File file, AnimatorDataset* pDataset)
    {
        XmlElement* rootElement = pDataset->getAsXmlElement("AnimatorPresetDemo");
        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
