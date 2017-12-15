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
#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/PointSelection.h"
#include "../../Common/AmbiSettings.h"
#include "../../Common/MultiSliderControl.h"
#include "PresetInfo.h"
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
                                  public ActionBroadcaster,
                                  public ComboBoxListener,
                                  public ButtonListener,
                                  public SliderListener
{
public:
    //==============================================================================
    SpeakerSettingsComponent (Array<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, ActionListener* pTestSoundListener);
    ~SpeakerSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	static void showAsDialog(Array<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, ActionListener* pTestSoundListener);

	// table overrides
	int getNumRows() override;
	void selectedRowsChanged(int lastRowSelected) override;
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;

	// table item callbacks
	String getTableText(const int columnId, const int rowNumber) const;
	void setTableText(const int columnId, const int rowNumber, const String& newText) const;
	void setGain(int rowNumber, double newValue) const;
	double getGain(int rowNumber) const;
	void speakerTest(int rowNumber) const;
	TableListBox* getTable() const;

	void updateDirectionFlip() const;
	void loadPreset(PresetInfo* preset) const;
	void updateComboBox(String elementToSelect = String::empty) const;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	bool CheckForExistingPreset(String newPresetName) const;
	void updateDistanceScaler() const;
	static int fact(int n);
	void setInPhaseWeighting(AmbiSettings* pSettings);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Array<AmbiPoint>* pSpeakerArray;
	OwnedArray<PresetInfo>* pPresets;
	PointSelection* pPointSelection;
	AmbiSettings* pAmbiSettings;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> groupSpeakers;
    ScopedPointer<ComboBox> comboBoxChannelConfig;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> buttonLoad;
    ScopedPointer<TextButton> buttonSave;
    ScopedPointer<TableListBox> speakerList;
    ScopedPointer<TextButton> buttonAdd;
    ScopedPointer<TextButton> buttonRemove;
    ScopedPointer<TextButton> buttonMoveDown;
    ScopedPointer<TextButton> buttonMoveUp;
    ScopedPointer<GroupComponent> groupAmbisonics;
    ScopedPointer<Label> label2;
    ScopedPointer<Slider> sliderDistanceScaler;
    ScopedPointer<MultiSliderControl> ambiChannelControl;
    ScopedPointer<Label> label3;
    ScopedPointer<TextButton> buttonInPhase;
    ScopedPointer<TextButton> buttonBasic;
    ScopedPointer<ToggleButton> btnFlipDirection;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeakerSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
