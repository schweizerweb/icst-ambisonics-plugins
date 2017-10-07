/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/PointSelection.h"
#include "PresetInfo.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SpeakerSettings  : public Component,
                         public TableListBoxModel,
                         public ChangeListener,
                         public ComboBoxListener,
                         public ButtonListener
{
public:
    //==============================================================================
    SpeakerSettings (Array<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection);
    ~SpeakerSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	static void showAsDialog(Array<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection);
	int getNumRows() override;
	void selectedRowsChanged(int lastRowSelected) override;
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	void loadPreset(PresetInfo* preset) const;
	void updateComboBox(String elementToSelect = String::empty) const;
	void changeListenerCallback(ChangeBroadcaster* source) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Array<AmbiPoint>* pSpeakerArray;
	OwnedArray<PresetInfo>* pPresets;
	PointSelection* pPointSelection;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> comboBoxChannelConfig;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> buttonLoad;
    ScopedPointer<TextButton> buttonSave;
    ScopedPointer<TableListBox> speakerList;
    ScopedPointer<TextButton> buttonAdd;
    ScopedPointer<TextButton> buttonRemove;
    ScopedPointer<TextButton> buttonMoveDown;
    ScopedPointer<TextButton> buttonMoveUp;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeakerSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
