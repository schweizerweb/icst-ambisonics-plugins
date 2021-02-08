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
#include "../../Common/EditableTextCustomComponent.h"
#include "CheckBoxCustomComponent.h"
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "SpeakerTestCustomComponent.h"
#include "../../Common/TrackColors.h"
#include "../../Common/Constants.h"
#include "FilterSettingsComponent.h"
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
#define	COLUMN_ID_D			12
#define COLUMN_ID_DISTANCE	3
#define COLUMN_ID_DELAY		4
#define COLUMN_ID_DELAY_COMPENSATION 13
#define COLUMN_ID_COLOR     15
#define COLUMN_ID_GAIN		5
#define	COLUMN_ID_TEST		6
#define COLUMN_ID_FILTER 14
//[/MiscUserDefs]

//==============================================================================
SpeakerSettingsComponent::SpeakerSettingsComponent (AmbiSpeakerSet* pSpeakerSet, DecoderPresetHelper* pPresetHelper, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, DecoderSettings* pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* pFilterSpecification)
    : pSpeakerSet(pSpeakerSet), pPresetHelper(pPresetHelper), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings),pDecoderSettings(pDecoderSettings), pFilterSpecification(pFilterSpecification)
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

    sliderDistanceScaler.reset (new juce::Slider ("sliderDistanceScaler"));
    addAndMakeVisible (sliderDistanceScaler.get());
    sliderDistanceScaler->setRange (1, 500, 0.1);
    sliderDistanceScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);

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

    labelDistanceScaler.reset (new juce::Label ("labelDistanceScaler",
                                                TRANS("Distance scaler")));
    addAndMakeVisible (labelDistanceScaler.get());
    labelDistanceScaler->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceScaler->setJustificationType (juce::Justification::centredLeft);
    labelDistanceScaler->setEditable (false, false, false);
    labelDistanceScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDistanceScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnEditMode.reset (new juce::ToggleButton ("btnEditMode"));
    addAndMakeVisible (btnEditMode.get());
    btnEditMode->setButtonText (TRANS("Edit mode"));
    btnEditMode->addListener (this);

    textOscPort.reset (new juce::TextEditor ("textOscPort"));
    addAndMakeVisible (textOscPort.get());
    textOscPort->setMultiLine (false);
    textOscPort->setReturnKeyStartsNewLine (false);
    textOscPort->setReadOnly (false);
    textOscPort->setScrollbarsShown (true);
    textOscPort->setCaretVisible (true);
    textOscPort->setPopupMenuEnabled (true);
    textOscPort->setText (juce::String());

    labelOscPort.reset (new juce::Label ("labelOscPort",
                                         TRANS("OSC-Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscPort->setJustificationType (juce::Justification::centredLeft);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textTimeout.reset (new juce::TextEditor ("textTimeout"));
    addAndMakeVisible (textTimeout.get());
    textTimeout->setMultiLine (false);
    textTimeout->setReturnKeyStartsNewLine (false);
    textTimeout->setReadOnly (false);
    textTimeout->setScrollbarsShown (true);
    textTimeout->setCaretVisible (true);
    textTimeout->setPopupMenuEnabled (true);
    textTimeout->setText (juce::String());

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
    buttonSpeakerTest->setButtonText (TRANS("Test all speakers"));
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


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1200, 800);


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
	speakerList->getHeader().addColumn("A", COLUMN_ID_A, 50);
	speakerList->getHeader().addColumn("E", COLUMN_ID_E, 50);
	//speakerList->getHeader().addColumn("D", COLUMN_ID_D, 50);
	speakerList->getHeader().addColumn("Distance [m]", COLUMN_ID_DISTANCE, 80);
	speakerList->getHeader().addColumn("Delay [ms]", COLUMN_ID_DELAY, 80);
	speakerList->getHeader().addColumn("Delay comp. [ms]", COLUMN_ID_DELAY_COMPENSATION, 100);
	speakerList->getHeader().addColumn("Color", COLUMN_ID_COLOR, 30);
	speakerList->getHeader().addColumn("Gain [dB]", COLUMN_ID_GAIN, 80);
	speakerList->getHeader().addColumn("Test", COLUMN_ID_TEST, 30);
    speakerList->getHeader().addColumn("Filter", COLUMN_ID_FILTER, 40);
	speakerList->getHeader().setStretchToFitActive(true);
	speakerList->getHeader().resizeAllColumnsToFit(speakerList->getWidth());
	updateComboBox();
	pPointSelection->addChangeListener(this);
    updateUI();

    btnEditMode->setToggleState(pDecoderSettings->editMode, dontSendNotification);

	// OSC
	textOscPort->addListener(this);
	textTimeout->addListener(this);
	toggleOsc->setToggleState(pDecoderSettings->oscReceive, dontSendNotification);
	textOscPort->setText(String(pDecoderSettings->oscReceivePort));
	textTimeout->setText(String(pDecoderSettings->oscReceiveTimeoutMs));

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
    sliderDistanceScaler = nullptr;
    ambiChannelControl = nullptr;
    labelChannelWeights = nullptr;
    labelDistanceScaler = nullptr;
    btnEditMode = nullptr;
    textOscPort = nullptr;
    labelOscPort = nullptr;
    textTimeout = nullptr;
    labelTimeout = nullptr;
    toggleOsc = nullptr;
    buttonSpeakerTest = nullptr;
    labelDevelopmentVersion = nullptr;
    buttonManage = nullptr;
    comboBoxChannelWeightingMode = nullptr;
    buttonManageFilters = nullptr;


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

    groupOsc->setBounds ((8 + 0) + 0, (0 + (getHeight() - 267)) + 199, ((getWidth() - 18) - 0) - 0, 60);
    groupAmbisonics->setBounds (8 + 0, 0 + (getHeight() - 267), (getWidth() - 18) - 0, 199);
    groupSpeakers->setBounds (8, 0, getWidth() - 18, getHeight() - 267);
    comboBoxChannelConfig->setBounds (8 + 192, 0 + 24, getWidth() - 531, 24);
    labelPresets->setBounds ((8 + 192) + -8 - 64, 0 + 24, 64, 24);
    buttonSave->setBounds (8 + (getWidth() - 18) - 233 - 80, 0 + 24, 80, 24);
    speakerList->setBounds (8 + 16, 0 + 56, (getWidth() - 18) - 32, (getHeight() - 267) - 96);
    buttonAdd->setBounds ((8 + 16) + 0, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    buttonRemove->setBounds ((8 + 16) + 72, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    buttonMoveDown->setBounds ((8 + 16) + ((getWidth() - 18) - 32) - 64, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    buttonMoveUp->setBounds ((8 + 16) + ((getWidth() - 18) - 32) - 136, (0 + 56) + ((getHeight() - 267) - 96) - -8, 64, 24);
    sliderDistanceScaler->setBounds ((8 + 0) + 424, (0 + (getHeight() - 267)) + 20, getWidth() - 458, 24);
    ambiChannelControl->setBounds ((8 + 0) + 16, (0 + (getHeight() - 267)) + 52, ((getWidth() - 18) - 0) - 32, 199 - 68);
    labelChannelWeights->setBounds ((8 + 0) + 16, (0 + (getHeight() - 267)) + 20, 112, 24);
    labelDistanceScaler->setBounds ((8 + 0) + 312, (0 + (getHeight() - 267)) + 20, 104, 24);
    btnEditMode->setBounds (8 + 16, 0 + 24, 150, 24);
    textOscPort->setBounds (((8 + 0) + 0) + (((getWidth() - 18) - 0) - 0) - 217 - 88, ((0 + (getHeight() - 267)) + 199) + 20, 88, 24);
    labelOscPort->setBounds (((8 + 0) + 0) + (((getWidth() - 18) - 0) - 0) - 313 - 72, ((0 + (getHeight() - 267)) + 199) + 20, 72, 24);
    textTimeout->setBounds (((8 + 0) + 0) + (((getWidth() - 18) - 0) - 0) - 15 - 82, ((0 + (getHeight() - 267)) + 199) + 20, 82, 24);
    labelTimeout->setBounds (((8 + 0) + 0) + (((getWidth() - 18) - 0) - 0) - 108 - 93, ((0 + (getHeight() - 267)) + 199) + 20, 93, 24);
    toggleOsc->setBounds (((8 + 0) + 0) + 16, ((0 + (getHeight() - 267)) + 199) + 20, 180, 24);
    buttonSpeakerTest->setBounds (proportionOfWidth (0.4977f) - (120 / 2), (0 + 56) + ((getHeight() - 267) - 96) - -8, 120, 24);
    labelDevelopmentVersion->setBounds (proportionOfWidth (0.5000f) - (proportionOfWidth (0.3991f) / 2), 0, proportionOfWidth (0.3991f), 24);
    buttonManage->setBounds (8 + (getWidth() - 18) - 145 - 80, 0 + 24, 80, 24);
    comboBoxChannelWeightingMode->setBounds ((8 + 0) + 136, (0 + (getHeight() - 267)) + 20, 120, 24);
    buttonManageFilters->setBounds (8 + (getWidth() - 18) - 17 - 120, 0 + 24, 120, 24);
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
		int selection = pPointSelection->getMainSelectedPointIndex();
		if (selection >= 0 && selection < pSpeakerSet->size())
		{
			pPointSelection->unselectPoint();
			pSpeakerSet->remove(selection);
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SpeakerSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderDistanceScaler.get())
    {
        //[UserSliderCode_sliderDistanceScaler] -- add your slider handling code here..
		pAmbiSettings->setDistanceScaler(sliderDistanceScaler->getValue());
		speakerList->updateContent();
		speakerList->repaint();
        //[/UserSliderCode_sliderDistanceScaler]
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
	if(lastRowSelected >= 0 && lastRowSelected < pSpeakerSet->size())
		pPointSelection->selectPoint(lastRowSelected);
}

void SpeakerSettingsComponent::paintRowBackground(Graphics& g, int rowNumber, int, int, bool rowIsSelected)
{
	const Colour alternateColour(getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
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
	case COLUMN_ID_DISTANCE: text = String(pt->getPoint()->getDistance() * pAmbiSettings->getDistanceScaler(), 2); break;
	case COLUMN_ID_DELAY: text = String(delayHelper.getTotalDelayMs(pAmbiSettings, pt), 2); break;
	case COLUMN_ID_DELAY_COMPENSATION: text = String(delayHelper.getDelayCompensationMs(pAmbiSettings, pSpeakerSet->getMaxNormalizedDistance(), pt), 2); break;
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
		|| columnId == COLUMN_ID_D
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
	case COLUMN_ID_D: pSpeakerSet->setDistance(rowNumber, newValue); break;
	case COLUMN_ID_DISTANCE: pSpeakerSet->setDistance(rowNumber, newValue / pAmbiSettings->getDistanceScaler()); break;
    case COLUMN_ID_COLOR: pSpeakerSet->setChannelColor(rowNumber, Colour(uint32(newValue))); break;
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
	case COLUMN_ID_X: return pt->getPoint()->getX();
	case COLUMN_ID_Y: return pt->getPoint()->getY();
	case COLUMN_ID_Z: return pt->getPoint()->getZ();
	case COLUMN_ID_A: return Constants::RadToGrad(pt->getPoint()->getAzimuth());
	case COLUMN_ID_E: return Constants::RadToGrad(pt->getPoint()->getElevation());
	case COLUMN_ID_D: return pt->getPoint()->getDistance();
	case COLUMN_ID_DISTANCE: return pt->getPoint()->getDistance() * pAmbiSettings->getDistanceScaler();
    case COLUMN_ID_COLOR: return pt->getColor().getARGB();
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
		return SliderRange(-1.0, 1.0, 0.001);

	case COLUMN_ID_D:
		return SliderRange(Constants::DistanceMin, sqrt(2.0), 0.001);

	case COLUMN_ID_A:
		return SliderRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);

	case COLUMN_ID_E:
		return SliderRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);

	case COLUMN_ID_GAIN:
		return SliderRange(Constants::GainDbMin, Constants::GainDbMax, 0.5);

	case COLUMN_ID_DISTANCE:
		return SliderRange(0.0, pAmbiSettings->getDistanceScaler(), 0.001);
	}

	return SliderRange(0.0, 1.0, 0.001);
}

bool SpeakerSettingsComponent::getEnabled(const int /*columnId*/, const int /*rowNumber*/)
{
    return true;
}

void SpeakerSettingsComponent::updateDistanceScaler() const
{
	sliderDistanceScaler->setValue(pAmbiSettings->getDistanceScaler());
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

	updateDistanceScaler();
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
		speakerList->selectRow(pPointSelection->getMainSelectedPointIndex());
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
	groupSpeakers->setEnabled(en);
	labelPresets->setEnabled(en);
	comboBoxChannelConfig->setEnabled(en);
	speakerList->setEnabled(en);
	buttonManage->setEnabled(en);
	buttonSave->setEnabled(en);
	buttonAdd->setEnabled(en);
	buttonRemove->setEnabled(en);
	buttonMoveUp->setEnabled(en);
	buttonMoveDown->setEnabled(en);
	buttonSpeakerTest->setEnabled(en);

    ambiChannelControl->setEnabled(pAmbiSettings->getWeightMode() == AmbiSettings::MANUAL);
}

void SpeakerSettingsComponent::textEditorTextChanged(TextEditor& editor)
{
	if (&editor == textOscPort.get())
	{
		if (textOscPort->getText().containsOnly("0123456789"))
		{
			pDecoderSettings->oscReceivePort = textOscPort->getText().getIntValue();
			sendChangeMessage();
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Invalid port number");
			editor.undo();
		}
	}
	else if (&editor == textTimeout.get())
	{
		if (textTimeout->getText().containsOnly("0123456789"))
		{
			pDecoderSettings->oscReceiveTimeoutMs = textTimeout->getText().getIntValue();
			sendChangeMessage();
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Invalid timeout");
			editor.undo();
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
                 componentName="" parentClasses="public Component, public TableListBoxModel, public ChangeListener, public TextEditor::Listener, public ActionBroadcaster, public ChangeBroadcaster, public TableColumnCallback, ActionListener"
                 constructorParams="AmbiSpeakerSet* pSpeakerSet, DecoderPresetHelper* pPresetHelper, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, DecoderSettings* pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* pFilterSpecification"
                 variableInitialisers="pSpeakerSet(pSpeakerSet), pPresetHelper(pPresetHelper), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings),pDecoderSettings(pDecoderSettings), pFilterSpecification(pFilterSpecification)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1200" initialHeight="800">
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
                  virtualName="" explicitFocusOrder="0" pos="8 0 18M 267M" title="Speakers"/>
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
              virtualName="" explicitFocusOrder="0" pos="233Rr 24 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="speakerList" id="34ae3e87c64e62da" memberName="speakerList"
                    virtualName="" explicitFocusOrder="0" pos="16 56 32M 96M" posRelativeX="450188aa0f332e78"
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
  <SLIDER name="sliderDistanceScaler" id="8ae6ec5973e2470e" memberName="sliderDistanceScaler"
          virtualName="" explicitFocusOrder="0" pos="424 20 458M 24" posRelativeX="17eb4b418501687a"
          posRelativeY="17eb4b418501687a" min="1.0" max="500.0" int="0.1"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="ambiChannelControl" id="4ec5a32a175ea48d" memberName="ambiChannelControl"
                    virtualName="" explicitFocusOrder="0" pos="16 52 32M 68M" posRelativeX="17eb4b418501687a"
                    posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                    posRelativeH="17eb4b418501687a" class="MultiSliderControl" params="CURRENT_AMBISONICS_ORDER_NB_OF_GAINS, pAmbiSettings-&gt;getAmbiOrderWeightPointer(), &amp;ambiChannelNames, 0.0, 1.5, 0.00001"/>
  <LABEL name="labelChannelWeights" id="ce2f83213d847908" memberName="labelChannelWeights"
         virtualName="" explicitFocusOrder="0" pos="16 20 112 24" posRelativeX="17eb4b418501687a"
         posRelativeY="17eb4b418501687a" edTextCol="ff000000" edBkgCol="0"
         labelText="Channel weights" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelDistanceScaler" id="bbbf87bcedfbda85" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="312 20 104 24" posRelativeX="17eb4b418501687a"
         posRelativeY="17eb4b418501687a" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance scaler" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnEditMode" id="c11c14f07c9141ac" memberName="btnEditMode"
                virtualName="" explicitFocusOrder="0" pos="16 24 150 24" posRelativeX="450188aa0f332e78"
                posRelativeY="450188aa0f332e78" buttonText="Edit mode" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscPort" id="ef3c7d2594795ec7" memberName="textOscPort"
              virtualName="" explicitFocusOrder="0" pos="217Rr 20 88 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="313Rr 20 72 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="OSC-Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textTimeout" id="337c3e6db7308866" memberName="textTimeout"
              virtualName="" explicitFocusOrder="0" pos="15Rr 20 82 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelTimeout" id="628a0500d66bc466" memberName="labelTimeout"
         virtualName="" explicitFocusOrder="0" pos="108Rr 20 93 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Timeout [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="toggleOsc" id="1b103b47888e742b" memberName="toggleOsc"
                virtualName="" explicitFocusOrder="0" pos="16 20 180 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Receive OSC messages"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="buttonSpeakerTest" id="5fad387b688247bf" memberName="buttonSpeakerTest"
              virtualName="" explicitFocusOrder="0" pos="49.784%c -8R 120 24"
              posRelativeY="34ae3e87c64e62da" buttonText="Test all speakers"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelDevelopmentVersion" id="c41821090201078b" memberName="labelDevelopmentVersion"
         virtualName="" explicitFocusOrder="0" pos="50%c 0 39.856% 24"
         bkgCol="bded0d0d" textCol="ffffff00" outlineCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="Unofficial Pre-Release" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="25.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="buttonManage" id="a4621bea805565b3" memberName="buttonManage"
              virtualName="" explicitFocusOrder="0" pos="145Rr 24 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="comboBoxChannelWeightingMode" id="e9f5f23a259dd1c0" memberName="comboBoxChannelWeightingMode"
            virtualName="" explicitFocusOrder="0" pos="136 20 120 24" posRelativeX="17eb4b418501687a"
            posRelativeY="17eb4b418501687a" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="buttonManageFilters" id="5b471faa99c7496b" memberName="buttonManageFilters"
              virtualName="" explicitFocusOrder="0" pos="17Rr 24 120 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage filters..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

