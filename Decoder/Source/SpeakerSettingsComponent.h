/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "../../Common/PointSelection.h"
#include "DecoderAmbiSettings.h"
#include "DecoderSettings.h"
#include "../../Common/MultiSliderControl.h"
#include "../../Common/TableColumnCallback.h"
#include "../../Common/DelayHelper.h"
#include "../../Common/TestSoundGenerator.h"
#include "../../Common/AmbiSpeakerSet.h"
#include "FilterSettingsComponent.h"
#include "DecoderPresetHelper.h"
#include "FilterPresetHelper.h"
#include "../../Common/PresetManagerDialog.h"
#include "../../Common/ZoomSettings.h"
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
                                  public ChangeBroadcaster,
                                  public TableColumnCallback,
                                  ActionListener,
                                  public juce::ComboBox::Listener,
                                  public juce::Button::Listener,
                                  public juce::Slider::Listener
{
public:
    //==============================================================================
    SpeakerSettingsComponent (AmbiSpeakerSet* pSpeakerSet, DecoderPresetHelper* pPresetHelper, PointSelection* pPointSelection, DecoderAmbiSettings* pAmbiSettings, DecoderSettings* pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* pFilterSpecification, ZoomSettings* pZoomSettings);
    ~SpeakerSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    // table overrides
	int getNumRows() override;
	void selectedRowsChanged(int lastRowSelected) override;
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;

	// table item callbacks
	String getTableText(const int columnId, const int rowNumber) override;
	void setTableText(const int columnId, const int rowNumber, const String& newText) override;
	void setValue(int columnId, int rowNumber, double newValue) override;
	double getValue(int columnId, int rowNumber) override;
    void setFlag(int columnId, int rowNumber, bool newValue) const;
    bool getFlag(int columnId, int rowNumber) const;
	void speakerTest(int rowNumber) const;
	TableListBox* getTable() override;
    SliderRange getSliderRange(int columnId) override;
    bool getEnabled(const int columnId, const int rowNumber) override;


	void updateComboBox() const;
	void changeListenerCallback(ChangeBroadcaster* source) override;
    void actionListenerCallback(const String &message) override;
	void updateUI() const;
	FilterBankInfo* getFilterInfo(int rowNumber) const;
	dsp::ProcessSpec* getFilterSpecification() const;
	void controlDimming();
    FilterPresetHelper* getFilterPresetHelper() const;
    void initChannelWeightSliders();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbiSpeakerSet* pSpeakerSet;
	DecoderPresetHelper* pPresetHelper;
	PointSelection* pPointSelection;
	DecoderAmbiSettings* pAmbiSettings;
	DecoderSettings* pDecoderSettings;
	DelayHelper delayHelper;
	TestSoundGenerator* pTestSoundGenerator;
	dsp::ProcessSpec* pFilterSpecification;
    PresetManagerDialog presetManagerDialog;
    std::unique_ptr<FilterPresetHelper> filterPresetHelper;
    ZoomSettings* pZoomSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupOsc;
    std::unique_ptr<juce::GroupComponent> groupAmbisonics;
    std::unique_ptr<juce::GroupComponent> groupSpeakers;
    std::unique_ptr<juce::ComboBox> comboBoxChannelConfig;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;
    std::unique_ptr<TableListBox> speakerList;
    std::unique_ptr<juce::TextButton> buttonAdd;
    std::unique_ptr<juce::TextButton> buttonRemove;
    std::unique_ptr<juce::TextButton> buttonMoveDown;
    std::unique_ptr<juce::TextButton> buttonMoveUp;
    std::unique_ptr<MultiSliderControl> ambiChannelControl;
    std::unique_ptr<juce::Label> labelChannelWeights;
    std::unique_ptr<juce::ToggleButton> btnEditMode;
    std::unique_ptr<juce::Label> labelOscPort;
    std::unique_ptr<juce::Label> labelTimeout;
    std::unique_ptr<juce::ToggleButton> toggleOsc;
    std::unique_ptr<juce::TextButton> buttonSpeakerTest;
    std::unique_ptr<juce::Label> labelDevelopmentVersion;
    std::unique_ptr<juce::TextButton> buttonManage;
    std::unique_ptr<juce::ComboBox> comboBoxChannelWeightingMode;
    std::unique_ptr<juce::TextButton> buttonManageFilters;
    std::unique_ptr<juce::ComboBox> comboBoxApply;
    std::unique_ptr<juce::TextButton> buttonCsv;
    std::unique_ptr<juce::TextButton> buttonScaling;
    std::unique_ptr<juce::Slider> sliderPort;
    std::unique_ptr<juce::Slider> sliderTimeout;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeakerSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

