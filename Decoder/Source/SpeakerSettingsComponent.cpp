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

//[Headers] You can add your own extra header files here...
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "CheckBoxFilterCustomComponent.h"
#include "SpeakerTestCustomComponent.h"
#include "../../Common/TrackColors.h"
#include "../../Common/Constants.h"
#include "../../Common/ColorDefinition.h"
#include "FilterSettingsComponent.h"
#include "CsvImportExport.h"
#include "ScalingComponent.h"
//[/Headers]

#include "SpeakerSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define COLUMN_ID_NB		1
#define COLUMN_ID_NAME		2
#define	COLUMN_ID_X			7
#define	COLUMN_ID_Y			8
#define	COLUMN_ID_Z			9
#define	COLUMN_ID_A			10
#define	COLUMN_ID_E			11
#define COLUMN_ID_DISTANCE	3
#define COLUMN_ID_DELAY		4
#define COLUMN_ID_DELAY_COMPENSATION 13
#define COLUMN_ID_COLOR     15
#define COLUMN_ID_MUTE          16
#define COLUMN_ID_GAIN		5
#define	COLUMN_ID_TEST		6
#define COLUMN_ID_FILTER    14

#define ID_APPLY_COLOR      1
#define ID_APPLY_FILTER     2
#define ID_APPLY_GAIN       3
#define ID_APPLY_DEC_GAIN   4
#define ID_APPLY_INC_GAIN   5
//[/MiscUserDefs]

