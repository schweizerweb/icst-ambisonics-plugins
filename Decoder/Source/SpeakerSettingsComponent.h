/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/PointSelection.h"
#include "../../Common/AmbiSettings.h"
#include "../../Common/AmbiDataSet.h"
#include "DecoderSettings.h"
#include "../../Common/MultiSliderControl.h"
#include "PresetInfo.h"
#include "SliderRange.h"
#include "../../Common/DelayHelper.h"
#include "../../Common/TestSoundGenerator.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SpeakerSettingsComponent  : public Component,
                                  public TableListBoxModel,
                                  public ChangeListener,
                                  public TextEditor::Listener,
                                  public ActionBroadcaster,
                                  public ChangeBroadcaster,
                                  public ComboBox::Listener,
                                  public Button::Listener,
                                  public Slider::Listener
{
public:
    //==============================================================================
    SpeakerSettingsComponent (AmbiDataSet* pSpeakerSet, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, DecoderSettings* pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback);
    ~SpeakerSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void textEditorTextChanged(TextEditor& editor) override;

    // table overrides
	int getNumRows() override;
	void selectedRowsChanged(int lastRowSelected) override;
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;

	// table item callbacks
	String getTableText(const int columnId, const int rowNumber) const;
	void setTableText(const int columnId, const int rowNumber, const String& newText) const;
	void setValue(int columnId, int rowNumber, double newValue) const;
	double getValue(int columnId, int rowNumber) const;
	void speakerTest(int rowNumber) const;
	TableListBox* getTable() const;
	SliderRange getSliderRange(int columnId) const;

	void updateDirectionFlip() const;
	void loadPreset(PresetInfo* preset) const;
	void updateComboBox(String elementToSelect = String()) const;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	bool CheckForExistingPreset(String newPresetName) const;
	void updateDistanceScaler() const;
	static double fact(int n);
	void setInPhaseWeighting(AmbiSettings* pSettings) const;
	void controlDimming();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbiDataSet* pSpeakerSet;
	OwnedArray<PresetInfo>* pPresets;
	PointSelection* pPointSelection;
	AmbiSettings* pAmbiSettings;
	DecoderSettings* pDecoderSettings;
	DelayHelper delayHelper;
	TestSoundGenerator* pTestSoundGenerator;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupOsc;
    std::unique_ptr<GroupComponent> groupAmbisonics;
    std::unique_ptr<GroupComponent> groupSpeakers;
    std::unique_ptr<ComboBox> comboBoxChannelConfig;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<TextButton> buttonLoad;
    std::unique_ptr<TextButton> buttonSave;
    std::unique_ptr<TableListBox> speakerList;
    std::unique_ptr<TextButton> buttonAdd;
    std::unique_ptr<TextButton> buttonRemove;
    std::unique_ptr<TextButton> buttonMoveDown;
    std::unique_ptr<TextButton> buttonMoveUp;
    std::unique_ptr<Slider> sliderDistanceScaler;
    std::unique_ptr<MultiSliderControl> ambiChannelControl;
    std::unique_ptr<TextButton> buttonBasic;
    std::unique_ptr<Label> labelChannelWeights;
    std::unique_ptr<TextButton> buttonInPhase;
    std::unique_ptr<ToggleButton> btnFlipDirection;
    std::unique_ptr<Label> labelDistanceScaler;
    std::unique_ptr<ToggleButton> btnEditMode;
    std::unique_ptr<TextEditor> textOscPort;
    std::unique_ptr<Label> labelOscPort;
    std::unique_ptr<TextEditor> textTimeout;
    std::unique_ptr<Label> labelTimeout;
    std::unique_ptr<ToggleButton> toggleOsc;
    std::unique_ptr<TextButton> buttonSpeakerTest;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeakerSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

