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
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/ColorEditorCustomComponent.h"
#include "../../Common/TrackColors.h"
#include "DistanceEncodingComponent.h"
//[/Headers]

#include "EncoderSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define COLUMN_ID_NB		1
#define COLUMN_ID_NAME		2
#define	COLUMN_ID_X			7
#define	COLUMN_ID_Y			8
#define	COLUMN_ID_Z			9
#define	COLUMN_ID_A			10
#define	COLUMN_ID_E			11
#define	COLUMN_ID_D			12
#define COLUMN_ID_GAIN		5
#define COLUMN_ID_COLOR		13

//[/MiscUserDefs]

//==============================================================================
EncoderSettingsComponent::EncoderSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams)
    : pEncoderSettings(pSettings), pSources(pSourceSet), pPointSelection(pPointSelection), pAudioParams(pAudioParams)
{
    //[Constructor_pre] You can add your own custom stuff here..
	addChangeListener(pChangeListener);

	groupModel.reset(new GroupTableListModel(pSourceSet, pPointSelection, this));
    //[/Constructor_pre]

    multiEncoderElements.reset (new Component());
    addAndMakeVisible (multiEncoderElements.get());
    multiEncoderElements->setName ("multiEncoderElements");

    groupSources.reset (new GroupComponent ("groupSources",
                                            TRANS("Sources")));
    addAndMakeVisible (groupSources.get());

    groupOsc.reset (new GroupComponent ("groupOsc",
                                        TRANS("OSC")));
    addAndMakeVisible (groupOsc.get());

    toggleReceiveOsc.reset (new ToggleButton ("toggleReceiveOsc"));
    addAndMakeVisible (toggleReceiveOsc.get());
    toggleReceiveOsc->setButtonText (TRANS("Receive OSC"));
    toggleReceiveOsc->addListener (this);

    textOscReceivePort.reset (new TextEditor ("textOscReceivePort"));
    addAndMakeVisible (textOscReceivePort.get());
    textOscReceivePort->setMultiLine (false);
    textOscReceivePort->setReturnKeyStartsNewLine (false);
    textOscReceivePort->setReadOnly (false);
    textOscReceivePort->setScrollbarsShown (true);
    textOscReceivePort->setCaretVisible (true);
    textOscReceivePort->setPopupMenuEnabled (true);
    textOscReceivePort->setText (String());

    labelOscPort.reset (new Label ("labelOscPort",
                                   TRANS("Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscPort->setJustificationType (Justification::centredRight);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (TextEditor::textColourId, Colours::black);
    labelOscPort->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    toggleSendOsc.reset (new ToggleButton ("toggleSendOsc"));
    addAndMakeVisible (toggleSendOsc.get());
    toggleSendOsc->setButtonText (TRANS("Send Positions to Decoder"));
    toggleSendOsc->addListener (this);

    textOscSendIp.reset (new TextEditor ("textOscSendIp"));
    addAndMakeVisible (textOscSendIp.get());
    textOscSendIp->setMultiLine (false);
    textOscSendIp->setReturnKeyStartsNewLine (false);
    textOscSendIp->setReadOnly (false);
    textOscSendIp->setScrollbarsShown (true);
    textOscSendIp->setCaretVisible (true);
    textOscSendIp->setPopupMenuEnabled (true);
    textOscSendIp->setText (String());

    labelOscSendIp.reset (new Label ("labelOscSendIp",
                                     TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIp.get());
    labelOscSendIp->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIp->setJustificationType (Justification::centredRight);
    labelOscSendIp->setEditable (false, false, false);
    labelOscSendIp->setColour (TextEditor::textColourId, Colours::black);
    labelOscSendIp->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textOscSendPort.reset (new TextEditor ("textOscSendPort"));
    addAndMakeVisible (textOscSendPort.get());
    textOscSendPort->setMultiLine (false);
    textOscSendPort->setReturnKeyStartsNewLine (false);
    textOscSendPort->setReadOnly (false);
    textOscSendPort->setScrollbarsShown (true);
    textOscSendPort->setCaretVisible (true);
    textOscSendPort->setPopupMenuEnabled (true);
    textOscSendPort->setText (String());

    textOscSendInterval.reset (new TextEditor ("textOscSendInterval"));
    addAndMakeVisible (textOscSendInterval.get());
    textOscSendInterval->setMultiLine (false);
    textOscSendInterval->setReturnKeyStartsNewLine (false);
    textOscSendInterval->setReadOnly (false);
    textOscSendInterval->setScrollbarsShown (true);
    textOscSendInterval->setCaretVisible (true);
    textOscSendInterval->setPopupMenuEnabled (true);
    textOscSendInterval->setText (String());

    labelOscSendInterval.reset (new Label ("labelOscSendInterval",
                                           TRANS("Interval [ms]:")));
    addAndMakeVisible (labelOscSendInterval.get());
    labelOscSendInterval->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendInterval->setJustificationType (Justification::centredRight);
    labelOscSendInterval->setEditable (false, false, false);
    labelOscSendInterval->setColour (TextEditor::textColourId, Colours::black);
    labelOscSendInterval->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    groupEncoding.reset (new GroupComponent ("groupEncoding",
                                             TRANS("Encoding")));
    addAndMakeVisible (groupEncoding.get());

    toggleDistanceEncoding.reset (new ToggleButton ("toggleDistanceEncoding"));
    addAndMakeVisible (toggleDistanceEncoding.get());
    toggleDistanceEncoding->setButtonText (TRANS("Enable Distance Encoding"));
    toggleDistanceEncoding->addListener (this);

    toggleDirectionFlip.reset (new ToggleButton ("toggleDirectionFlip"));
    addAndMakeVisible (toggleDirectionFlip.get());
    toggleDirectionFlip->setButtonText (TRANS("Flip Direction (Orientation)"));
    toggleDirectionFlip->addListener (this);

    toggleSendOscExt.reset (new ToggleButton ("toggleSendOscExt"));
    addAndMakeVisible (toggleSendOscExt.get());
    toggleSendOscExt->setButtonText (TRANS("Send Positions for External Usage"));
    toggleSendOscExt->addListener (this);

    textOscSendIpExt.reset (new TextEditor ("textOscSendIpExt"));
    addAndMakeVisible (textOscSendIpExt.get());
    textOscSendIpExt->setMultiLine (false);
    textOscSendIpExt->setReturnKeyStartsNewLine (false);
    textOscSendIpExt->setReadOnly (false);
    textOscSendIpExt->setScrollbarsShown (true);
    textOscSendIpExt->setCaretVisible (true);
    textOscSendIpExt->setPopupMenuEnabled (true);
    textOscSendIpExt->setText (String());

    labelOscSendIpExt.reset (new Label ("labelOscSendIpExt",
                                        TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIpExt.get());
    labelOscSendIpExt->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIpExt->setJustificationType (Justification::centredRight);
    labelOscSendIpExt->setEditable (false, false, false);
    labelOscSendIpExt->setColour (TextEditor::textColourId, Colours::black);
    labelOscSendIpExt->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textOscSendPortExt.reset (new TextEditor ("textOscSendPortExt"));
    addAndMakeVisible (textOscSendPortExt.get());
    textOscSendPortExt->setMultiLine (false);
    textOscSendPortExt->setReturnKeyStartsNewLine (false);
    textOscSendPortExt->setReadOnly (false);
    textOscSendPortExt->setScrollbarsShown (true);
    textOscSendPortExt->setCaretVisible (true);
    textOscSendPortExt->setPopupMenuEnabled (true);
    textOscSendPortExt->setText (String());

    sourceList.reset (new TableListBox());
    addAndMakeVisible (sourceList.get());
    sourceList->setName ("sourceList");

    buttonAdd.reset (new TextButton ("buttonAdd"));
    addAndMakeVisible (buttonAdd.get());
    buttonAdd->setButtonText (TRANS("add"));
    buttonAdd->addListener (this);

    buttonRemove.reset (new TextButton ("buttonRemove"));
    addAndMakeVisible (buttonRemove.get());
    buttonRemove->setButtonText (TRANS("remove"));
    buttonRemove->addListener (this);

    buttonMoveDown.reset (new TextButton ("buttonMoveDown"));
    addAndMakeVisible (buttonMoveDown.get());
    buttonMoveDown->setButtonText (TRANS("down"));
    buttonMoveDown->addListener (this);

    buttonMoveUp.reset (new TextButton ("buttonMoveUp"));
    addAndMakeVisible (buttonMoveUp.get());
    buttonMoveUp->setButtonText (TRANS("up"));
    buttonMoveUp->addListener (this);

    groupGroups.reset (new GroupComponent ("groupGroups",
                                           TRANS("Groups")));
    addAndMakeVisible (groupGroups.get());

    groupList.reset (new TableListBox());
    addAndMakeVisible (groupList.get());
    groupList->setName ("groupList");

    buttonAddGroup.reset (new TextButton ("buttonAddGroup"));
    addAndMakeVisible (buttonAddGroup.get());
    buttonAddGroup->setButtonText (TRANS("add"));
    buttonAddGroup->addListener (this);

    buttonRemoveGroup.reset (new TextButton ("buttonRemoveGroup"));
    addAndMakeVisible (buttonRemoveGroup.get());
    buttonRemoveGroup->setButtonText (TRANS("remove"));
    buttonRemoveGroup->addListener (this);

    comboBoxPresets.reset (new ComboBox ("comboBoxPresets"));
    addAndMakeVisible (comboBoxPresets.get());
    comboBoxPresets->setEditableText (false);
    comboBoxPresets->setJustificationType (Justification::centredLeft);
    comboBoxPresets->setTextWhenNothingSelected (TRANS("-"));
    comboBoxPresets->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxPresets->addListener (this);

    labelPresets.reset (new Label ("labelPresets",
                                   TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (TextEditor::textColourId, Colours::black);
    labelPresets->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    buttonImport.reset (new TextButton ("buttonImport"));
    addAndMakeVisible (buttonImport.get());
    buttonImport->setButtonText (TRANS("import"));
    buttonImport->addListener (this);

    buttonSave.reset (new TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    buttonExport.reset (new TextButton ("buttonExport"));
    addAndMakeVisible (buttonExport.get());
    buttonExport->setButtonText (TRANS("export"));
    buttonExport->addListener (this);

    labelDevelopmentVersion.reset (new Label ("labelDevelopmentVersion",
                                              TRANS("Unofficial Pre-Release")));
    addAndMakeVisible (labelDevelopmentVersion.get());
    labelDevelopmentVersion->setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelDevelopmentVersion->setJustificationType (Justification::centred);
    labelDevelopmentVersion->setEditable (false, false, false);
    labelDevelopmentVersion->setColour (Label::backgroundColourId, Colour (0xbded0d0d));
    labelDevelopmentVersion->setColour (Label::textColourId, Colours::yellow);
    labelDevelopmentVersion->setColour (Label::outlineColourId, Colours::yellow);
    labelDevelopmentVersion->setColour (TextEditor::textColourId, Colours::black);
    labelDevelopmentVersion->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    toggleDoppler.reset (new ToggleButton ("toggleDoppler"));
    addAndMakeVisible (toggleDoppler.get());
    toggleDoppler->setButtonText (TRANS("Enable Doppler"));
    toggleDoppler->addListener (this);

    sliderDistanceScaler.reset (new Slider ("sliderDistanceScaler"));
    addAndMakeVisible (sliderDistanceScaler.get());
    sliderDistanceScaler->setRange (1, 1000, 0);
    sliderDistanceScaler->setSliderStyle (Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);

    labelDistanceScaler.reset (new Label ("labelDistanceScaler",
                                          TRANS("Distance Scaler:")));
    addAndMakeVisible (labelDistanceScaler.get());
    labelDistanceScaler->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceScaler->setJustificationType (Justification::centredRight);
    labelDistanceScaler->setEditable (false, false, false);
    labelDistanceScaler->setColour (TextEditor::textColourId, Colours::black);
    labelDistanceScaler->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    btnEditDistanceEncoding.reset (new TextButton ("btnEditDistanceEncoding"));
    addAndMakeVisible (btnEditDistanceEncoding.get());
    btnEditDistanceEncoding->setButtonText (TRANS("edit..."));
    btnEditDistanceEncoding->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 300);


    //[Constructor] You can add your own custom stuff here..
	labelDevelopmentVersion->setVisible(Constants::isDevelopmentVersion());
	textOscReceivePort->addListener(this);
	textOscSendPort->addListener(this);
	textOscSendIp->addListener(this);
	textOscSendInterval->addListener(this);
	textOscSendPortExt->addListener(this);
	textOscSendIpExt->addListener(this);

	toggleReceiveOsc->setToggleState(pEncoderSettings->oscReceiveFlag, dontSendNotification);
	textOscReceivePort->setText(String(pEncoderSettings->oscReceivePort));

	toggleSendOsc->setToggleState(pEncoderSettings->oscSendFlag, dontSendNotification);
	textOscSendIp->setText(String(pEncoderSettings->oscSendTargetHost));
	textOscSendPort->setText(String(pEncoderSettings->oscSendPort));
	textOscSendInterval->setText(String(pEncoderSettings->oscSendIntervalMs));

	toggleSendOscExt->setToggleState(pEncoderSettings->oscSendExtFlag, dontSendNotification);
	textOscSendIpExt->setText(String(pEncoderSettings->oscSendExtTargetHost));
	textOscSendPortExt->setText(String(pEncoderSettings->oscSendExtPort));

	toggleDistanceEncoding->setToggleState(pEncoderSettings->distanceEncodingFlag, dontSendNotification);
	toggleDirectionFlip->setToggleState(pEncoderSettings->getDirectionFlip(), dontSendNotification);

	toggleDoppler->setToggleState(pEncoderSettings->dopplerEncodingFlag, dontSendNotification);
	sliderDistanceScaler->setValue(pEncoderSettings->getDistanceScaler());

	groupSources->setVisible(MULTI_ENCODER_MODE);
	buttonAdd->setVisible(MULTI_ENCODER_MODE);
	buttonRemove->setVisible(MULTI_ENCODER_MODE);
	buttonMoveUp->setVisible(MULTI_ENCODER_MODE);
	buttonMoveDown->setVisible(MULTI_ENCODER_MODE);
	sourceList->setVisible(MULTI_ENCODER_MODE);
	groupGroups->setVisible(MULTI_ENCODER_MODE);
	groupList->setVisible(MULTI_ENCODER_MODE);
	buttonAddGroup->setVisible(MULTI_ENCODER_MODE);
	buttonRemoveGroup->setVisible(MULTI_ENCODER_MODE);
	labelPresets->setVisible(MULTI_ENCODER_MODE);
	comboBoxPresets->setVisible(MULTI_ENCODER_MODE);
	buttonSave->setVisible(MULTI_ENCODER_MODE);
	buttonImport->setVisible(MULTI_ENCODER_MODE);
	buttonExport->setVisible(MULTI_ENCODER_MODE);

	// table
	sourceList->setModel(this);
	sourceList->getHeader().addColumn("CH", COLUMN_ID_NB, 30);
	sourceList->getHeader().addColumn("Name", COLUMN_ID_NAME, 100);
	sourceList->getHeader().addColumn("X", COLUMN_ID_X, 50);
	sourceList->getHeader().addColumn("Y", COLUMN_ID_Y, 50);
	sourceList->getHeader().addColumn("Z", COLUMN_ID_Z, 50);
	sourceList->getHeader().addColumn("A", COLUMN_ID_A, 50);
	sourceList->getHeader().addColumn("E", COLUMN_ID_E, 50);
	sourceList->getHeader().addColumn("D", COLUMN_ID_D, 50);
	sourceList->getHeader().addColumn("Gain [dB]", COLUMN_ID_GAIN, 80);
	sourceList->getHeader().addColumn("Color", COLUMN_ID_COLOR, 60);
	sourceList->getHeader().setStretchToFitActive(true);
	sourceList->getHeader().resizeAllColumnsToFit(sourceList->getWidth());

	groupModel->initTable(groupList.get());
	pPointSelection->addChangeListener(this);

	// load stored presets

	initializePresets();

	controlDimming();
    //[/Constructor]
}

EncoderSettingsComponent::~EncoderSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	pPointSelection->removeChangeListener(this);
    //[/Destructor_pre]

    multiEncoderElements = nullptr;
    groupSources = nullptr;
    groupOsc = nullptr;
    toggleReceiveOsc = nullptr;
    textOscReceivePort = nullptr;
    labelOscPort = nullptr;
    toggleSendOsc = nullptr;
    textOscSendIp = nullptr;
    labelOscSendIp = nullptr;
    textOscSendPort = nullptr;
    textOscSendInterval = nullptr;
    labelOscSendInterval = nullptr;
    groupEncoding = nullptr;
    toggleDistanceEncoding = nullptr;
    toggleDirectionFlip = nullptr;
    toggleSendOscExt = nullptr;
    textOscSendIpExt = nullptr;
    labelOscSendIpExt = nullptr;
    textOscSendPortExt = nullptr;
    sourceList = nullptr;
    buttonAdd = nullptr;
    buttonRemove = nullptr;
    buttonMoveDown = nullptr;
    buttonMoveUp = nullptr;
    groupGroups = nullptr;
    groupList = nullptr;
    buttonAddGroup = nullptr;
    buttonRemoveGroup = nullptr;
    comboBoxPresets = nullptr;
    labelPresets = nullptr;
    buttonImport = nullptr;
    buttonSave = nullptr;
    buttonExport = nullptr;
    labelDevelopmentVersion = nullptr;
    toggleDoppler = nullptr;
    sliderDistanceScaler = nullptr;
    labelDistanceScaler = nullptr;
    btnEditDistanceEncoding = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	groupModel = nullptr;
    //[/Destructor]
}

//==============================================================================
void EncoderSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EncoderSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    multiEncoderElements->setBounds (8, 152 + 112, getWidth() - 17, getHeight() - 303);
    groupSources->setBounds (8 + 0, (152 + 112) + 0, (getWidth() - 17) - 0, roundToInt ((getHeight() - 303) * 0.6000f));
    groupOsc->setBounds (8, 8, getWidth() - 17, 144);
    toggleReceiveOsc->setBounds (8 + 14, 8 + 19, 150, 24);
    textOscReceivePort->setBounds (8 + (getWidth() - 17) - 24 - 48, 8 + 19, 48, 24);
    labelOscPort->setBounds (8 + (getWidth() - 17) - 75 - 49, 8 + 19, 49, 24);
    toggleSendOsc->setBounds (8 + 14, 8 + 79, 199, 24);
    textOscSendIp->setBounds (8 + (getWidth() - 17) - 78 - 106, 8 + 79, 106, 24);
    labelOscSendIp->setBounds (8 + (getWidth() - 17) - 188 - 124, 8 + 79, 124, 24);
    textOscSendPort->setBounds (8 + (getWidth() - 17) - 24 - 48, 8 + 79, 48, 24);
    textOscSendInterval->setBounds (8 + (getWidth() - 17) - 24 - 48, 8 + 109, 48, 24);
    labelOscSendInterval->setBounds (8 + (getWidth() - 17) - 76 - 110, 8 + 109, 110, 24);
    groupEncoding->setBounds (8, 152, getWidth() - 17, 112);
    toggleDistanceEncoding->setBounds (8 + 14, 152 + 19, 199, 24);
    toggleDirectionFlip->setBounds (8 + 14, 152 + 79, 207, 24);
    toggleSendOscExt->setBounds (8 + 14, 8 + 49, 245, 24);
    textOscSendIpExt->setBounds (8 + (getWidth() - 17) - 78 - 106, 8 + 49, 106, 24);
    labelOscSendIpExt->setBounds (8 + (getWidth() - 17) - 188 - 126, 8 + 49, 126, 24);
    textOscSendPortExt->setBounds (8 + (getWidth() - 17) - 24 - 48, 8 + 49, 48, 24);
    sourceList->setBounds ((8 + 0) + 16, ((152 + 112) + 0) + 19, ((getWidth() - 17) - 0) - 31, (roundToInt ((getHeight() - 303) * 0.6000f)) - 67);
    buttonAdd->setBounds ((8 + 0) + 17, ((152 + 112) + 0) + (roundToInt ((getHeight() - 303) * 0.6000f)) - 40, 64, 24);
    buttonRemove->setBounds ((8 + 0) + 89, ((152 + 112) + 0) + (roundToInt ((getHeight() - 303) * 0.6000f)) - 40, 64, 24);
    buttonMoveDown->setBounds ((8 + 0) + ((getWidth() - 17) - 0) - 80, ((152 + 112) + 0) + (roundToInt ((getHeight() - 303) * 0.6000f)) - 40, 64, 24);
    buttonMoveUp->setBounds ((8 + 0) + ((getWidth() - 17) - 0) - 152, ((152 + 112) + 0) + (roundToInt ((getHeight() - 303) * 0.6000f)) - 40, 64, 24);
    groupGroups->setBounds (8 + 0, (152 + 112) + (getHeight() - 303) - (roundToInt ((getHeight() - 303) * 0.4000f)), (getWidth() - 17) - 0, roundToInt ((getHeight() - 303) * 0.4000f));
    groupList->setBounds ((8 + 0) + 16, ((152 + 112) + (getHeight() - 303) - (roundToInt ((getHeight() - 303) * 0.4000f))) + 19, ((getWidth() - 17) - 0) - 31, (roundToInt ((getHeight() - 303) * 0.4000f)) - 67);
    buttonAddGroup->setBounds ((8 + 0) + 17, ((152 + 112) + (getHeight() - 303) - (roundToInt ((getHeight() - 303) * 0.4000f))) + (roundToInt ((getHeight() - 303) * 0.4000f)) - 40, 64, 24);
    buttonRemoveGroup->setBounds ((8 + 0) + 89, ((152 + 112) + (getHeight() - 303) - (roundToInt ((getHeight() - 303) * 0.4000f))) + (roundToInt ((getHeight() - 303) * 0.4000f)) - 40, 64, 24);
    comboBoxPresets->setBounds (83, getHeight() - 34, getWidth() - 312, 24);
    labelPresets->setBounds (8, getHeight() - 34, 64, 24);
    buttonImport->setBounds (getWidth() - 81 - 64, getHeight() - 34, 64, 24);
    buttonSave->setBounds (getWidth() - 153 - 64, getHeight() - 34, 64, 24);
    buttonExport->setBounds (getWidth() - 9 - 64, getHeight() - 34, 64, 24);
    labelDevelopmentVersion->setBounds (proportionOfWidth (0.5000f) - (proportionOfWidth (0.3984f) / 2), 8, proportionOfWidth (0.3984f), 24);
    toggleDoppler->setBounds (8 + 14, 152 + 49, 199, 24);
    sliderDistanceScaler->setBounds (8 + (getWidth() - 17) - 24 - 202, 152 + 49, 202, 24);
    labelDistanceScaler->setBounds (8 + (getWidth() - 17) - 229 - 109, 152 + 49, 109, 24);
    btnEditDistanceEncoding->setBounds (8 + (getWidth() - 17) - 24 - 86, 152 + 19, 86, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EncoderSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleReceiveOsc.get())
    {
        //[UserButtonCode_toggleReceiveOsc] -- add your button handler code here..
		pEncoderSettings->oscReceiveFlag = toggleReceiveOsc->getToggleState();
		sendChangeMessage();
        //[/UserButtonCode_toggleReceiveOsc]
    }
    else if (buttonThatWasClicked == toggleSendOsc.get())
    {
        //[UserButtonCode_toggleSendOsc] -- add your button handler code here..
		pEncoderSettings->oscSendFlag = toggleSendOsc->getToggleState();
		sendChangeMessage();
        //[/UserButtonCode_toggleSendOsc]
    }
    else if (buttonThatWasClicked == toggleDistanceEncoding.get())
    {
        //[UserButtonCode_toggleDistanceEncoding] -- add your button handler code here..
		pEncoderSettings->distanceEncodingFlag = toggleDistanceEncoding->getToggleState();
		sendChangeMessage();
        //[/UserButtonCode_toggleDistanceEncoding]
    }
    else if (buttonThatWasClicked == toggleDirectionFlip.get())
    {
        //[UserButtonCode_toggleDirectionFlip] -- add your button handler code here..
		pEncoderSettings->setDirectionFlip(toggleDirectionFlip->getToggleState());
		sendChangeMessage();
        //[/UserButtonCode_toggleDirectionFlip]
    }
    else if (buttonThatWasClicked == toggleSendOscExt.get())
    {
        //[UserButtonCode_toggleSendOscExt] -- add your button handler code here..
		pEncoderSettings->oscSendExtFlag = toggleSendOscExt->getToggleState();
		sendChangeMessage();
        //[/UserButtonCode_toggleSendOscExt]
    }
    else if (buttonThatWasClicked == buttonAdd.get())
    {
        //[UserButtonCode_buttonAdd] -- add your button handler code here..
		if (pAudioParams != nullptr && pSources->size() < pAudioParams->size())
		{
			Uuid newId = Uuid();
			pSources->addNew(newId.toString(), Point3D<double>(0.0, 0.0, 0.0, pAudioParams->getUnchecked(pSources->size())), pSources->getNewUniqueName(), TrackColors::getColor(pSources->size() + 1));
			pPointSelection->selectPoint(pSources->size() - 1);
			sourceList->updateContent();
			sourceList->repaint();
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "No more sources allowed!");
		}
        //[/UserButtonCode_buttonAdd]
    }
    else if (buttonThatWasClicked == buttonRemove.get())
    {
        //[UserButtonCode_buttonRemove] -- add your button handler code here..
		int selection = pPointSelection->getMainSelectedPointIndex();
		if (pPointSelection->getSelectionMode() == PointSelection::Point && selection >= 0 && selection < pSources->size())
		{
			pPointSelection->unselectPoint();
			pSources->remove(selection);
			sourceList->updateContent();
			sourceList->repaint();
		}
        //[/UserButtonCode_buttonRemove]
    }
    else if (buttonThatWasClicked == buttonMoveDown.get())
    {
        //[UserButtonCode_buttonMoveDown] -- add your button handler code here..
		int selection = pPointSelection->getMainSelectedPointIndex();
		if (selection >= 0 && selection < pSources->size() - 1)
		{
			pPointSelection->unselectPoint();
			pSources->swap(selection, selection + 1);
			pPointSelection->selectPoint(selection + 1);
		}
        //[/UserButtonCode_buttonMoveDown]
    }
    else if (buttonThatWasClicked == buttonMoveUp.get())
    {
        //[UserButtonCode_buttonMoveUp] -- add your button handler code here..
		int selection = pPointSelection->getMainSelectedPointIndex();
		if (selection >= 1 && selection < pSources->size())
		{
			pPointSelection->unselectPoint();
			pSources->swap(selection, selection - 1);
			pPointSelection->selectPoint(selection - 1);
		}
        //[/UserButtonCode_buttonMoveUp]
    }
    else if (buttonThatWasClicked == buttonAddGroup.get())
    {
        //[UserButtonCode_buttonAddGroup] -- add your button handler code here..
		Uuid newId = Uuid();
		pSources->addGroup(newId.toString(), Point3D<double>(0.0, 0.0, 0.0), "G", Colours::orange);
		pPointSelection->selectGroup(pSources->groupCount() - 1, false);
		groupList->updateContent();
		groupList->repaint();
        //[/UserButtonCode_buttonAddGroup]
    }
    else if (buttonThatWasClicked == buttonRemoveGroup.get())
    {
        //[UserButtonCode_buttonRemoveGroup] -- add your button handler code here..
		int selection = pPointSelection->getMainSelectedPointIndex();
		if (pPointSelection->getSelectionMode() == PointSelection::Group && selection >= 0 && selection < pSources->size())
		{
			pPointSelection->unselectPoint();
			pSources->removeGroup(selection);
			groupList->updateContent();
			groupList->repaint();
		}

        //[/UserButtonCode_buttonRemoveGroup]
    }
    else if (buttonThatWasClicked == buttonImport.get())
    {
        //[UserButtonCode_buttonImport] -- add your button handler code here..
		FileChooser chooser("Select preset XML to import...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
		if (chooser.browseForFileToOpen())
		{
			File importFile(chooser.getResult());
			AmbiSourceSet testSet;
			if(testSet.loadFromXmlFile(importFile))
			{
				importFile.copyFileTo(presetDirectory.getFullPathName() + "/" + importFile.getFileName());
				initializePresets();
				comboBoxPresets->setText(importFile.getFileNameWithoutExtension(), sendNotificationAsync);
			}
			else
			{
				AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset import", "Error loading preset " + importFile.getFullPathName());
			}
		}
        //[/UserButtonCode_buttonImport]
    }
    else if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
		AlertWindow alert("Save Preset", "", AlertWindow::NoIcon);
		Array<String> existingPresets;
		existingPresets.add("");
		for (File preset : presets)
    		existingPresets.add(preset.getFileNameWithoutExtension());

		alert.addComboBox("existing", existingPresets, "Overwrite existing");
		alert.addTextEditor("text", "", "Or enter new name", false);
		alert.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
		alert.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
		alert.getComboBoxComponent("existing")->setText(comboBoxPresets->getText());

		int returnValue = alert.runModalLoop();
		if(returnValue == 1)
		{
			String presetName = alert.getComboBoxComponent("existing")->getText();
			if(presetName.isEmpty())
				presetName = alert.getTextEditorContents("text");

			if (presetName.isEmpty() || File::createLegalFileName(presetName) != presetName)
			{
				AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid preset name: " + presetName);
				return;
			}

			File newFile(presetDirectory.getFullPathName() + "/" + presetName + ".xml");

			if (newFile.existsAsFile())
			{
				AlertWindow confirm("Overwrite?", "Are you sure to overwrite preset \"" + presetName + "\"?", AlertWindow::QuestionIcon);
				confirm.addButton("No", 0, KeyPress(KeyPress::escapeKey, 0, 0));
				confirm.addButton("Yes", 1, KeyPress(KeyPress::returnKey, 0, 0));
				if (confirm.runModalLoop() == 0)
				{
					return;
				}
			}

			pSources->writeToXmlFile(newFile);
			initializePresets();
			comboBoxPresets->setText(presetName, dontSendNotification);
		}
        //[/UserButtonCode_buttonSave]
    }
    else if (buttonThatWasClicked == buttonExport.get())
    {
        //[UserButtonCode_buttonExport] -- add your button handler code here..
		FileChooser chooser("Select filename to export...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
		if (chooser.browseForFileToSave(true))
		{
			File exportFile(chooser.getResult());
			if(!pSources->writeToXmlFile(exportFile))
			{
				AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset export", "Error exporting preset to " + exportFile.getFullPathName());
			}
		}
        //[/UserButtonCode_buttonExport]
    }
    else if (buttonThatWasClicked == toggleDoppler.get())
    {
        //[UserButtonCode_toggleDoppler] -- add your button handler code here..
		pEncoderSettings->dopplerEncodingFlag = toggleDoppler->getToggleState();
		sendChangeMessage();
        //[/UserButtonCode_toggleDoppler]
    }
    else if (buttonThatWasClicked == btnEditDistanceEncoding.get())
    {
        //[UserButtonCode_btnEditDistanceEncoding] -- add your button handler code here..
		CallOutBox::launchAsynchronously(new DistanceEncodingComponent(&pEncoderSettings->distanceEncodingParams), getScreenBounds(), nullptr);
        //[/UserButtonCode_btnEditDistanceEncoding]
    }

    //[UserbuttonClicked_Post]
	controlDimming();
    //[/UserbuttonClicked_Post]
}

void EncoderSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxPresets.get())
    {
        //[UserComboBoxCode_comboBoxPresets] -- add your combo box handling code here..
		String presetToLoad = comboBoxPresets->getText();
		for (File preset : presets)
		{
			if (preset.getFileNameWithoutExtension() == presetToLoad)
			{
				pSources->loadFromXmlFile(preset, pAudioParams);
				sourceList->updateContent();
				sourceList->repaint();
				groupList->updateContent();
				groupList->repaint();
			}
		}
        //[/UserComboBoxCode_comboBoxPresets]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void EncoderSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderDistanceScaler.get())
    {
        //[UserSliderCode_sliderDistanceScaler] -- add your slider handling code here..
		pEncoderSettings->setDistanceScaler(sliderDistanceScaler->getValue());
		sendChangeMessage();
        //[/UserSliderCode_sliderDistanceScaler]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void EncoderSettingsComponent::initializePresets()
{
	presetDirectory = File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder");
	if (!presetDirectory.exists())
		presetDirectory.createDirectory();

	presets.clear();
	comboBoxPresets->clear();

	int id = 1;
	DirectoryIterator iterator(presetDirectory, false, "*.xml");
	while (iterator.next())
	{
		// try to load preset
		AmbiSourceSet test;
		if (test.loadFromXmlFile(iterator.getFile()))
		{
			String name = iterator.getFile().getFileNameWithoutExtension();
			presets.add(iterator.getFile());
			comboBoxPresets->addItem(name, id++);
		}
	}
}

void EncoderSettingsComponent::textEditorTextChanged(TextEditor& textEditor)
{
	if (&textEditor == textOscReceivePort.get())
		checkForNumbers(&textEditor, &pEncoderSettings->oscReceivePort);
	else if (&textEditor == textOscSendPort.get())
		checkForNumbers(&textEditor, &pEncoderSettings->oscSendPort);
	else if (&textEditor == textOscSendInterval.get())
		checkForNumbers(&textEditor, &pEncoderSettings->oscSendIntervalMs);
	else if (&textEditor == textOscSendIp.get())
		pEncoderSettings->oscSendTargetHost = textEditor.getText();
	else if (&textEditor == textOscSendPortExt.get())
		checkForNumbers(&textEditor, &pEncoderSettings->oscSendExtPort);
	else if (&textEditor == textOscSendIpExt.get())
		pEncoderSettings->oscSendExtTargetHost = textEditor.getText();

	sendChangeMessage();
}

void EncoderSettingsComponent::controlDimming() const
{
	btnEditDistanceEncoding->setEnabled(pEncoderSettings->distanceEncodingFlag);
	textOscReceivePort->setEnabled(pEncoderSettings->oscReceiveFlag);
	textOscSendIp->setEnabled(pEncoderSettings->oscSendFlag);
	textOscSendPort->setEnabled(pEncoderSettings->oscSendFlag);
	textOscSendInterval->setEnabled(pEncoderSettings->oscSendFlag);
	textOscSendIpExt->setEnabled(pEncoderSettings->oscSendExtFlag);
	textOscSendPortExt->setEnabled(pEncoderSettings->oscSendExtFlag);
	buttonAdd->setEnabled(pSources->size() < pAudioParams->size());
	buttonRemove->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Point && pSources->size() > 0);
	buttonMoveUp->setEnabled(pPointSelection->getMainSelectedPointIndex() > 0);
	buttonMoveDown->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getMainSelectedPointIndex() < pSources->size() - 1);
	buttonAdd->setEnabled(true);
	buttonRemoveGroup->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Group && pSources->groupCount() > 0);
}

void EncoderSettingsComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	sourceList->updateContent();
	sourceList->repaint();
	groupList->updateContent();
	groupList->repaint();

	if (source == pPointSelection)
	{
		if (pPointSelection->getSelectionMode() == PointSelection::Point)
		{
			sourceList->selectRow(pPointSelection->getMainSelectedPointIndex());
			groupList->selectRow(-1);
		}
		else
		{
			sourceList->selectRow(-1);
			groupList->selectRow(pPointSelection->getMainSelectedPointIndex());
		}

		controlDimming();
	}
}

int EncoderSettingsComponent::getNumRows()
{
	return pSources->size();
}

void EncoderSettingsComponent::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
	const Colour alternateColour(getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
}

void EncoderSettingsComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
	AmbiPoint* pt = pSources->get(rowNumber);
	if (pt == nullptr)
		return;

	g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
	String text;
	switch (columnId)
	{
	case COLUMN_ID_NB: text = String(rowNumber + 1); break;
	case COLUMN_ID_NAME: text = pt->getName(); break;
	default: text = "";
	}
	g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

double EncoderSettingsComponent::getValue(int columnId, int rowNumber)
{
	AmbiPoint* pt = pSources->get(rowNumber);
	if (pt == nullptr)
		return 0.0;

	switch (columnId)
	{
	case COLUMN_ID_GAIN: return 10.0 * log10(pt->getGain());
	case COLUMN_ID_X: return pt->getPoint()->getX();
	case COLUMN_ID_Y: return pt->getPoint()->getY();
	case COLUMN_ID_Z: return pt->getPoint()->getZ();
	case COLUMN_ID_A: return Constants::RadToGrad(pt->getPoint()->getAzimuth());
	case COLUMN_ID_E: return Constants::RadToGrad(pt->getPoint()->getElevation());
	case COLUMN_ID_D: return pt->getPoint()->getDistance();
	case COLUMN_ID_COLOR: return pt->getColor().getARGB();
	default: return 0.0;
	}
}

void EncoderSettingsComponent::setValue(int columnId, int rowNumber, double newValue)
{
	switch (columnId)
	{
	case COLUMN_ID_GAIN: pSources->setGain(rowNumber, pow(10.0, 0.1 * newValue)); break;
	case COLUMN_ID_X: pSources->setX(rowNumber, newValue); break;
	case COLUMN_ID_Y: pSources->setY(rowNumber, newValue); break;
	case COLUMN_ID_Z: pSources->setZ(rowNumber, newValue); break;
	case COLUMN_ID_A: pSources->setAzimuth(rowNumber, Constants::GradToRad(newValue)); break;
	case COLUMN_ID_E: pSources->setElevation(rowNumber, Constants::GradToRad(newValue)); break;
	case COLUMN_ID_D: pSources->setDistance(rowNumber, newValue); break;
	case COLUMN_ID_COLOR: pSources->setChannelColor(rowNumber, Colour(uint32(newValue))); break;
	default: throw;
	}

	sourceList->updateContent();
	sourceList->repaint();
}

SliderRange EncoderSettingsComponent::getSliderRange(int columnId)
{
	switch (columnId)
	{
	case COLUMN_ID_X:
	case COLUMN_ID_Y:
	case COLUMN_ID_Z:
		return SliderRange(-1.0, 1.0, 0.001);

	case COLUMN_ID_D:
		return SliderRange(Constants::DistanceMin, Constants::DistanceMax, 0.001);

	case COLUMN_ID_A:
		return SliderRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);

	case COLUMN_ID_E:
		return SliderRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);

	case COLUMN_ID_GAIN:
		return SliderRange(Constants::GainDbMin, Constants::GainDbMax, 0.5);
	default: return SliderRange(0.0, 1.0, 0.001);
	}
}