//==============================================================================
SpeakerSettingsComponent::SpeakerSettingsComponent (AmbiSpeakerSet* pSpeakerSet, DecoderPresetHelper* pPresetHelper, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, DecoderSettings* pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* pFilterSpecification, ZoomSettings* pZoomSettings)
    : pSpeakerSet(pSpeakerSet), pPresetHelper(pPresetHelper), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings),pDecoderSettings(pDecoderSettings), pFilterSpecification(pFilterSpecification), pZoomSettings(pZoomSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
	OwnedArray<String> ambiChannelNames;
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		ambiChannelNames.add(new String("m = " + String(i)));
	this->pTestSoundGenerator = pTestSoundListener;

	addChangeListener(pCallback);
    pPresetHelper->addActionListener(this);

    filterPresetHelper.reset(new FilterPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiDecoder/Filters"), this));
    filterPresetHelper->initialize();
    //[/Constructor_pre]

    groupOsc.reset (new juce::GroupComponent ("groupOsc",
                                              TRANS("OSC")));
    addAndMakeVisible (groupOsc.get());

    groupAmbisonics.reset (new juce::GroupComponent ("groupAmbisonics",
                                                     TRANS("Ambisonics")));
    addAndMakeVisible (groupAmbisonics.get());

    groupSpeakers.reset (new juce::GroupComponent ("groupSpeakers",
                                                   TRANS("Speakers")));
    addAndMakeVisible (groupSpeakers.get());

    comboBoxChannelConfig.reset (new juce::ComboBox ("channelConfig"));
    addAndMakeVisible (comboBoxChannelConfig.get());
    comboBoxChannelConfig->setEditableText (false);
    comboBoxChannelConfig->setJustificationType (juce::Justification::centredLeft);
    comboBoxChannelConfig->setTextWhenNothingSelected (TRANS("-"));
    comboBoxChannelConfig->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxChannelConfig->addListener (this);

    labelPresets.reset (new juce::Label ("labelPresets",
                                         TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (juce::Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPresets->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonSave.reset (new juce::TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    speakerList.reset (new TableListBox());
    addAndMakeVisible (speakerList.get());
    speakerList->setName ("speakerList");

    buttonAdd.reset (new juce::TextButton ("buttonAdd"));
    addAndMakeVisible (buttonAdd.get());
    buttonAdd->setButtonText (TRANS("add"));
    buttonAdd->addListener (this);

    buttonRemove.reset (new juce::TextButton ("buttonAdd"));
    addAndMakeVisible (buttonRemove.get());
    buttonRemove->setButtonText (TRANS("remove"));
    buttonRemove->addListener (this);

    buttonMoveDown.reset (new juce::TextButton ("buttonMoveDown"));
    addAndMakeVisible (buttonMoveDown.get());
    buttonMoveDown->setButtonText (TRANS("down"));
    buttonMoveDown->addListener (this);

    buttonMoveUp.reset (new juce::TextButton ("buttonMoveUp"));
    addAndMakeVisible (buttonMoveUp.get());
    buttonMoveUp->setButtonText (TRANS("up"));
    buttonMoveUp->addListener (this);

    ambiChannelControl.reset (new MultiSliderControl (CURRENT_AMBISONICS_ORDER_NB_OF_GAINS, pAmbiSettings->getAmbiOrderWeightPointer(), &ambiChannelNames, 0.0, 1.5, 0.00001));
    addAndMakeVisible (ambiChannelControl.get());
    ambiChannelControl->setName ("ambiChannelControl");

    labelChannelWeights.reset (new juce::Label ("labelChannelWeights",
                                                TRANS("Channel weights")));
    addAndMakeVisible (labelChannelWeights.get());
    labelChannelWeights->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelChannelWeights->setJustificationType (juce::Justification::centredLeft);
    labelChannelWeights->setEditable (false, false, false);
    labelChannelWeights->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelChannelWeights->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnEditMode.reset (new juce::ToggleButton ("btnEditMode"));
    addAndMakeVisible (btnEditMode.get());
    btnEditMode->setButtonText (TRANS("Edit mode"));
    btnEditMode->addListener (this);

    labelOscPort.reset (new juce::Label ("labelOscPort",
                                         TRANS("OSC-Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscPort->setJustificationType (juce::Justification::centredLeft);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelTimeout.reset (new juce::Label ("labelTimeout",
                                         TRANS("Timeout [ms]:")));
    addAndMakeVisible (labelTimeout.get());
    labelTimeout->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelTimeout->setJustificationType (juce::Justification::centredLeft);
    labelTimeout->setEditable (false, false, false);
    labelTimeout->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelTimeout->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleOsc.reset (new juce::ToggleButton ("toggleOsc"));
    addAndMakeVisible (toggleOsc.get());
    toggleOsc->setButtonText (TRANS("Receive OSC messages"));
    toggleOsc->addListener (this);

    buttonSpeakerTest.reset (new juce::TextButton ("buttonSpeakerTest"));
    addAndMakeVisible (buttonSpeakerTest.get());
    buttonSpeakerTest->setButtonText (TRANS("test all speakers"));
    buttonSpeakerTest->addListener (this);

    labelDevelopmentVersion.reset (new juce::Label ("labelDevelopmentVersion",
                                                    TRANS("Unofficial Pre-Release")));
    addAndMakeVisible (labelDevelopmentVersion.get());
    labelDevelopmentVersion->setFont (juce::Font (25.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDevelopmentVersion->setJustificationType (juce::Justification::centred);
    labelDevelopmentVersion->setEditable (false, false, false);
    labelDevelopmentVersion->setColour (juce::Label::backgroundColourId, juce::Colour (0xbded0d0d));
    labelDevelopmentVersion->setColour (juce::Label::textColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::Label::outlineColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDevelopmentVersion->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonManage.reset (new juce::TextButton ("buttonManage"));
    addAndMakeVisible (buttonManage.get());
    buttonManage->setButtonText (TRANS("manage..."));
    buttonManage->addListener (this);

    comboBoxChannelWeightingMode.reset (new juce::ComboBox ("comboBoxChannelWeightingMode"));
    addAndMakeVisible (comboBoxChannelWeightingMode.get());
    comboBoxChannelWeightingMode->setEditableText (false);
    comboBoxChannelWeightingMode->setJustificationType (juce::Justification::centredLeft);
    comboBoxChannelWeightingMode->setTextWhenNothingSelected (juce::String());
    comboBoxChannelWeightingMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxChannelWeightingMode->addListener (this);

    buttonManageFilters.reset (new juce::TextButton ("buttonManageFilters"));
    addAndMakeVisible (buttonManageFilters.get());
    buttonManageFilters->setButtonText (TRANS("manage filters..."));
    buttonManageFilters->addListener (this);

    comboBoxApply.reset (new juce::ComboBox ("comboBoxApply"));
    addAndMakeVisible (comboBoxApply.get());
    comboBoxApply->setEditableText (false);
    comboBoxApply->setJustificationType (juce::Justification::centredLeft);
    comboBoxApply->setTextWhenNothingSelected (TRANS("apply..."));
    comboBoxApply->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxApply->addListener (this);

    buttonCsv.reset (new juce::TextButton ("buttonCsv"));
    addAndMakeVisible (buttonCsv.get());
    buttonCsv->setButtonText (TRANS("csv"));
    buttonCsv->addListener (this);

    buttonScaling.reset (new juce::TextButton ("buttonScaling"));
    addAndMakeVisible (buttonScaling.get());
    buttonScaling->setButtonText (TRANS("scaling"));
    buttonScaling->addListener (this);

    sliderPort.reset (new juce::Slider ("sliderPort"));
    addAndMakeVisible (sliderPort.get());
    sliderPort->setRange (0, 65535, 1);
    sliderPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderPort->addListener (this);

    sliderTimeout.reset (new juce::Slider ("sliderTimeout"));
    addAndMakeVisible (sliderTimeout.get());
    sliderTimeout->setRange (10, 10000, 1);
    sliderTimeout->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderTimeout->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 70, 20);
    sliderTimeout->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (900, 700);


    //[Constructor] You can add your own custom stuff here..
    labelDevelopmentVersion->setVisible(Constants::isDevelopmentVersion());

    // prepare weighting comboBox
    comboBoxChannelWeightingMode->addItem("Basic", AmbiSettings::BASIC);
    comboBoxChannelWeightingMode->addItem("In-Phase", AmbiSettings::INPHASE);
    comboBoxChannelWeightingMode->addItem("Max-rE", AmbiSettings::MAXRE);
    comboBoxChannelWeightingMode->addItem("Manual", AmbiSettings::MANUAL);

    // speaker list elements
	buttonSpeakerTest->setClickingTogglesState(true);
	buttonSpeakerTest->setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkred);
	speakerList->setModel(this);
	speakerList->getHeader().addColumn("CH", COLUMN_ID_NB, 30);
	speakerList->getHeader().addColumn("Name", COLUMN_ID_NAME, 100);
	speakerList->getHeader().addColumn("X", COLUMN_ID_X, 50);
	speakerList->getHeader().addColumn("Y", COLUMN_ID_Y, 50);
	speakerList->getHeader().addColumn("Z", COLUMN_ID_Z, 50);
	speakerList->getHeader().addColumn("A", COLUMN_ID_A, 60);
	speakerList->getHeader().addColumn("E", COLUMN_ID_E, 60);
	speakerList->getHeader().addColumn("D [m]", COLUMN_ID_DISTANCE, 60);
	speakerList->getHeader().addColumn("Delay [ms]", COLUMN_ID_DELAY, 80);
	speakerList->getHeader().addColumn("Delay comp. [ms]", COLUMN_ID_DELAY_COMPENSATION, 100);
	speakerList->getHeader().addColumn("Color", COLUMN_ID_COLOR, 30);
    speakerList->getHeader().addColumn("Mute", COLUMN_ID_MUTE, 30);
	speakerList->getHeader().addColumn("Gain [dB]", COLUMN_ID_GAIN, 80);
	speakerList->getHeader().addColumn("Test", COLUMN_ID_TEST, 30);
    speakerList->getHeader().addColumn("Filter", COLUMN_ID_FILTER, 40);
	speakerList->getHeader().setStretchToFitActive(true);
	speakerList->getHeader().resizeAllColumnsToFit(speakerList->getWidth());
    speakerList->setMultipleSelectionEnabled(true);
	updateComboBox();
	pPointSelection->addChangeListener(this);
    updateUI();

    btnEditMode->setToggleState(pDecoderSettings->editMode, dontSendNotification);

	// OSC
	toggleOsc->setToggleState(pDecoderSettings->oscReceive, dontSendNotification);
	sliderPort->setValue(pDecoderSettings->oscReceivePort);
	sliderTimeout->setValue(pDecoderSettings->oscReceiveTimeoutMs);

	controlDimming();
    //[/Constructor]
}

SpeakerSettingsComponent::~SpeakerSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pTestSoundGenerator->reset();
	pPointSelection->removeChangeListener(this);
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    groupOsc = nullptr;
    groupAmbisonics = nullptr;
    groupSpeakers = nullptr;
    comboBoxChannelConfig = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    speakerList = nullptr;
    buttonAdd = nullptr;
    buttonRemove = nullptr;
    buttonMoveDown = nullptr;
    buttonMoveUp = nullptr;
    ambiChannelControl = nullptr;
    labelChannelWeights = nullptr;
    btnEditMode = nullptr;
    labelOscPort = nullptr;
    labelTimeout = nullptr;
    toggleOsc = nullptr;
    buttonSpeakerTest = nullptr;
    labelDevelopmentVersion = nullptr;
    buttonManage = nullptr;
    comboBoxChannelWeightingMode = nullptr;
    buttonManageFilters = nullptr;
    comboBoxApply = nullptr;
    buttonCsv = nullptr;
    buttonScaling = nullptr;
    sliderPort = nullptr;
    sliderTimeout = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SpeakerSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpeakerSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupOsc->setBounds ((8 + 0) + 0, (0 + (getHeight() - 267)) + 199, ((getWidth() - 16) - 0) - 0, 60);
    groupAmbisonics->setBounds (8 + 0, 0 + (getHeight() - 267), (getWidth() - 16) - 0, 199);
    groupSpeakers->setBounds (8, 0, getWidth() - 16, getHeight() - 267);
    comboBoxChannelConfig->setBounds (8 + 192, 0 + 24, getWidth() - 531, 24);
    labelPresets->setBounds ((8 + 192) + -8 - 64, 0 + 24, 64, 24);
    buttonSave->setBounds (8 + (getWidth() - 16) - 224 - 80, 0 + 24, 80, 24);
    speakerList->setBounds (8 + 8, 0 + 56, (getWidth() - 16) - 16, (getHeight() - 267) - 96);
    buttonAdd->setBounds ((8 + 8) + 0, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    buttonRemove->setBounds ((8 + 8) + 72, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    buttonMoveDown->setBounds ((8 + 8) + ((getWidth() - 16) - 16) - 64, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    buttonMoveUp->setBounds ((8 + 8) + ((getWidth() - 16) - 16) - 136, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    ambiChannelControl->setBounds ((8 + 0) + 8, (0 + (getHeight() - 267)) + 52, ((getWidth() - 16) - 0) - 16, 199 - 60);
    labelChannelWeights->setBounds ((8 + 0) + 8, (0 + (getHeight() - 267)) + 20, 112, 24);
    btnEditMode->setBounds (8 + 8, 0 + 24, 150, 24);
    labelOscPort->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 398 - 75, ((0 + (getHeight() - 267)) + 199) + 20, 75, 24);
    labelTimeout->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 186 - 96, ((0 + (getHeight() - 267)) + 199) + 20, 96, 24);
    toggleOsc->setBounds (((8 + 0) + 0) + 8, ((0 + (getHeight() - 267)) + 199) + 20, 180, 24);
    buttonSpeakerTest->setBounds (proportionOfWidth (0.4978f) - (120 / 2), (0 + 56) + ((getHeight() - 267) - 96) - -8, 120, 24);
    labelDevelopmentVersion->setBounds (proportionOfWidth (0.5000f) - (proportionOfWidth (0.3986f) / 2), 0, proportionOfWidth (0.3986f), 24);
    buttonManage->setBounds (8 + (getWidth() - 16) - 136 - 80, 0 + 24, 80, 24);
    comboBoxChannelWeightingMode->setBounds ((8 + 0) + 128, (0 + (getHeight() - 267)) + 20, 120, 24);
    buttonManageFilters->setBounds (8 + (getWidth() - 16) - 8 - 120, 0 + 24, 120, 24);
    comboBoxApply->setBounds ((proportionOfWidth (0.4978f) - (120 / 2)) + 120 - -8, ((0 + 56) + ((getHeight() - 267) - 96) - -8) + 0, 144, 24);
    buttonCsv->setBounds ((proportionOfWidth (0.4978f) - (120 / 2)) + -8 - 64, ((0 + 56) + ((getHeight() - 267) - 96) - -8) + 0, 64, 24);
    buttonScaling->setBounds ((proportionOfWidth (0.4978f) - (120 / 2)) + -80 - 64, ((0 + 56) + ((getHeight() - 267) - 96) - -8) + 0, 64, 24);
    sliderPort->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 290 - 100, ((0 + (getHeight() - 267)) + 199) + 20, 100, 24);
    sliderTimeout->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 8 - 170, ((0 + (getHeight() - 267)) + 199) + 20, 170, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SpeakerSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxChannelConfig.get())
    {
        //[UserComboBoxCode_comboBoxChannelConfig] -- add your combo box handling code here..
		String presetName = comboBoxChannelConfig->getText();
        pPresetHelper->selectPresetName(presetName);

        //[/UserComboBoxCode_comboBoxChannelConfig]
    }
    else if (comboBoxThatHasChanged == comboBoxChannelWeightingMode.get())
    {
        //[UserComboBoxCode_comboBoxChannelWeightingMode] -- add your combo box handling code here..
        pAmbiSettings->setWeightMode(AmbiSettings::AmbiWeightMode( comboBoxChannelWeightingMode->getSelectedId()));
        ambiChannelControl->updateValues();
        controlDimming();
        //[/UserComboBoxCode_comboBoxChannelWeightingMode]
    }
    else if (comboBoxThatHasChanged == comboBoxApply.get())
    {
        //[UserComboBoxCode_comboBoxApply] -- add your combo box handling code here..
        int mainSelection = pPointSelection->getMainSelectedPointIndex();
        if (mainSelection >= 0 && mainSelection < pSpeakerSet->size())
        {

            if (comboBoxApply->getSelectedId() == ID_APPLY_COLOR)
            {
                Colour color = pSpeakerSet->get(mainSelection)->getColor();
                auto selection = pPointSelection->getSelectedIndices();
                for (int i = 0; i < selection.size(); i++)
                {
                    if (selection[i] != mainSelection)
                    {
                        pSpeakerSet->get(selection[i])->setColor(color);
                    }
                }
            }
            else if (comboBoxApply->getSelectedId() == ID_APPLY_FILTER)
            {
                FilterBankInfo* info = pSpeakerSet->get(mainSelection)->getFilterInfo();
                bool bypass = pSpeakerSet->get(mainSelection)->getFilterBypass();
                auto selection = pPointSelection->getSelectedIndices();
                for (int i = 0; i < selection.size(); i++)
                {
                    if (selection[i] != mainSelection)
                    {
                        pSpeakerSet->get(selection[i])->getFilterInfo()->copyFrom(info);
                        pSpeakerSet->get(selection[i])->setFilterBypass(bypass);
                    }
                }
            }
            else if (comboBoxApply->getSelectedId() == ID_APPLY_GAIN)
            {
                double gainFactor = pSpeakerSet->get(mainSelection)->getGain();
                auto selection = pPointSelection->getSelectedIndices();
                for (int i = 0; i < selection.size(); i++)
                {
                    if (selection[i] != mainSelection)
                    {
                        pSpeakerSet->get(selection[i])->setGain(gainFactor, true);
                    }
                }
            }
            else if(comboBoxApply->getSelectedId() == ID_APPLY_DEC_GAIN || comboBoxApply->getSelectedId() == ID_APPLY_INC_GAIN)
            {
                float gainFactor = float(Decibels::decibelsToGain(comboBoxApply->getSelectedId() == ID_APPLY_INC_GAIN ? 0.5 : -0.5));
                auto selection = pPointSelection->getSelectedIndices();
                for (auto index : selection)
                {
                    pSpeakerSet->get(index)->setGain(pSpeakerSet->get(index)->getGain() * gainFactor, true);
                }
            }
        }

        speakerList->updateContent();
        speakerList->repaint();
        comboBoxApply->setSelectedId(0);
        //[/UserComboBoxCode_comboBoxApply]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SpeakerSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
                return;

        pPresetHelper->writeToXmlFile(*newFile, pSpeakerSet, pAmbiSettings);
        comboBoxChannelConfig->setText("", dontSendNotification);
        delete newFile;
        //[/UserButtonCode_buttonSave]
    }
    else if (buttonThatWasClicked == buttonAdd.get())
    {
        //[UserButtonCode_buttonAdd] -- add your button handler code here..
		Uuid newId = Uuid();
		pSpeakerSet->add(new AmbiSpeaker(newId.toString(), Point3D<double>(0.0, 0.0, 0.0), "new", TrackColors::getSpeakerColor()));
		pPointSelection->selectPoint(pSpeakerSet->size() - 1);
		speakerList->updateContent();
		speakerList->repaint();
        //[/UserButtonCode_buttonAdd]
    }
    else if (buttonThatWasClicked == buttonRemove.get())
    {
        //[UserButtonCode_buttonRemove] -- add your button handler code here..
        auto selection = pPointSelection->getSelectedIndices();
        if(!pPointSelection->getSelectedIndices().isEmpty())
        {
        	pPointSelection->unselectPoint();
            selection.sort();
            for (int i = selection.size() - 1; i >= 0; i--)
            {
                pSpeakerSet->remove(selection[i]);
            }

			speakerList->updateContent();
			speakerList->repaint();
		}
        //[/UserButtonCode_buttonRemove]
    }
    else if (buttonThatWasClicked == buttonMoveDown.get())
    {
        //[UserButtonCode_buttonMoveDown] -- add your button handler code here..
		int selection = pPointSelection->getMainSelectedPointIndex();
		if(selection >= 0 && selection < pSpeakerSet->size() - 1)
		{
			pPointSelection->unselectPoint();
			pSpeakerSet->swap(selection, selection + 1);
			pPointSelection->selectPoint(selection + 1);
		}
        //[/UserButtonCode_buttonMoveDown]
    }
    else if (buttonThatWasClicked == buttonMoveUp.get())
    {
        //[UserButtonCode_buttonMoveUp] -- add your button handler code here..
		int selection = pPointSelection->getMainSelectedPointIndex();
		if (selection >= 1 && selection < pSpeakerSet->size())
		{
			pPointSelection->unselectPoint();
			pSpeakerSet->swap(selection, selection - 1);
			pPointSelection->selectPoint(selection - 1);
		}
        //[/UserButtonCode_buttonMoveUp]
    }
    else if (buttonThatWasClicked == btnEditMode.get())
    {
        //[UserButtonCode_btnEditMode] -- add your button handler code here..
		pDecoderSettings->editMode = btnEditMode->getToggleState();
		controlDimming();
		sendChangeMessage();
        //[/UserButtonCode_btnEditMode]
    }
    else if (buttonThatWasClicked == toggleOsc.get())
    {
        //[UserButtonCode_toggleOsc] -- add your button handler code here..
		pDecoderSettings->oscReceive = toggleOsc->getToggleState();
		sendChangeMessage();
        //[/UserButtonCode_toggleOsc]
    }
    else if (buttonThatWasClicked == buttonSpeakerTest.get())
    {
        //[UserButtonCode_buttonSpeakerTest] -- add your button handler code here..
		pTestSoundGenerator->toggleAutoTest();
        //[/UserButtonCode_buttonSpeakerTest]
    }
    else if (buttonThatWasClicked == buttonManage.get())
    {
        //[UserButtonCode_buttonManage] -- add your button handler code here..
        presetManagerDialog.show(this, pPresetHelper);
        //[/UserButtonCode_buttonManage]
    }
    else if (buttonThatWasClicked == buttonManageFilters.get())
    {
        //[UserButtonCode_buttonManageFilters] -- add your button handler code here..
        presetManagerDialog.show(this, filterPresetHelper.get(), false);
        //[/UserButtonCode_buttonManageFilters]
    }
    else if (buttonThatWasClicked == buttonCsv.get())
    {
        //[UserButtonCode_buttonCsv] -- add your button handler code here..
        PopupMenu m;
        m.addItem(1, "Import from CSV");
        m.addItem(2, "Export to CSV");
        m.addItem(3, "CSV Help");
        const int result = m.show();
        if (result == 1)
        {
            if (CsvImportExport::importFromCsv(pSpeakerSet))
            {
                sendChangeMessage();
                controlDimming();
                speakerList->updateContent();
                speakerList->repaint();
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "CSV Import", "Data import successful");
            }
            else
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "CSV Import", "Data import failed, please refer to file specification...");
            }
        }
        else if (result == 2)
        {
            if(CsvImportExport::exportToCsv(pSpeakerSet))
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "CSV Export", "Export completed successfully");
            }
            else
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "CSV Export", "Export failed!");
            }
        }
        else if (result == 3)
        {
            std::unique_ptr<Label> label = std::make_unique<Label>();
            label->setSize(600, 140);
            label->setText("Required CSV format:\nAzimuth[Degrees];Elevation[Degrees];Distance[m];{Name};{Color[ARGB Hex]};{Gain[dB]}\nParameters in {} are optional, semicolons can be omitted after last used parameter.\nIf parameter 'Name' is not specified, the channel number will be used as name.\n\nExample:\n315;0;10\n45;0;9.5\n10;-5;4;Subwoofer;ffff0000;-6", dontSendNotification);
            label->setJustificationType(Justification::left);
            CallOutBox::launchAsynchronously(std::move(label), buttonCsv->getBounds(), this);
        }
        //[/UserButtonCode_buttonCsv]
    }
    else if (buttonThatWasClicked == buttonScaling.get())
    {
        //[UserButtonCode_buttonScaling] -- add your button handler code here..
        CallOutBox::launchAsynchronously(std::make_unique<ScalingComponent>(this, pSpeakerSet, pZoomSettings), buttonScaling->getBounds(), this);
        //[/UserButtonCode_buttonScaling]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SpeakerSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderPort.get())
    {
        //[UserSliderCode_sliderPort] -- add your slider handling code here..
        pDecoderSettings->oscReceivePort = (int)sliderPort->getValue();
        sendChangeMessage();
        //[/UserSliderCode_sliderPort]
    }
    else if (sliderThatWasMoved == sliderTimeout.get())
    {
        //[UserSliderCode_sliderTimeout] -- add your slider handling code here..
        pDecoderSettings->oscReceiveTimeoutMs = (int)sliderTimeout->getValue();
        sendChangeMessage();
        //[/UserSliderCode_sliderTimeout]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
int SpeakerSettingsComponent::getNumRows()
{
	return pSpeakerSet->size();
}

void SpeakerSettingsComponent::selectedRowsChanged(int lastRowSelected)
{
    pPointSelection->unselectPoint();

    bool first = true;
    if (lastRowSelected >= 0 && lastRowSelected < pSpeakerSet->size())
    {
        auto set = speakerList->getSelectedRows();
        for (auto r : set.getRanges())
        {
            for (int i = r.getStart(); i < r.getEnd(); i++)
            {
                if (i != lastRowSelected)
                {
                    pPointSelection->selectPoint(i, !first);
                    first = false;
                }
            }
        }

        pPointSelection->selectPoint(lastRowSelected, !first);
        controlDimming();
    }
}

void SpeakerSettingsComponent::paintRowBackground(Graphics& g, int rowNumber, int, int, bool rowIsSelected)
{
	if (rowIsSelected)
    {
        Colour baseColor = COLOR_DEFINITION_SELECTED_ROW;
        if (pPointSelection->getMainSelectedPointIndex() == rowNumber)
            g.fillAll(baseColor);
        else
            g.fillAll(baseColor.withAlpha(0.4f));
    }
    else if (rowNumber % 2)
    {
        const Colour alternateColour(getLookAndFeel().findColour(ListBox::backgroundColourId)
            .interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), COLOR_DEFINITION_ALTERNATE_INTENSITY));

        g.fillAll(alternateColour);
    }
}

void SpeakerSettingsComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool)
{
	AmbiPoint* pt = pSpeakerSet->get(rowNumber);
	if (pt == nullptr)
		return;

	g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
	String text;
	switch(columnId)
	{
	case COLUMN_ID_NB: text = String(rowNumber + 1); break;
	case COLUMN_ID_NAME: text = pt->getName(); break;
	case COLUMN_ID_DISTANCE: text = String(pt->getRawPoint()->getDistance(), 2); break;
	case COLUMN_ID_DELAY: text = String(delayHelper.getTotalDelayMs(pt->getRawPoint()->getDistance()), 2); break;
	case COLUMN_ID_DELAY_COMPENSATION: text = String(delayHelper.getDelayCompensationMs(pt->getRawPoint()->getDistance(), pSpeakerSet->getMaxNormalizedDistance()), 2); break;
	default: text = "";
	}
	g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

Component* SpeakerSettingsComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate)
{
	if(columnId == COLUMN_ID_X
		|| columnId == COLUMN_ID_Y
		|| columnId == COLUMN_ID_Z
		|| columnId == COLUMN_ID_A
		|| columnId == COLUMN_ID_E
		|| columnId == COLUMN_ID_DISTANCE)
	{
		NumericColumnCustomComponent* numericBox = static_cast<NumericColumnCustomComponent*> (existingComponentToUpdate);
		if (numericBox == nullptr)
			numericBox = new NumericColumnCustomComponent(*this);

		numericBox->setRowAndColumn(rowNumber, columnId);
		return numericBox;
	}
	else if(columnId == COLUMN_ID_GAIN)
	{
		SliderColumnCustomComponent* gainBox = static_cast<SliderColumnCustomComponent*> (existingComponentToUpdate);
		if (gainBox == nullptr)
			gainBox = new SliderColumnCustomComponent(*this);

		gainBox->setRowAndColumn(rowNumber, columnId);
		return gainBox;
	}
	else if(columnId == COLUMN_ID_TEST)
	{
		SpeakerTestCustomComponent* button = static_cast<SpeakerTestCustomComponent*> (existingComponentToUpdate);
		if (button == nullptr)
			button = new SpeakerTestCustomComponent(*this);

		button->setRowAndColumn(rowNumber, columnId);
		return button;
	}
	else if(columnId == COLUMN_ID_NAME)
	{
		EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
		if (textLabel == nullptr)
			textLabel = new EditableTextCustomComponent(*this);

		textLabel->setRowAndColumn(rowNumber, columnId);
		return textLabel;
	}
    else if(columnId == COLUMN_ID_FILTER)
    {
        CheckBoxFilterCustomComponent* checkBox = static_cast<CheckBoxFilterCustomComponent*> (existingComponentToUpdate);
        if(checkBox == nullptr)
            checkBox = new CheckBoxFilterCustomComponent(*this);

        checkBox->setRowAndColumn(rowNumber, columnId);
        return checkBox;
    }
    else if(columnId == COLUMN_ID_MUTE)
    {
        CheckBoxCustomComponent* checkBox = static_cast<CheckBoxCustomComponent*> (existingComponentToUpdate);
        if(checkBox == nullptr)
            checkBox = new CheckBoxCustomComponent(*this);

        checkBox->setRowAndColumn(rowNumber, columnId);
        return checkBox;
    }
    else if(columnId == COLUMN_ID_COLOR)
    {
        ColorEditorCustomComponent* colorBox = static_cast<ColorEditorCustomComponent*> (existingComponentToUpdate);
        if (colorBox == nullptr)
            colorBox = new ColorEditorCustomComponent(*this);

        colorBox->setRowAndColumn(rowNumber, columnId);
        return colorBox;
    }
	return nullptr;
}

