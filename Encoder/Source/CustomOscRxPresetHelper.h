/*
  ==============================================================================

    CustomOscRxPresetHelper.h
    Created: 4 Aug 2022 10:46:25am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/PresetHelper.h"

class CustomOscRxPresetHelper : public PresetHelper
{
public:
    CustomOscRxPresetHelper(File presetDirectory, ActionListener* pActionListener)
        : PresetHelper(presetDirectory, pActionListener)
    {
        
    }
    
    bool checkValid(File presetFile) override
    {
        CustomOscInput testInfo;
        if(loadFromXmlFile(presetFile, &testInfo))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        // simple preset
        CustomOscInput simplePreset;
        simplePreset.oscString = "/demo/{i} {x} {y}";
        simplePreset.commandString = "";
        File fileSimplePreset = getPathForPresetName("Simple-Demo");
        writeToXmlFile(fileSimplePreset, &simplePreset);
        presetFiles.addIfNotAlreadyThere(fileSimplePreset);
        
        // JS demo
        CustomOscInput jsPreset;
        jsPreset.oscString = "/jsdemo/*";
        jsPreset.commandString =
R"(// demo that handles XY-plane group manipulation
// incoming OSC example: /jsdemo/1 0.5 0.5 0.5
           
i = s.path(2);      // index is taken from the second path element
x = s.arg(1);       // X is taken from the 1st argument
y = s.arg(2);       // Y is taken from the 2nd argument
z = s.getGroupZ(i); // Z is taken from the current position (to not change it)
stretch = s.arg(3); // The absolute stretch factor is taken from the 3rd argument

// now call the method for setting the group position
s.setGroupXYZ(i, x, y, z);

// and then call the setter for the absolute stretch
ret = s.setGroupStretch(i, stretch);

// using the return value of the setter function, we can implement custom error handling
// in case an error occurred, we overwrite the error message
// only the last error (in this case our custom message) will be displayed
if(ret != true)
    s.reportError("We had an error in setting absolute stretch!");
        )";

        File fileJsPreset = getPathForPresetName("JS-Demo");
        writeToXmlFile(fileJsPreset, &jsPreset);
        presetFiles.addIfNotAlreadyThere(fileJsPreset);
    }
    
    bool loadFromXmlFile(const File file, CustomOscInput* pOscRx)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("CustomOscRx");

        if (rootElement == nullptr || rootElement->getTagName() != "CustomOscRx")
            return false;
        
        bool ok = false;
        CustomOscInput* i = new CustomOscInput(rootElement->getChildByName("Settings"));
        if(i != nullptr)
        {
            ok = true;
            *pOscRx = *i;
        }
        
        delete i;
        return ok;
    }
    
    bool writeToXmlFile(const File file, CustomOscInput* pOscRx)
    {
        XmlElement* rootElement = new XmlElement("CustomOscRx");

        XmlElement* xmlSettings = pOscRx->getAsXmlElement("Settings");
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
};
