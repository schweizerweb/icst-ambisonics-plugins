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

class CustomOscRxPresetHelper : public PresetHelper
{
public:
    CustomOscRxPresetHelper(File _presetDirectory, ActionListener* _pActionListener)
        : PresetHelper(_presetDirectory, _pActionListener)
    {
        
    }
    
    String UniqueActionMessageSelectPreset() override { return "SelectCustomOscRxPreset"; }
    String UniqueActionMessageSavePreset() override { return "SaveCustomOscRxPreset"; }

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
        
        // iannix cursor preset
        CustomOscInput iannixPreset;
        iannixPreset.oscString = "ci1:/cursor {cf,2} {} {} {} {} {x} {y} {z}";
        iannixPreset.commandString = "";
        File fileIannixPreset = getPathForPresetName("Iannix-Cusor-Demo");
        writeToXmlFile(fileIannixPreset, &iannixPreset);
        presetFiles.addIfNotAlreadyThere(fileIannixPreset);
        
        // iannix cursor preset using JS
        CustomOscInput iannixPresetJs;
        iannixPresetJs.oscString = "/cursor";
        iannixPresetJs.commandString =
R"(// demo that parses Iannix cursor messages using JS

i = 2;          // hard coded channel index
c = s.arg(1);   // cursor index from iannix, used for filtering

if(c==2)
{
    x = s.arg(6);   // X is taken from the 6th argument
    y = s.arg(7);   // Y is taken from the 7th argument
    z = s.arg(8);   // Z is taken from the 8th argument
    s.setXYZ(i, x, y, z);   // finally set the new position
}
)";
        File fileIannixPresetJs = getPathForPresetName("Iannix-Cusor-Demo-JS");
        writeToXmlFile(fileIannixPresetJs, &iannixPresetJs);
        presetFiles.addIfNotAlreadyThere(fileIannixPresetJs);

        // preset for absolute quaternion rotation using JS
        CustomOscInput quatPresetJs;
        quatPresetJs.oscString = "/absgrouprotation/quaternion";
        quatPresetJs.commandString =
R"(// set absolute group rotation using quaterion values
// syntax: /absgrouprotation/quaternion [groupName] [q1] [q2] [q3] [q4]

groupName = s.arg(1);
q1 = s.arg(2);
q2 = s.arg(3);
q3 = s.arg(4);
q4 = s.arg(5);

s.setGroupRotationByName (groupName, q1, q2, q3, q4);
)";
        File filequatPresetJs = getPathForPresetName("Abs-Quaternion-Rotation-JS");
        writeToXmlFile(filequatPresetJs, &quatPresetJs);
        presetFiles.addIfNotAlreadyThere(filequatPresetJs);

        // preset for absolute group stretch using JS
        CustomOscInput stretchPresetJs;
        stretchPresetJs.oscString = "/absgroupstretch/factor";
        stretchPresetJs.commandString =
R"(// set absolute group stretch factor
// syntax: /absgroupstretch/factor [groupName] [factor]

groupName = s.arg(1);
factor = s.arg(2);

s.setGroupStretchByName (groupName, factor);
)";
        File fileStretchPresetJs = getPathForPresetName("Abs-Stretch-JS");
        writeToXmlFile(fileStretchPresetJs, &stretchPresetJs);
        presetFiles.addIfNotAlreadyThere(fileStretchPresetJs);
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