String SpeakerSettingsComponent::getTableText(const int columnId, const int rowNumber)
{
	AmbiPoint* pt = pSpeakerSet->get(rowNumber);
	if (pt == nullptr)
		return "";

	switch(columnId)
	{
	case COLUMN_ID_NAME: return pt->getName();
	default: return "";
	}
}

void SpeakerSettingsComponent::setTableText(const int columnId, const int rowNumber, const String& newText)
{
	switch(columnId)
	{
	case COLUMN_ID_NAME: pSpeakerSet->setChannelName(rowNumber, newText); break;
	default: throw;
	}
}

void SpeakerSettingsComponent::setValue(int columnId, int rowNumber, double newValue)
{
	switch (columnId)
	{
    case COLUMN_ID_GAIN: pSpeakerSet->setGain(rowNumber, Decibels::decibelsToGain(newValue)); break;
	case COLUMN_ID_X: pSpeakerSet->setX(rowNumber, newValue); break;
	case COLUMN_ID_Y: pSpeakerSet->setY(rowNumber, newValue); break;
	case COLUMN_ID_Z: pSpeakerSet->setZ(rowNumber, newValue); break;
	case COLUMN_ID_A: pSpeakerSet->setAzimuth(rowNumber, Constants::GradToRad(newValue)); break;
	case COLUMN_ID_E: pSpeakerSet->setElevation(rowNumber, Constants::GradToRad(newValue)); break;
	case COLUMN_ID_DISTANCE: pSpeakerSet->setDistance(rowNumber, newValue); break;
    case COLUMN_ID_COLOR: pSpeakerSet->setChannelColor(rowNumber, Colour(uint32(newValue))); break;
    case COLUMN_ID_MUTE: pSpeakerSet->setMute(rowNumber, newValue == 1); break;
            
        default: return; // do nothing
	}

	speakerList->updateContent();
	speakerList->repaint();
}