TableListBox* EncoderSettingsComponent::getTable()
{
	return sourceList.get();
}

String EncoderSettingsComponent::getTableText(const int columnId, const int rowNumber)
{
	AmbiPoint* pt = pSources->get(rowNumber);
	if (pt == nullptr)
		return "";

	switch (columnId)
	{
	case COLUMN_ID_NAME: return pt->getName();
	default: return "";
	}
}

void EncoderSettingsComponent::setTableText(const int columnId, const int rowNumber, const String& newText)
{
	switch (columnId)
	{
	case COLUMN_ID_NAME: pSources->setChannelName(rowNumber, newText); break;
	default: throw;
	}
}

void EncoderSettingsComponent::selectedRowsChanged(int lastRowSelected)
{
	if (lastRowSelected >= 0 && lastRowSelected < pSources->size())
		pPointSelection->selectPoint(lastRowSelected);
}

Component* EncoderSettingsComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate)
{
	if (columnId == COLUMN_ID_X
		|| columnId == COLUMN_ID_Y
		|| columnId == COLUMN_ID_Z
		|| columnId == COLUMN_ID_A
		|| columnId == COLUMN_ID_E
		|| columnId == COLUMN_ID_D)
	{
		NumericColumnCustomComponent* numericBox = static_cast<NumericColumnCustomComponent*> (existingComponentToUpdate);
		if (numericBox == nullptr)
			numericBox = new NumericColumnCustomComponent(*this);

		numericBox->setRowAndColumn(rowNumber, columnId);
		return numericBox;
	}
	else if (columnId == COLUMN_ID_GAIN)
	{
		SliderColumnCustomComponent* gainBox = static_cast<SliderColumnCustomComponent*> (existingComponentToUpdate);
		if (gainBox == nullptr)
			gainBox = new SliderColumnCustomComponent(*this);

		gainBox->setRowAndColumn(rowNumber, columnId);
		return gainBox;
	}
	else if (columnId == COLUMN_ID_NAME)
	{
		EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
		if (textLabel == nullptr)
			textLabel = new EditableTextCustomComponent(*this);

		textLabel->setRowAndColumn(rowNumber, columnId);
		return textLabel;
	}
	else if (columnId == COLUMN_ID_COLOR)
	{
		ColorEditorCustomComponent* colorBox = static_cast<ColorEditorCustomComponent*> (existingComponentToUpdate);
		if (colorBox == nullptr)
			colorBox = new ColorEditorCustomComponent(*this);

		colorBox->setRowAndColumn(rowNumber, columnId);
		return colorBox;
	}

	return nullptr;
}

