/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PresetManagerComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PresetManagerComponent::PresetManagerComponent (PresetHelper* pPresetHelper, bool allowApply)
    : pPresetHelper(pPresetHelper)
{
    //[Constructor_pre] You can add your own custom stuff here..

    //[/Constructor_pre]

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


    //[UserPreSize]
    //[/UserPreSize]

    setSize (700, 400);


    //[Constructor] You can add your own custom stuff here..
    tableModel.reset(new PresetTableModel(pPresetHelper, this));
    tableModel->initTable(presetTable.get(), allowApply);
    presetTable->setMultipleSelectionEnabled(true);
    pPresetHelper->addActionListener(this);
    //[/Constructor]
}

PresetManagerComponent::~PresetManagerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    presetTable = nullptr;
    btnRemove = nullptr;
    btnRemoveAll = nullptr;
    btnExportAll = nullptr;
    btnImport = nullptr;
    btnRestoreDefaults = nullptr;
    btnExport = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PresetManagerComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    controlDimming();
    //[/UserPaint]
}

void PresetManagerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    presetTable->setBounds (8, 8, getWidth() - 16, getHeight() - 45);
    btnRemove->setBounds (8, getHeight() - 8 - 24, 104, 24);
    btnRemoveAll->setBounds (120, getHeight() - 8 - 24, 104, 24);
    btnExportAll->setBounds (getWidth() - 8 - 104, getHeight() - 8 - 24, 104, 24);
    btnImport->setBounds (getWidth() - 232 - 104, getHeight() - 8 - 24, 104, 24);
    btnRestoreDefaults->setBounds (232, getHeight() - 8 - 24, 120, 24);
    btnExport->setBounds (getWidth() - 120 - 104, getHeight() - 8 - 24, 104, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PresetManagerComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnRemove.get())
    {
        //[UserButtonCode_btnRemove] -- add your button handler code here..
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
        //[/UserButtonCode_btnRemove]
    }
    else if (buttonThatWasClicked == btnRemoveAll.get())
    {
        //[UserButtonCode_btnRemoveAll] -- add your button handler code here..
        pPresetHelper->tryDeleteAll();
        //[/UserButtonCode_btnRemoveAll]
    }
    else if (buttonThatWasClicked == btnExportAll.get())
    {
        //[UserButtonCode_btnExportAll] -- add your button handler code here..
        FileChooser chooser("Select directory to export...", File::getSpecialLocation(File::userHomeDirectory));
        if (chooser.browseForDirectory())
        {
            exportToFolder(pPresetHelper->presetFiles, chooser.getResult().getFullPathName());
        }
        //[/UserButtonCode_btnExportAll]
    }
    else if (buttonThatWasClicked == btnImport.get())
    {
        //[UserButtonCode_btnImport] -- add your button handler code here..
        FileChooser chooser("Select preset XML to import...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
        if (chooser.browseForMultipleFilesToOpen())
        {
            pPresetHelper->tryImportFiles(chooser.getResults());
        }
        //[/UserButtonCode_btnImport]
    }
    else if (buttonThatWasClicked == btnRestoreDefaults.get())
    {
        //[UserButtonCode_btnRestoreDefaults] -- add your button handler code here..
        if(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Restore Defaults", "All user changes to default presets will be overwritten, continue?", String(), String(), nullptr, nullptr))
        {
            pPresetHelper->restoreDefaults();
        }
        //[/UserButtonCode_btnRestoreDefaults]
    }
    else if (buttonThatWasClicked == btnExport.get())
    {
        //[UserButtonCode_btnExport] -- add your button handler code here..
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
        //[/UserButtonCode_btnExport]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PresetManagerComponent::filesDropped (const juce::StringArray& filenames, int /*mouseX*/, int /*mouseY*/)
{
    //[UserCode_filesDropped] -- Add your code here...
    Array<File> fileArray;
    for(String name : filenames)
    {
        fileArray.add(File(name));
    }

    pPresetHelper->tryImportFiles(fileArray);
    //[/UserCode_filesDropped]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PresetManagerComponent" componentName=""
                 parentClasses="public Component, public FileDragAndDropTarget, ActionListener"
                 constructorParams="PresetHelper* pPresetHelper, bool allowApply"
                 variableInitialisers="pPresetHelper(pPresetHelper)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="700" initialHeight="400">
  <METHODS>
    <METHOD name="filesDropped (const StringArray&amp; filenames, int mouseX, int mouseY)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="presetTable" id="e97fa46fceef853d" memberName="presetTable"
                    virtualName="" explicitFocusOrder="0" pos="8 8 16M 45M" class="TableListBox"
                    params=""/>
  <TEXTBUTTON name="btnRemove" id="35aff5008999ac55" memberName="btnRemove"
              virtualName="" explicitFocusOrder="0" pos="8 8Rr 104 24" buttonText="Remove"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnRemoveAll" id="967cba23dec15a81" memberName="btnRemoveAll"
              virtualName="" explicitFocusOrder="0" pos="120 8Rr 104 24" buttonText="Remove All"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnExportAll" id="9394823c0048b5e8" memberName="btnExportAll"
              virtualName="" explicitFocusOrder="0" pos="8Rr 8Rr 104 24" buttonText="Export All"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnImport" id="8bf69d278e8d3cf0" memberName="btnImport"
              virtualName="" explicitFocusOrder="0" pos="232Rr 8Rr 104 24"
              buttonText="Import" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnRestoreDefaults" id="c5da3bad1b135575" memberName="btnRestoreDefaults"
              virtualName="" explicitFocusOrder="0" pos="232 8Rr 120 24" buttonText="Restore Defaults"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnExport" id="32141f834035c38b" memberName="btnExport"
              virtualName="" explicitFocusOrder="0" pos="120Rr 8Rr 104 24"
              buttonText="Export" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

