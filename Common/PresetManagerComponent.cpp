/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PresetManagerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define RETURN_TYPE_NO          1
#define RETURN_TYPE_YES         2
#define RETURN_TYPE_YESFORALL   3
#define RETURN_TYPE_CANCEL      0
//[/MiscUserDefs]

//==============================================================================
PresetManagerComponent::PresetManagerComponent (Array<File>* pPresetFiles, File presetDirectory, ActionListener* pActionListener, PresetHelper* pPresetHelper)
    : pPresetFiles(pPresetFiles), presetDirectory(presetDirectory), pPresetHelper(pPresetHelper)
{
    //[Constructor_pre] You can add your own custom stuff here..

    //[/Constructor_pre]

    presetTable.reset (new TableListBox());
    addAndMakeVisible (presetTable.get());
    presetTable->setName ("presetTable");

    btnRemove.reset (new TextButton ("btnRemove"));
    addAndMakeVisible (btnRemove.get());
    btnRemove->setButtonText (TRANS("Remove"));
    btnRemove->addListener (this);

    btnRemoveAll.reset (new TextButton ("btnRemoveAll"));
    addAndMakeVisible (btnRemoveAll.get());
    btnRemoveAll->setButtonText (TRANS("Remove All"));
    btnRemoveAll->addListener (this);

    btnExportAll.reset (new TextButton ("btnExportAll"));
    addAndMakeVisible (btnExportAll.get());
    btnExportAll->setButtonText (TRANS("Export All"));
    btnExportAll->addListener (this);

    btnImport.reset (new TextButton ("btnImport"));
    addAndMakeVisible (btnImport.get());
    btnImport->setButtonText (TRANS("Import"));
    btnImport->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    addActionListener(pActionListener);
    tableModel.reset(new PresetTableModel(pPresetFiles, this));
    tableModel->initTable(presetTable.get());
    presetTable->setMultipleSelectionEnabled(true);
    //[/Constructor]
}

PresetManagerComponent::~PresetManagerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    presetTable = nullptr;
    btnRemove = nullptr;
    btnRemoveAll = nullptr;
    btnExportAll = nullptr;
    btnImport = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PresetManagerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
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
    btnImport->setBounds (getWidth() - 118 - 104, getHeight() - 8 - 24, 104, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PresetManagerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnRemove.get())
    {
        //[UserButtonCode_btnRemove] -- add your button handler code here..
        Array<File> filesToDelete;
        SparseSet<int> selectedRows = presetTable->getSelectedRows();
        for(int i = 0; i < selectedRows.size(); i++)
        {
            filesToDelete.add((*pPresetFiles)[selectedRows[i]]);
        }
        tryDeleteFiles(filesToDelete);
        
        //[/UserButtonCode_btnRemove]
    }
    else if (buttonThatWasClicked == btnRemoveAll.get())
    {
        //[UserButtonCode_btnRemoveAll] -- add your button handler code here..
        tryDeleteFiles(*pPresetFiles);
        //[/UserButtonCode_btnRemoveAll]
    }
    else if (buttonThatWasClicked == btnExportAll.get())
    {
        //[UserButtonCode_btnExportAll] -- add your button handler code here..
        bool overwriteAll = false;
        FileChooser chooser("Select directory to export...", File::getSpecialLocation(File::userHomeDirectory));
        if (chooser.browseForDirectory())
        {
            for(File file : (*pPresetFiles))
            {
                File target(chooser.getResult().getFullPathName() + "/" + file.getFileName());
                if(target.exists() && !overwriteAll)
                {
                    int returnType = showOverwriteDialog(target.getFullPathName());
                    if(returnType == RETURN_TYPE_NO)
                        continue;
                    else if(returnType == RETURN_TYPE_YESFORALL)
                        overwriteAll = true;
                    else if(returnType == RETURN_TYPE_CANCEL)
                    {
                        return;
                    }
                }
                if(!file.copyFileTo(target))
                {
                    AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset export", "Error exporting preset to " + target.getFullPathName());
                }
            }
        }
        //[/UserButtonCode_btnExportAll]
    }
    else if (buttonThatWasClicked == btnImport.get())
    {
        //[UserButtonCode_btnImport] -- add your button handler code here..
        FileChooser chooser("Select preset XML to import...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
        if (chooser.browseForMultipleFilesToOpen())
        {
            tryImportFiles(chooser.getResults());
        }
        //[/UserButtonCode_btnImport]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PresetManagerComponent::filesDropped (const StringArray& filenames, int mouseX, int mouseY)
{
    //[UserCode_filesDropped] -- Add your code here...
    Array<File> fileArray;
    for(String name : filenames)
    {
        fileArray.add(File(name));
    }
    tryImportFiles(fileArray);
    //[/UserCode_filesDropped]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PresetManagerComponent::tryImportFiles(Array<File> files)
{
    bool overwriteAll = false;
    String errorMessage;
    
    for(File file : files)
    {
        if(pPresetHelper->checkValid(file))
        {
            File newFile(presetDirectory.getFullPathName() + "/" + file.getFileName());
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
            pPresetFiles->addIfNotAlreadyThere(newFile);
            sendActionMessage(ACTION_MESSAGE_PRESETS_CHANGED);
        }
        else
        {
            errorMessage += file.getFullPathName() + "\r\n";
        }
    }
    
    if(!errorMessage.isEmpty())
    {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset import", "Error loading presets: \r\n" + errorMessage);
    }
    
    
    presetTable->updateContent();
    presetTable->repaint();
}

void PresetManagerComponent::tryDeleteFiles(Array<File> files)
{
    String message = "Following Presets will be deleted and cannot be restored:\r\n\r\n";
    for(File file : files)
    {
        message += file.getFileNameWithoutExtension() + "\r\n";
    }
    message += "\r\nProceed?";
    
    if(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Confirm Delete", message))
    {
        for(File file : files)
        {
            int index = pPresetFiles->indexOf(file);
            pPresetFiles->remove(index);
            file.deleteFile();
        }
        
        sendActionMessage(ACTION_MESSAGE_PRESETS_CHANGED);
    }
    
    presetTable->updateContent();
    presetTable->repaint();
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

int PresetManagerComponent::showOverwriteDialog(String filename)
{
    AlertWindow alert("Existing file(s)", filename + " already exists, overwrite?", AlertWindow::AlertIconType::WarningIcon);
    alert.addButton("No", RETURN_TYPE_NO);
    alert.addButton("Yes", RETURN_TYPE_YES);
    alert.addButton("Yes for All", RETURN_TYPE_YESFORALL);
    alert.addButton("Cancel", RETURN_TYPE_CANCEL);
    return alert.runModalLoop();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PresetManagerComponent" componentName=""
                 parentClasses="public Component, public ActionBroadcaster, public FileDragAndDropTarget"
                 constructorParams="Array&lt;File&gt;* pPresetFiles, File presetDirectory, ActionListener* pActionListener, PresetHelper* pPresetHelper"
                 variableInitialisers="pPresetFiles(pPresetFiles), presetDirectory(presetDirectory), pPresetHelper(pPresetHelper)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
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
              virtualName="" explicitFocusOrder="0" pos="118Rr 8Rr 104 24"
              buttonText="Import" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