double SpeakerSettingsComponent::getValue(int columnId, int rowNumber)
{
	AmbiPoint* pt = pSpeakerSet->get(rowNumber);
	if (pt == nullptr)
		return 0.0;

	switch (columnId)
	{
    case COLUMN_ID_GAIN: return Decibels::gainToDecibels(pt->getGain());
	case COLUMN_ID_X: return pt->getRawPoint()->getX();
	case COLUMN_ID_Y: return pt->getRawPoint()->getY();
	case COLUMN_ID_Z: return pt->getRawPoint()->getZ();
	case COLUMN_ID_A: return Constants::RadToGrad(pt->getRawPoint()->getAzimuth());
	case COLUMN_ID_E: return Constants::RadToGrad(pt->getRawPoint()->getElevation());
	case COLUMN_ID_DISTANCE: return pt->getRawPoint()->getDistance();
    case COLUMN_ID_COLOR: return pt->getColor().getARGB();
        case COLUMN_ID_MUTE: return pt->getMute() ? 1 : 0;
        default: return 0.0;
	}
}

void SpeakerSettingsComponent::setFlag(int columnId, int rowNumber, bool newValue) const
{
    switch (columnId)
    {
    case COLUMN_ID_FILTER:
        pSpeakerSet->setFilterBypass(rowNumber, !newValue);
    	break;
            
    default: throw;
    }

    speakerList->updateContent();
    speakerList->repaint();
}

