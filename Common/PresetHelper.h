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

#define ACTION_MESSAGE_PRESET_CHANGED "PresetChanged"
#define ACTION_MESSAGE_PRESET_LIST_CHANGED "PresetListChanged"
#define ACTION_MESSAGE_SELECT_PRESET "SelectPreset "
#define ACTION_MESSAGE_SAVE_PRESET "SavePreset"

#define RETURN_TYPE_NO          1
#define RETURN_TYPE_YES         2
#define RETURN_TYPE_YESFORALL   3
#define RETURN_TYPE_CANCEL      0

class PresetHelper : public ActionBroadcaster
{
public:
    PresetHelper(File presetDirectory, ActionListener* pActionListener);
    virtual ~PresetHelper() {}
    virtual bool checkValid(File presetFile) = 0;
    
    void restoreDefaults();
    void initialize();
    
    void selectPresetName(String name);
    
    File* tryCreateNewPreset();
    void tryDeletePresets(Array<String> presetNames);
    void tryDeleteAll();
    void tryImportFiles(Array<File> files);

    void notifyPresetChanged();
    void notifyPresetListChanged();
    
    int showOverwriteDialog(String filename);
    
    void buildDefaultPreset(String name, String fileContent);

    Array<File> presetFiles;
    
protected:
    File getPathForPresetName(String name);
    
private:
    void selectPreset(File file);
    virtual void restoreDefaultsInternal() = 0;
    
private:
    File presetDirectory;
};
