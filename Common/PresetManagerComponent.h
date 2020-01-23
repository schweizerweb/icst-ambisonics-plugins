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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PresetHelper.h"
#include "PresetTableModel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PresetManagerComponent  : public Component,
                                public ActionBroadcaster,
                                public FileDragAndDropTarget,
                                public Button::Listener
{
public:
    //==============================================================================
    PresetManagerComponent (Array<File>* pPresetFiles, File presetDirectory, ActionListener* pActionListener, PresetHelper* pPresetHelper);
    ~PresetManagerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void tryImportFiles(Array<File> files);
    void tryDeleteFiles(Array<File> files);
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    int showOverwriteDialog(String filename);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void filesDropped (const StringArray& filenames, int mouseX, int mouseY) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Array<File>* pPresetFiles;
    File presetDirectory;
    PresetHelper* pPresetHelper;
    std::unique_ptr<PresetTableModel> tableModel;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TableListBox> presetTable;
    std::unique_ptr<TextButton> btnRemove;
    std::unique_ptr<TextButton> btnRemoveAll;
    std::unique_ptr<TextButton> btnExportAll;
    std::unique_ptr<TextButton> btnImport;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManagerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

