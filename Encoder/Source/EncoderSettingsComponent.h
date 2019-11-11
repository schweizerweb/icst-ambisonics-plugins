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
#include "EncoderSettings.h"
#include "GroupTableListModel.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/TableColumnCallback.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EncoderSettingsComponent  : public Component,
                                  public TextEditor::Listener,
                                  public ActionBroadcaster,
                                  public ChangeBroadcaster,
                                  public TableListBoxModel,
                                  public ChangeListener,
                                  public TableColumnCallback,
                                  public Button::Listener,
                                  public ComboBox::Listener
{
public:
    //==============================================================================
    EncoderSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams);
    ~EncoderSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void checkForNumbers(TextEditor* pEditor, int* pParameter) const;
	void checkForNumbers(TextEditor* pEditor, float* pParameter) const;
	void textEditorTextChanged(TextEditor&) override;
	void controlDimming() const;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	int getNumRows() override;
	void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	double getValue(int columnId, int rowNumber) override;
	void setValue(int columnId, int rowNumber, double newValue) override;
	SliderRange getSliderRange(int columnId) override;
	TableListBox* getTable() override;
	String getTableText(const int columnId, const int rowNumber) override;
	void setTableText(const int columnId, const int rowNumber, const String& newText) override;
	void selectedRowsChanged(int lastRowSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate) override;
	void initializePresets();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	EncoderSettings* pEncoderSettings;
	AmbiSourceSet* pSources;
	PointSelection* pPointSelection;
	Array<AudioParameterSet>* pAudioParams;
	std::unique_ptr<GroupTableListModel> groupModel;
	Array<File> presets;
	File presetDirectory;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Component> multiEncoderElements;
    std::unique_ptr<GroupComponent> groupSources;
    std::unique_ptr<GroupComponent> groupOsc;
    std::unique_ptr<ToggleButton> toggleReceiveOsc;
    std::unique_ptr<TextEditor> textOscReceivePort;
    std::unique_ptr<Label> labelOscPort;
    std::unique_ptr<ToggleButton> toggleSendOsc;
    std::unique_ptr<TextEditor> textOscSendIp;
    std::unique_ptr<Label> labelOscSendIp;
    std::unique_ptr<TextEditor> textOscSendPort;
    std::unique_ptr<TextEditor> textOscSendInterval;
    std::unique_ptr<Label> labelOscSendInterval;
    std::unique_ptr<GroupComponent> groupEncoding;
    std::unique_ptr<ToggleButton> toggleDistanceEncoding;
    std::unique_ptr<TextEditor> textUnitCircleRadius;
    std::unique_ptr<Label> labelUnitCircleRadius;
    std::unique_ptr<ToggleButton> toggleDirectionFlip;
    std::unique_ptr<ToggleButton> toggleSendOscExt;
    std::unique_ptr<TextEditor> textOscSendIpExt;
    std::unique_ptr<Label> labelOscSendIpExt;
    std::unique_ptr<TextEditor> textOscSendPortExt;
    std::unique_ptr<TableListBox> sourceList;
    std::unique_ptr<TextButton> buttonAdd;
    std::unique_ptr<TextButton> buttonRemove;
    std::unique_ptr<TextButton> buttonMoveDown;
    std::unique_ptr<TextButton> buttonMoveUp;
    std::unique_ptr<GroupComponent> groupGroups;
    std::unique_ptr<TableListBox> groupList;
    std::unique_ptr<TextButton> buttonAddGroup;
    std::unique_ptr<TextButton> buttonRemoveGroup;
    std::unique_ptr<ComboBox> comboBoxPresets;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<TextButton> buttonImport;
    std::unique_ptr<TextButton> buttonSave;
    std::unique_ptr<TextButton> buttonExport;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncoderSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

