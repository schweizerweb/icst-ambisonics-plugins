/*
  ==============================================================================

    PresetHelper.cpp
    Created: 26 Jan 2020 10:24:27am
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "PresetHelper.h"

PresetHelper::PresetHelper(File presetDirectory, ActionListener* pActionListener): presetDirectory(presetDirectory)
{
    if (!presetDirectory.exists())
        presetDirectory.createDirectory();

    addActionListener(pActionListener);
}

void PresetHelper::initialize()
{
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

File* PresetHelper::tryCreateNewPreset()
{
    AlertWindow alert("Save Preset", "", AlertWindow::NoIcon);
    Array<String> existingPresets;
    existingPresets.add("");
    for (File file : presetFiles)
        existingPresets.add(file.getFileNameWithoutExtension());

    alert.addComboBox("existing", existingPresets, "Overwrite existing");
    alert.addTextEditor("text", "", "Or enter new name", false);
    alert.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
    alert.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));

    int returnValue = alert.runModalLoop();
    if(returnValue == 1)
    {
        String presetName = alert.getTextEditorContents("text");
        if(presetName.isEmpty())
            presetName = alert.getComboBoxComponent("existing")->getText();

        if (presetName.isEmpty() || File::createLegalFileName(presetName) != presetName)
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid preset name: " + presetName);
            return nullptr;
        }

        File newFile = getPathForPresetName(presetName);
        if (newFile.existsAsFile())
        {
            AlertWindow confirm("Overwrite?", "Are you sure to overwrite preset \"" + presetName + "\"?", AlertWindow::QuestionIcon);
            confirm.addButton("No", 0, KeyPress(KeyPress::escapeKey, 0, 0));
            confirm.addButton("Yes", 1, KeyPress(KeyPress::returnKey, 0, 0));
            if (confirm.runModalLoop() == 0)
            {
                return nullptr;
            }
        }
    
        presetFiles.addIfNotAlreadyThere(newFile);
        notifyPresetListChanged();
        return new File(newFile);
    }
    
    return nullptr;
}

void PresetHelper::tryDeletePresets(Array<String> presetNames)
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
    }
}

void PresetHelper::tryDeleteAll()
{
    Array<String> allNames;
    for(File f : presetFiles)
        allNames.add(f.getFileNameWithoutExtension());
    tryDeletePresets(allNames);
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
        notifyPresetListChanged();
    }
    
    if(!errorMessage.isEmpty())
    {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset import", "Error loading presets: \r\n" + errorMessage);
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
    sendActionMessage(String(ACTION_MESSAGE_SELECT_PRESET) + file.getFullPathName());
}

File PresetHelper::getPathForPresetName(String name)
{
    return File(presetDirectory.getFullPathName() + "/" + name + ".xml");
}