bool SpeakerSettingsComponent::getFlag(int columnId, int rowNumber) const
{
    AmbiSpeaker* pt = pSpeakerSet->get(rowNumber);
    if (pt == nullptr)
        return false;

    switch (columnId)
    {
    case COLUMN_ID_FILTER: return !pt->getFilterBypass();
    default: return false;
    }
}

void SpeakerSettingsComponent::speakerTest(int rowNumber) const
{
	pTestSoundGenerator->toggle(rowNumber);
}

TableListBox* SpeakerSettingsComponent::getTable()
{
	return speakerList.get();
}

SliderRange SpeakerSettingsComponent::getSliderRange(int columnId)
{
	switch(columnId)
	{
	case COLUMN_ID_X:
	case COLUMN_ID_Y:
	case COLUMN_ID_Z:
		return SliderRange(-10000.0, 10000.0, 0.001);

	case COLUMN_ID_A:
		return SliderRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);

	case COLUMN_ID_E:
		return SliderRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);

	case COLUMN_ID_GAIN:
		return SliderRange(Constants::GainDbMin, Constants::GainDbMax, 0.5);

	case COLUMN_ID_DISTANCE:
		return SliderRange(0.0, 10000.0 * MathConstants<double>::sqrt2, 0.001);
	}

	return SliderRange(0.0, 1.0, 0.001);
}

