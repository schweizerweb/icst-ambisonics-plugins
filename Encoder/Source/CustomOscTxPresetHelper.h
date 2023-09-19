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
