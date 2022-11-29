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
        CustomOscInput simplePreset;
        simplePreset.oscString = "/demo/{i} {x} {y}";
        
        CustomOscInput jsPreset;
        jsPreset.oscString = "/jsdemo/*";
        std::vector<String> lines;
        lines.push_back("// demo that handles XY-plane group manipulation");
        lines.push_back("// incoming OSC example: /jsdemo/1 0.5 0.5 0.5");
        lines.push_back("");
        lines.push_back("i = s.path(2);      // index is taken from the second path element");
        lines.push_back("x = s.arg(1);       // X is taken from the 1st argument");
        lines.push_back("y = s.arg(2);       // Y is taken from the 2nd argument");
        lines.push_back("z = s.getGroupZ(i); // Z is taken from the current position (to not change it)");
        lines.push_back("stretch = s.arg(3); // The absolute stretch factor is taken from the 3rd argument");
        lines.push_back("");
        lines.push_back("// now call the method for setting the group position");
        lines.push_back("s.setGroupXYZ(i, x, y, z);");
        lines.push_back("");
        lines.push_back("// and then call the setter for the absolute stretch");
        lines.push_back("ret = s.setGroupStretch(i, stretch);");
        lines.push_back("");
        lines.push_back("// using the return value of the setter function, we can implement custom error handling");
        lines.push_back("// in case an error occurred, we overwrite the error message");
        lines.push_back("// only the last error (in this case our custom message) will be displayed");
        lines.push_back("if(ret != true)");
        lines.push_back("   s.reportError(\"We had an error in setting absolute stretch!\");");
        
        String str;
        for(auto l : lines)
        {
            str.append(l, l.length());
            str.append(NewLine::getDefault(), strlen(NewLine::getDefault()));
        }
        jsPreset.commandString = str;
                        
        File fileSimplePreset = getPathForPresetName("Simple-Demo");
        File fileJsPreset = getPathForPresetName("JS-Demo");
                        
        
        writeToXmlFile(fileSimplePreset, &simplePreset);
        writeToXmlFile(fileJsPreset, &jsPreset);
                        
        presetFiles.addIfNotAlreadyThere(fileSimplePreset);
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