bool SpeakerSettingsComponent::getEnabled(const int /*columnId*/, const int /*rowNumber*/)
{
    return true;
}

FilterBankInfo* SpeakerSettingsComponent::getFilterInfo(int rowNumber) const
{
	AmbiSpeaker* pt = pSpeakerSet->get(rowNumber);
	if (pt == nullptr)
		return nullptr;

	return pt->getFilterInfo();
}

dsp::ProcessSpec* SpeakerSettingsComponent::getFilterSpecification() const
{
	return pFilterSpecification;
}

FilterPresetHelper* SpeakerSettingsComponent::getFilterPresetHelper() const
{
    return filterPresetHelper.get();
}

void SpeakerSettingsComponent::updateUI() const
{
	speakerList->updateContent();
	speakerList->repaint();

	ambiChannelControl->updateValues();
    comboBoxChannelWeightingMode->setSelectedId(pAmbiSettings->getWeightMode());
}

void SpeakerSettingsComponent::updateComboBox() const
{
	comboBoxChannelConfig->clear();
	int i = 1;
	for (File file : pPresetHelper->presetFiles)
	{
		comboBoxChannelConfig->addItem(file.getFileNameWithoutExtension(), i++);
	}
}

void SpeakerSettingsComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	speakerList->updateContent();
	speakerList->repaint();

	if (source == pPointSelection)
	{
        SparseSet<int> sel;
        for (int index : pPointSelection->getSelectedIndices())
        {
            sel.addRange(Range<int>(index, index + 1));
        }

        speakerList->setSelectedRows(sel, dontSendNotification);
        controlDimming();
	}
}

void SpeakerSettingsComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        updateComboBox();
    }
    else if(message == ACTION_MESSAGE_PRESET_CHANGED)
    {
        if(pAmbiSettings->getWarningFlag())
        {
            const MessageManagerLock lock;
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Inconsistent Ambisonic Order", "This preset was saved using a different order plugin. Ambisonic channel weighting may have to be adjusted.");
        }

        updateUI();
        controlDimming();
        sendChangeMessage();
    }
}