void EncoderSettingsComponent::checkForNumbers(TextEditor* pEditor, int* pParameter) const
{
	if (pEditor->getText().containsOnly("0123456789"))
	{
		*pParameter = pEditor->getText().getIntValue();
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Invalid number");
		pEditor->undo();
	}
}

void EncoderSettingsComponent::checkForNumbers(TextEditor* pEditor, float* pParameter) const
{
	if (pEditor->getText().containsOnly("0123456789."))
	{
		*pParameter = pEditor->getText().getFloatValue();
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Invalid number");
		pEditor->undo();
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EncoderSettingsComponent"
                 componentName="" parentClasses="public Component, public TextEditor::Listener, public ActionBroadcaster, public ChangeBroadcaster, public TableListBoxModel, public ChangeListener, public TableColumnCallback"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array&lt;AudioParameterSet&gt;* pAudioParams"
                 variableInitialisers="pEncoderSettings(pSettings), pSources(pSourceSet), pPointSelection(pPointSelection), pAudioParams(pAudioParams)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="650" initialHeight="300">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="multiEncoderElements" id="73249ab85d6bba3a" memberName="multiEncoderElements"
                    virtualName="" explicitFocusOrder="0" pos="8 0R 17M 303M" posRelativeY="b72378bdfe4e130"
                    class="Component" params=""/>
  <GROUPCOMPONENT name="groupSources" id="da4e7711e3fff0be" memberName="groupSources"
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 60.038%" posRelativeX="73249ab85d6bba3a"
                  posRelativeY="73249ab85d6bba3a" posRelativeW="73249ab85d6bba3a"
                  posRelativeH="73249ab85d6bba3a" title="Sources"/>
  <GROUPCOMPONENT name="groupOsc" id="f4cf3a53a6ef0d87" memberName="groupOsc" virtualName=""
                  explicitFocusOrder="0" pos="8 8 17M 144" title="OSC"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="14 19 150 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Receive OSC" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscReceivePort" id="ef3c7d2594795ec7" memberName="textOscReceivePort"
              virtualName="" explicitFocusOrder="0" pos="24Rr 19 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="75Rr 19 49 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOsc" id="309574c60d08c55e" memberName="toggleSendOsc"
                virtualName="" explicitFocusOrder="0" pos="14 79 199 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Send Positions to Decoder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIp" id="f5d167c87a68db4c" memberName="textOscSendIp"
              virtualName="" explicitFocusOrder="0" pos="78Rr 79 106 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIp" id="ba6c86d4711d8bd0" memberName="labelOscSendIp"
         virtualName="" explicitFocusOrder="0" pos="188Rr 79 124 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textOscSendPort" id="c2dff2f42ed301d1" memberName="textOscSendPort"
              virtualName="" explicitFocusOrder="0" pos="24Rr 79 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textOscSendInterval" id="2c20c78495fbd07c" memberName="textOscSendInterval"
              virtualName="" explicitFocusOrder="0" pos="24Rr 109 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendInterval" id="104391e6ca4d3f12" memberName="labelOscSendInterval"
         virtualName="" explicitFocusOrder="0" pos="76Rr 109 110 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <GROUPCOMPONENT name="groupEncoding" id="b72378bdfe4e130" memberName="groupEncoding"
                  virtualName="" explicitFocusOrder="0" pos="8 152 17M 112" title="Encoding"/>
  <TOGGLEBUTTON name="toggleDistanceEncoding" id="c46d0c7f045490ec" memberName="toggleDistanceEncoding"
                virtualName="" explicitFocusOrder="0" pos="14 19 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Distance Encoding"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="toggleDirectionFlip" id="261d6104440c6519" memberName="toggleDirectionFlip"
                virtualName="" explicitFocusOrder="0" pos="14 79 207 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Flip Direction (Orientation)"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="toggleSendOscExt" id="4c50ad9f06c0feaf" memberName="toggleSendOscExt"
                virtualName="" explicitFocusOrder="0" pos="14 49 245 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Send Positions for External Usage"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIpExt" id="ed3cbf6e9d145d08" memberName="textOscSendIpExt"
              virtualName="" explicitFocusOrder="0" pos="78Rr 49 106 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExt" id="ee4b90142f7a53f4" memberName="labelOscSendIpExt"
         virtualName="" explicitFocusOrder="0" pos="188Rr 49 126 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textOscSendPortExt" id="8f6b9c97cd3fa86f" memberName="textOscSendPortExt"
              virtualName="" explicitFocusOrder="0" pos="24Rr 49 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <GENERICCOMPONENT name="sourceList" id="54cde0d0bf4f7a53" memberName="sourceList"
                    virtualName="" explicitFocusOrder="0" pos="16 19 31M 67M" posRelativeX="da4e7711e3fff0be"
                    posRelativeY="da4e7711e3fff0be" posRelativeW="da4e7711e3fff0be"
                    posRelativeH="da4e7711e3fff0be" class="TableListBox" params=""/>
  <TEXTBUTTON name="buttonAdd" id="e1290b9a1a32d249" memberName="buttonAdd"
              virtualName="" explicitFocusOrder="0" pos="17 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonRemove" id="49c8de1156e72d8c" memberName="buttonRemove"
              virtualName="" explicitFocusOrder="0" pos="89 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="remove" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveDown" id="7291297cb3544d01" memberName="buttonMoveDown"
              virtualName="" explicitFocusOrder="0" pos="80R 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="down" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveUp" id="e2d399b90fa42e97" memberName="buttonMoveUp"
              virtualName="" explicitFocusOrder="0" pos="152R 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="up" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="groupGroups" id="983b0a3b2c5c945a" memberName="groupGroups"
                  virtualName="" explicitFocusOrder="0" pos="0 0Rr 0M 39.962%"
                  posRelativeX="73249ab85d6bba3a" posRelativeY="73249ab85d6bba3a"
                  posRelativeW="73249ab85d6bba3a" posRelativeH="73249ab85d6bba3a"
                  title="Groups"/>
  <GENERICCOMPONENT name="groupList" id="df462ef21c261681" memberName="groupList"
                    virtualName="" explicitFocusOrder="0" pos="16 19 31M 67M" posRelativeX="983b0a3b2c5c945a"
                    posRelativeY="983b0a3b2c5c945a" posRelativeW="983b0a3b2c5c945a"
                    posRelativeH="983b0a3b2c5c945a" class="TableListBox" params=""/>
  <TEXTBUTTON name="buttonAddGroup" id="84fdb7fb0d342ca6" memberName="buttonAddGroup"
              virtualName="" explicitFocusOrder="0" pos="17 40R 64 24" posRelativeX="983b0a3b2c5c945a"
              posRelativeY="983b0a3b2c5c945a" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonRemoveGroup" id="4b7306753c54f44c" memberName="buttonRemoveGroup"
              virtualName="" explicitFocusOrder="0" pos="89 40R 64 24" posRelativeX="983b0a3b2c5c945a"
              posRelativeY="983b0a3b2c5c945a" buttonText="remove" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="comboBoxPresets" id="4b25adf5b07e9492" memberName="comboBoxPresets"
            virtualName="" explicitFocusOrder="0" pos="83 34R 312M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="8 34R 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonImport" id="5a786eb91323df32" memberName="buttonImport"
              virtualName="" explicitFocusOrder="0" pos="81Rr 34R 64 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="import" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="153Rr 34R 64 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonExport" id="92cbcfe3e89f89e" memberName="buttonExport"
              virtualName="" explicitFocusOrder="0" pos="9Rr 34R 64 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="export" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelDevelopmentVersion" id="c41821090201078b" memberName="labelDevelopmentVersion"
         virtualName="" explicitFocusOrder="0" pos="49.962%c 8 39.82% 24"
         bkgCol="bded0d0d" textCol="ffffff00" outlineCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="Unofficial Pre-Release" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="25.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="toggleDoppler" id="8a9ea68cd17e7a8c" memberName="toggleDoppler"
                virtualName="" explicitFocusOrder="0" pos="14 49 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Doppler" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="sliderDistanceScaler" id="86549d5794437a4a" memberName="sliderDistanceScaler"
          virtualName="" explicitFocusOrder="0" pos="24Rr 49 202 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="1.0" max="1000.0" int="0.0"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelDistanceScaler" id="3db2cd25c7d2d40f" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="229Rr 49 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="btnEditDistanceEncoding" id="d37af0003751ec97" memberName="btnEditDistanceEncoding"
              virtualName="" explicitFocusOrder="0" pos="24Rr 19 86 24" posRelativeX="b72378bdfe4e130"
              posRelativeY="b72378bdfe4e130" buttonText="edit..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

