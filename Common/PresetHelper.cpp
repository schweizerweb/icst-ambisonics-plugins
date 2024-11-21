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



#include "PresetHelper.h"

PresetHelper::PresetHelper(File _presetDirectory, ActionListener* pActionListener): presetDirectory(_presetDirectory)
{
    if (!presetDirectory.exists())
        presetDirectory.createDirectory();

    addActionListener(pActionListener);
}

void PresetHelper::initialize()
{
    presetFiles.clear();
    bool hasPresets = false;
    RangedDirectoryIterator iterator(presetDirectory, false, "*.xml");
    for(DirectoryEntry entry : iterator)
    {
        // try to load preset
        if(checkValid(entry.getFile()))
        {
            presetFiles.add(entry.getFile());
            hasPresets = true;
        }
    }

    presetFiles.sort();

    if(!hasPresets)
        restoreDefaults();
}

void PresetHelper::selectPresetName(String name)
{
    for(File file : presetFiles)
    {
        if(file.getFileNameWithoutExtension() == name)
        {
            selectPreset(file);
            return;
        }
    }
}

void PresetHelper::restoreDefaults()
{
    restoreDefaultsInternal();
    presetFiles.sort();
    notifyPresetListChanged();
}

void PresetHelper::notifyPresetChanged()
{
    sendActionMessage(ACTION_MESSAGE_PRESET_CHANGED);
}

void PresetHelper::notifyPresetListChanged()
{
    sendActionMessage(ACTION_MESSAGE_PRESET_LIST_CHANGED);
}

void PresetHelper::tryCreateNewPreset(std::function<void(File*)> callback)
{
    alertWindow.reset(new AlertWindow("Save Preset", "", AlertWindow::NoIcon));
    Array<String> existingPresets;
    existingPresets.add("");
    for (File file : presetFiles)
        existingPresets.add(file.getFileNameWithoutExtension());

    alertWindow->addComboBox("existing", existingPresets, "Overwrite existing");
    alertWindow->addTextEditor("text", "", "Or enter new name", false);
    alertWindow->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
    alertWindow->addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
    
    alertWindow->enterModalState(true, ModalCallbackFunction::create([&, callback](int returnValue) {
        if (returnValue == 1)
        {
            String presetName = alertWindow->getTextEditorContents("text");
            if (presetName.isEmpty())
                presetName = alertWindow->getComboBoxComponent("existing")->getText();

            alertWindow = nullptr;

            if (presetName.isEmpty() || File::createLegalFileName(presetName) != presetName)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error", "Invalid preset name: " + presetName);
                return;
            }

            File newFile = getPathForPresetName(presetName);
            if (newFile.existsAsFile())
            {
                AlertWindow confirm("Overwrite?", "Are you sure to overwrite preset \"" + presetName + "\"?", AlertWindow::QuestionIcon);
                confirm.addButton("No", 0, KeyPress(KeyPress::escapeKey, 0, 0));
                confirm.addButton("Yes", 1, KeyPress(KeyPress::returnKey, 0, 0));
                if (confirm.runModalLoop() == 0)
                {
                    return;
                }
            }

            presetFiles.addIfNotAlreadyThere(newFile);
            notifyPresetListChanged();
            callback(&newFile);
        }
        else
        {
            alertWindow = nullptr;
        }
        }));
    
    return;
}

bool PresetHelper::tryDeletePresets(Array<String> presetNames)
{
    String message = "Following Presets will be deleted and cannot be restored:\r\n\r\n";
    for(String name : presetNames)
    {
        message += name + "\r\n";
    }
    message += "\r\nProceed?";

    if(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Confirm Delete", message))
    {
        for(String name : presetNames)
        {
            for(File file : presetFiles)
            {
                if(file.getFileNameWithoutExtension() == name)
                {
                    int index = presetFiles.indexOf(file);
                    presetFiles.remove(index);
                    file.deleteFile();
                    break;
                }
            }
        }

        notifyPresetListChanged();
        return true;
    }

    return false;
}

bool PresetHelper::tryDeleteAll()
{
    Array<String> allNames;
    for(File f : presetFiles)
        allNames.add(f.getFileNameWithoutExtension());
    return tryDeletePresets(allNames);
}

void PresetHelper::tryImportFiles(Array<File> files)
{
    bool overwriteAll = false;
    bool presetListChanged = false;
    String errorMessage;

    for(File file : files)
    {
        if(checkValid(file))
        {
            File newFile = getPathForPresetName(file.getFileNameWithoutExtension());
            if(newFile.exists() && !overwriteAll)
            {
                int returnType = showOverwriteDialog(newFile.getFileNameWithoutExtension());
                if(returnType == RETURN_TYPE_NO)
                    continue;
                else if(returnType == RETURN_TYPE_YESFORALL)
                    overwriteAll = true;
                else if(returnType == RETURN_TYPE_CANCEL)
                {
                    errorMessage += "Import cancelled!";
                    break;
                }
            }

            file.copyFileTo(newFile);
            presetFiles.addIfNotAlreadyThere(newFile);
            presetListChanged = true;
        }
        else
        {
            errorMessage += file.getFullPathName() + "\r\n";
        }
    }

    if(presetListChanged)
    {
        presetFiles.sort();
        notifyPresetListChanged();
    }
    
    if(!errorMessage.isEmpty())
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Preset import", "Error loading presets: \r\n" + errorMessage);
    }
}


int PresetHelper::showOverwriteDialog(String filename)
{
    AlertWindow alert("Existing file(s)", filename + " already exists, overwrite?", AlertWindow::AlertIconType::WarningIcon);
    alert.addButton("No", RETURN_TYPE_NO);
    alert.addButton("Yes", RETURN_TYPE_YES);
    alert.addButton("Yes for All", RETURN_TYPE_YESFORALL);
    alert.addButton("Cancel", RETURN_TYPE_CANCEL);
    return alert.runModalLoop();
}

void PresetHelper::selectPreset(File file)
{
    sendActionMessage(String(UniqueActionMessageSelectPreset()) + file.getFullPathName());
}

File PresetHelper::getPathForPresetName(String name)
{
    return File(presetDirectory.getFullPathName() + "/" + name + ".xml");
}

void PresetHelper::buildDefaultPreset(String name, String fileContent)
{
    File file = getPathForPresetName(name);
    file.replaceWithText(fileContent);
    presetFiles.addIfNotAlreadyThere(file);
}