void SpeakerSettingsComponent::controlDimming()
{
	bool en = pDecoderSettings->editMode;
    bool multiselection = pPointSelection->getSelectedIndices().size() > 1;

	groupSpeakers->setEnabled(en);
	labelPresets->setEnabled(en);
	comboBoxChannelConfig->setEnabled(en);
	speakerList->setEnabled(en);
    buttonCsv->setEnabled(en);
    buttonScaling->setEnabled(en);
	buttonManage->setEnabled(en);
	buttonSave->setEnabled(en);
	buttonAdd->setEnabled(en);
	buttonRemove->setEnabled(en);
	buttonMoveUp->setEnabled(en && !multiselection);
	buttonMoveDown->setEnabled(en && !multiselection);
	buttonSpeakerTest->setEnabled(en);

    ambiChannelControl->setEnabled(pAmbiSettings->getWeightMode() == AmbiSettings::MANUAL);

    comboBoxApply->setVisible(multiselection);
    if(multiselection)
    {
        comboBoxApply->clear(dontSendNotification);

        int mainIndex = pPointSelection->getMainSelectedPointIndex();
        if(mainIndex >= 0 && mainIndex < pSpeakerSet->size())
        {
            comboBoxApply->addItem("Color from CH " + String(mainIndex + 1), ID_APPLY_COLOR);
            comboBoxApply->addItem("Filter from CH " + String(mainIndex + 1), ID_APPLY_FILTER);
            comboBoxApply->addItem("Gain from CH " + String(mainIndex + 1), ID_APPLY_GAIN);

            comboBoxApply->addItem(TRANS("Gain -0.5 dB"), ID_APPLY_DEC_GAIN);
            comboBoxApply->addItem(TRANS("Gain +0.5 dB"), ID_APPLY_INC_GAIN);
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

<JUCER_COMPONENT documentType="Component" className="SpeakerSettingsComponent"
                 componentName="" parentClasses="public Component, public TableListBoxModel, public ChangeListener, public ActionBroadcaster, public ChangeBroadcaster, public TableColumnCallback, ActionListener"
                 constructorParams="AmbiSpeakerSet* pSpeakerSet, DecoderPresetHelper* pPresetHelper, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, DecoderSettings* pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* pFilterSpecification, ZoomSettings* pZoomSettings"
                 variableInitialisers="pSpeakerSet(pSpeakerSet), pPresetHelper(pPresetHelper), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings),pDecoderSettings(pDecoderSettings), pFilterSpecification(pFilterSpecification), pZoomSettings(pZoomSettings)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="900" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupOsc" id="f4cf3a53a6ef0d87" memberName="groupOsc" virtualName=""
                  explicitFocusOrder="0" pos="0 0R 0M 60" posRelativeX="17eb4b418501687a"
                  posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                  title="OSC"/>
  <GROUPCOMPONENT name="groupAmbisonics" id="17eb4b418501687a" memberName="groupAmbisonics"
                  virtualName="" explicitFocusOrder="0" pos="0 0R 0M 199" posRelativeX="450188aa0f332e78"
                  posRelativeY="450188aa0f332e78" posRelativeW="450188aa0f332e78"
                  title="Ambisonics"/>
  <GROUPCOMPONENT name="groupSpeakers" id="450188aa0f332e78" memberName="groupSpeakers"
                  virtualName="" explicitFocusOrder="0" pos="8 0 16M 267M" title="Speakers"/>
  <COMBOBOX name="channelConfig" id="4b25adf5b07e9492" memberName="comboBoxChannelConfig"
            virtualName="" explicitFocusOrder="0" pos="192 24 531M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="-8r 24 64 24" posRelativeX="4b25adf5b07e9492"
         posRelativeY="450188aa0f332e78" edTextCol="ff000000" edBkgCol="0"
         labelText="Presets:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="224Rr 24 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="speakerList" id="34ae3e87c64e62da" memberName="speakerList"
                    virtualName="" explicitFocusOrder="0" pos="8 56 16M 96M" posRelativeX="450188aa0f332e78"
                    posRelativeY="450188aa0f332e78" posRelativeW="450188aa0f332e78"
                    posRelativeH="450188aa0f332e78" class="TableListBox" params=""/>
  <TEXTBUTTON name="buttonAdd" id="e1290b9a1a32d249" memberName="buttonAdd"
              virtualName="" explicitFocusOrder="0" pos="0 -8R 64 24" posRelativeX="34ae3e87c64e62da"
              posRelativeY="34ae3e87c64e62da" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonAdd" id="49c8de1156e72d8c" memberName="buttonRemove"
              virtualName="" explicitFocusOrder="0" pos="72 -8R 64 24" posRelativeX="34ae3e87c64e62da"
              posRelativeY="34ae3e87c64e62da" buttonText="remove" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveDown" id="7291297cb3544d01" memberName="buttonMoveDown"
              virtualName="" explicitFocusOrder="0" pos="64R -8R 64 24" posRelativeX="34ae3e87c64e62da"
              posRelativeY="34ae3e87c64e62da" buttonText="down" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveUp" id="e2d399b90fa42e97" memberName="buttonMoveUp"
              virtualName="" explicitFocusOrder="0" pos="136R -8R 64 24" posRelativeX="34ae3e87c64e62da"
              posRelativeY="34ae3e87c64e62da" buttonText="up" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="ambiChannelControl" id="4ec5a32a175ea48d" memberName="ambiChannelControl"
                    virtualName="" explicitFocusOrder="0" pos="8 52 16M 60M" posRelativeX="17eb4b418501687a"
                    posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                    posRelativeH="17eb4b418501687a" class="MultiSliderControl" params="CURRENT_AMBISONICS_ORDER_NB_OF_GAINS, pAmbiSettings-&gt;getAmbiOrderWeightPointer(), &amp;ambiChannelNames, 0.0, 1.5, 0.00001"/>
  <LABEL name="labelChannelWeights" id="ce2f83213d847908" memberName="labelChannelWeights"
         virtualName="" explicitFocusOrder="0" pos="8 20 112 24" posRelativeX="17eb4b418501687a"
         posRelativeY="17eb4b418501687a" edTextCol="ff000000" edBkgCol="0"
         labelText="Channel weights" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnEditMode" id="c11c14f07c9141ac" memberName="btnEditMode"
                virtualName="" explicitFocusOrder="0" pos="8 24 150 24" posRelativeX="450188aa0f332e78"
                posRelativeY="450188aa0f332e78" buttonText="Edit mode" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="398Rr 20 75 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="OSC-Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelTimeout" id="628a0500d66bc466" memberName="labelTimeout"
         virtualName="" explicitFocusOrder="0" pos="186Rr 20 96 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Timeout [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="toggleOsc" id="1b103b47888e742b" memberName="toggleOsc"
                virtualName="" explicitFocusOrder="0" pos="8 20 180 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Receive OSC messages"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="buttonSpeakerTest" id="5fad387b688247bf" memberName="buttonSpeakerTest"
              virtualName="" explicitFocusOrder="0" pos="49.784%c -8R 120 24"
              posRelativeY="34ae3e87c64e62da" buttonText="test all speakers"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelDevelopmentVersion" id="c41821090201078b" memberName="labelDevelopmentVersion"
         virtualName="" explicitFocusOrder="0" pos="50%c 0 39.856% 24"
         bkgCol="bded0d0d" textCol="ffffff00" outlineCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="Unofficial Pre-Release" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="25.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="buttonManage" id="a4621bea805565b3" memberName="buttonManage"
              virtualName="" explicitFocusOrder="0" pos="136Rr 24 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="comboBoxChannelWeightingMode" id="e9f5f23a259dd1c0" memberName="comboBoxChannelWeightingMode"
            virtualName="" explicitFocusOrder="0" pos="128 20 120 24" posRelativeX="17eb4b418501687a"
            posRelativeY="17eb4b418501687a" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="buttonManageFilters" id="5b471faa99c7496b" memberName="buttonManageFilters"
              virtualName="" explicitFocusOrder="0" pos="8Rr 24 120 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage filters..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="comboBoxApply" id="30951c9518bbedeb" memberName="comboBoxApply"
            virtualName="" explicitFocusOrder="0" pos="-8R 0 144 24" posRelativeX="5fad387b688247bf"
            posRelativeY="5fad387b688247bf" editable="0" layout="33" items=""
            textWhenNonSelected="apply..." textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="buttonCsv" id="d9c286724f47cdb0" memberName="buttonCsv"
              virtualName="" explicitFocusOrder="0" pos="-8r 0 64 24" posRelativeX="5fad387b688247bf"
              posRelativeY="5fad387b688247bf" buttonText="csv" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonScaling" id="d872dac34b0e60ef" memberName="buttonScaling"
              virtualName="" explicitFocusOrder="0" pos="-80r 0 64 24" posRelativeX="5fad387b688247bf"
              posRelativeY="5fad387b688247bf" buttonText="scaling" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="sliderPort" id="32291ecaacc9e3c0" memberName="sliderPort"
          virtualName="" explicitFocusOrder="0" pos="290Rr 20 100 24" posRelativeX="f4cf3a53a6ef0d87"
          posRelativeY="f4cf3a53a6ef0d87" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderTimeout" id="781da96f4673e99c" memberName="sliderTimeout"
          virtualName="" explicitFocusOrder="0" pos="8Rr 20 170 24" posRelativeX="f4cf3a53a6ef0d87"
          posRelativeY="f4cf3a53a6ef0d87" min="10.0" max="10000.0" int="1.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="70" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

