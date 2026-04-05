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

#include "PresetManagerComponent.h"

PresetManagerComponent::PresetManagerComponent (PresetHelper* _pPresetHelper, bool allowApply)
    : pPresetHelper(_pPresetHelper)
{
    presetTable.reset (new TableListBox());
    addAndMakeVisible (presetTable.get());
    presetTable->setName ("presetTable");

    btnRemove.reset (new juce::TextButton ("btnRemove"));
    addAndMakeVisible (btnRemove.get());
    btnRemove->setButtonText (TRANS("Remove"));
    btnRemove->addListener (this);

    btnRemoveAll.reset (new juce::TextButton ("btnRemoveAll"));
    addAndMakeVisible (btnRemoveAll.get());
    btnRemoveAll->setButtonText (TRANS("Remove All"));
    btnRemoveAll->addListener (this);

    btnExportAll.reset (new juce::TextButton ("btnExportAll"));
    addAndMakeVisible (btnExportAll.get());
    btnExportAll->setButtonText (TRANS("Export All"));
    btnExportAll->addListener (this);

    btnImport.reset (new juce::TextButton ("btnImport"));
    addAndMakeVisible (btnImport.get());
    btnImport->setButtonText (TRANS("Import"));
    btnImport->addListener (this);

    btnRestoreDefaults.reset (new juce::TextButton ("btnRestoreDefaults"));
    addAndMakeVisible (btnRestoreDefaults.get());
    btnRestoreDefaults->setButtonText (TRANS("Restore Defaults"));
    btnRestoreDefaults->addListener (this);

    btnExport.reset (new juce::TextButton ("btnExport"));
    addAndMakeVisible (btnExport.get());
    btnExport->setButtonText (TRANS("Export"));
    btnExport->addListener (this);

    setSize (700, 400);

    tableModel.reset(new PresetTableModel(pPresetHelper, this));
    tableModel->initTable(presetTable.get(), allowApply);
    presetTable->setMultipleSelectionEnabled(true);
    pPresetHelper->addActionListener(this);
}

PresetManagerComponent::~PresetManagerComponent()
{
    pPresetHelper->removeActionListener(this);
    presetTable->setModel(nullptr);

    presetTable = nullptr;
    btnRemove = nullptr;
    btnRemoveAll = nullptr;
    btnExportAll = nullptr;
    btnImport = nullptr;
    btnRestoreDefaults = nullptr;
    btnExport = nullptr;
}

void PresetManagerComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));

    controlDimming();
}

void PresetManagerComponent::resized()
{
    presetTable->setBounds (8, 8, getWidth() - 16, getHeight() - 45);
    btnRemove->setBounds (8, getHeight() - 8 - 24, 104, 24);
    btnRemoveAll->setBounds (120, getHeight() - 8 - 24, 104, 24);
    btnExportAll->setBounds (getWidth() - 8 - 104, getHeight() - 8 - 24, 104, 24);
    btnImport->setBounds (getWidth() - 232 - 104, getHeight() - 8 - 24, 104, 24);
    btnRestoreDefaults->setBounds (232, getHeight() - 8 - 24, 120, 24);
    btnExport->setBounds (getWidth() - 120 - 104, getHeight() - 8 - 24, 104, 24);
}

void PresetManagerComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnRemove.get())
    {
        Array<String> presetsToDelete;
        SparseSet<int> selectedRows = presetTable->getSelectedRows();
        for(int i = 0; i < presetTable->getNumRows(); i++)
        {
            if(presetTable->isRowSelected(i))
            {
                presetsToDelete.add(pPresetHelper->presetFiles[i].getFileNameWithoutExtension());
            }
        }

        pPresetHelper->tryDeletePresets(presetsToDelete);
    }
    else if (buttonThatWasClicked == btnRemoveAll.get())
    {
        pPresetHelper->tryDeleteAll();
    }
    else if (buttonThatWasClicked == btnExportAll.get())
    {
        FileChooser chooser("Select directory to export...", File::getSpecialLocation(File::userHomeDirectory));
        if (chooser.browseForDirectory())
        {
            exportToFolder(pPresetHelper->presetFiles, chooser.getResult().getFullPathName());
        }
    }
    else if (buttonThatWasClicked == btnImport.get())
    {
        FileChooser chooser("Select preset XML to import...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
        if (chooser.browseForMultipleFilesToOpen())
        {
            pPresetHelper->tryImportFiles(chooser.getResults());
        }
    }
    else if (buttonThatWasClicked == btnRestoreDefaults.get())
    {
        if(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Restore Defaults", "All user changes to default presets will be overwritten, continue?", String(), String(), nullptr, nullptr))
        {
            pPresetHelper->restoreDefaults();
        }
    }
    else if (buttonThatWasClicked == btnExport.get())
    {
        FileChooser chooser("Select directory to export...", File::getSpecialLocation(File::userHomeDirectory));
        if (chooser.browseForDirectory())
        {
            Array<File> presetsToExport;
            SparseSet<int> selectedRows = presetTable->getSelectedRows();
            for (int i = 0; i < presetTable->getNumRows(); i++)
            {
                if (presetTable->isRowSelected(i))
                {
                    presetsToExport.add(pPresetHelper->presetFiles[i]);
                }
            }

            String exportFolder = chooser.getResult().getFullPathName();
            exportToFolder(presetsToExport, exportFolder);
        }
    }
}

void PresetManagerComponent::filesDropped (const juce::StringArray& filenames, int /*mouseX*/, int /*mouseY*/)
{
    Array<File> fileArray;
    for(String name : filenames)
    {
        fileArray.add(File(name));
    }

    pPresetHelper->tryImportFiles(fileArray);
}

bool PresetManagerComponent::isInterestedInFileDrag(const juce::StringArray &files)
{
    for(String file : files)
    {
        File testFile(file);
        if(!testFile.exists() || testFile.getFileExtension() != ".xml")
            return false;
    }

    return true;
}

void PresetManagerComponent::controlDimming()
{
    btnRemove->setEnabled(presetTable->getSelectedRows().size() > 0);
    btnExport->setEnabled(presetTable->getSelectedRows().size() > 0);
    btnRemoveAll->setEnabled(pPresetHelper->presetFiles.size() > 0);
    btnExportAll->setEnabled(pPresetHelper->presetFiles.size() > 0);
}

void PresetManagerComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        presetTable->updateContent();
        presetTable->repaint();
    }
}

void PresetManagerComponent::exportToFolder(Array<File> presetsToExport, String exportFolder)
{
    bool overwriteAll = false;

    for (File file : presetsToExport)
    {
        File target(exportFolder + "/" + file.getFileName());
        if (target.exists() && !overwriteAll)
        {
            int returnType = pPresetHelper->showOverwriteDialog(target.getFullPathName());
            if (returnType == RETURN_TYPE_NO)
                continue;
            else if (returnType == RETURN_TYPE_YESFORALL)
                overwriteAll = true;
            else if (returnType == RETURN_TYPE_CANCEL)
            {
                break;
            }
        }
        if (!file.copyFileTo(target))
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Preset export", "Error exporting preset to " + target.getFullPathName());
        }
    }
}
