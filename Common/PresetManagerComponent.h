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
                                public FileDragAndDropTarget,
                                ActionListener,
                                public juce::Button::Listener
{
public:
    //==============================================================================
    PresetManagerComponent (PresetHelper* pPresetHelper, bool allowApply);
    ~PresetManagerComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void controlDimming();
    void actionListenerCallback(const String &message) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void filesDropped (const juce::StringArray& filenames, int mouseX, int mouseY) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PresetHelper* pPresetHelper;
    std::unique_ptr<PresetTableModel> tableModel;
    void exportToFolder(Array<File> presetsToExport, String exportFolder);
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TableListBox> presetTable;
    std::unique_ptr<juce::TextButton> btnRemove;
    std::unique_ptr<juce::TextButton> btnRemoveAll;
    std::unique_ptr<juce::TextButton> btnExportAll;
    std::unique_ptr<juce::TextButton> btnImport;
    std::unique_ptr<juce::TextButton> btnRestoreDefaults;
    std::unique_ptr<juce::TextButton> btnExport;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManagerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

