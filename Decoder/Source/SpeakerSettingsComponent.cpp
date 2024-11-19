/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
#include "../../Common/ImportExport.h"
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
//[/MiscUserDefs]

//==============================================================================
SpeakerSettingsComponent::SpeakerSettingsComponent (AmbiSpeakerSet* _pSpeakerSet, DecoderPresetHelper* _pPresetHelper, PointSelection* _pPointSelection, AmbiSettingsCollection* _pAmbiSettings, DecoderSettings* _pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* _pFilterSpecification, ZoomSettings* _pZoomSettings, ChannelLayout* _pChannelLayout)
    : pSpeakerSet(_pSpeakerSet), pPresetHelper(_pPresetHelper), pPointSelection(_pPointSelection), pAmbiSettings(_pAmbiSettings),pDecoderSettings(_pDecoderSettings), pFilterSpecification(_pFilterSpecification), pZoomSettings(_pZoomSettings), pChannelLayout(_pChannelLayout)
{
    //[Constructor_pre] You can add your own custom stuff here..
	OwnedArray<String> ambiChannelNames;
	for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
		ambiChannelNames.add(new String("m = " + String(i)));
	this->pTestSoundGenerator = pTestSoundListener;

	addChangeListener(pCallback);
    pPresetHelper->addActionListener(this);
    pChannelLayout->addActionListener(this);

    filterPresetHelper.reset(new FilterPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiDecoder/Filters"), this));
    filterPresetHelper->initialize();
    //[/Constructor_pre]

    groupOsc.reset (new juce::GroupComponent ("groupOsc",
                                              TRANS ("OSC")));
    addAndMakeVisible (groupOsc.get());

    groupAmbisonics.reset (new juce::GroupComponent ("groupAmbisonics",
                                                     TRANS ("Ambisonics")));
    addAndMakeVisible (groupAmbisonics.get());

    groupSpeakers.reset (new juce::GroupComponent ("groupSpeakers",
                                                   TRANS ("Speakers")));
    addAndMakeVisible (groupSpeakers.get());

    comboBoxChannelConfig.reset (new juce::ComboBox ("channelConfig"));
    addAndMakeVisible (comboBoxChannelConfig.get());
    comboBoxChannelConfig->setEditableText (false);
    comboBoxChannelConfig->setJustificationType (juce::Justification::centredLeft);
    comboBoxChannelConfig->setTextWhenNothingSelected (TRANS ("-"));
    comboBoxChannelConfig->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    comboBoxChannelConfig->addListener (this);

    labelPresets.reset (new juce::Label ("labelPresets",
                                         TRANS ("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
    labelPresets->setJustificationType (juce::Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPresets->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonSave.reset (new juce::TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS ("save"));
    buttonSave->addListener (this);

    speakerList.reset (new TableListBox());
    addAndMakeVisible (speakerList.get());
    speakerList->setName ("speakerList");

    buttonAdd.reset (new juce::TextButton ("buttonAdd"));
    addAndMakeVisible (buttonAdd.get());
    buttonAdd->setButtonText (TRANS ("add"));
    buttonAdd->addListener (this);

    buttonRemove.reset (new juce::TextButton ("buttonAdd"));
    addAndMakeVisible (buttonRemove.get());
    buttonRemove->setButtonText (TRANS ("remove"));
    buttonRemove->addListener (this);

    buttonMoveDown.reset (new juce::TextButton ("buttonMoveDown"));
    addAndMakeVisible (buttonMoveDown.get());
    buttonMoveDown->setButtonText (TRANS ("down"));
    buttonMoveDown->addListener (this);

    buttonMoveUp.reset (new juce::TextButton ("buttonMoveUp"));
    addAndMakeVisible (buttonMoveUp.get());
    buttonMoveUp->setButtonText (TRANS ("up"));
    buttonMoveUp->addListener (this);

    btnEditMode.reset (new juce::ToggleButton ("btnEditMode"));
    addAndMakeVisible (btnEditMode.get());
    btnEditMode->setButtonText (TRANS ("Edit mode"));
    btnEditMode->addListener (this);

    labelOscPort.reset (new juce::Label ("labelOscPort",
                                         TRANS ("OSC-Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
    labelOscPort->setJustificationType (juce::Justification::centredLeft);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelTimeout.reset (new juce::Label ("labelTimeout",
                                         TRANS ("Timeout [ms]:")));
    addAndMakeVisible (labelTimeout.get());
    labelTimeout->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
    labelTimeout->setJustificationType (juce::Justification::centredLeft);
    labelTimeout->setEditable (false, false, false);
    labelTimeout->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelTimeout->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleOsc.reset (new juce::ToggleButton ("toggleOsc"));
    addAndMakeVisible (toggleOsc.get());
    toggleOsc->setButtonText (TRANS ("Receive OSC messages"));
    toggleOsc->addListener (this);

    buttonSpeakerTest.reset (new juce::TextButton ("buttonSpeakerTest"));
    addAndMakeVisible (buttonSpeakerTest.get());
    buttonSpeakerTest->setButtonText (TRANS ("test all speakers"));
    buttonSpeakerTest->addListener (this);

    labelDevelopmentVersion.reset (new juce::Label ("labelDevelopmentVersion",
                                                    TRANS ("Unofficial Pre-Release")));
    addAndMakeVisible (labelDevelopmentVersion.get());
    labelDevelopmentVersion->setFont (juce::Font (juce::FontOptions("Regular", 25.00f, juce::Font::plain)));
    labelDevelopmentVersion->setJustificationType (juce::Justification::centred);
    labelDevelopmentVersion->setEditable (false, false, false);
    labelDevelopmentVersion->setColour (juce::Label::backgroundColourId, juce::Colour (0xbded0d0d));
    labelDevelopmentVersion->setColour (juce::Label::textColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::Label::outlineColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDevelopmentVersion->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonManage.reset (new juce::TextButton ("buttonManage"));
    addAndMakeVisible (buttonManage.get());
    buttonManage->setButtonText (TRANS ("manage..."));
    buttonManage->addListener (this);

    buttonManageFilters.reset (new juce::TextButton ("buttonManageFilters"));
    addAndMakeVisible (buttonManageFilters.get());
    buttonManageFilters->setButtonText (TRANS ("manage filters..."));
    buttonManageFilters->addListener (this);

    buttonCsv.reset (new juce::TextButton ("buttonCsv"));
    addAndMakeVisible (buttonCsv.get());
    buttonCsv->setButtonText (TRANS ("import/export"));
    buttonCsv->addListener (this);

    buttonScaling.reset (new juce::TextButton ("buttonScaling"));
    addAndMakeVisible (buttonScaling.get());
    buttonScaling->setButtonText (TRANS ("scaling"));
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

    toggleMultiDecoder.reset (new juce::ToggleButton ("toggleMultiDecoder"));
    addAndMakeVisible (toggleMultiDecoder.get());
    toggleMultiDecoder->setButtonText (TRANS ("Multi-Decoder mode"));
    toggleMultiDecoder->addListener (this);

    multiDecoderControl.reset (new MultiDecoderComponent(pAmbiSettings, pSpeakerSet, getFilterPresetHelper(), pFilterSpecification, this, pChannelLayout));
    addAndMakeVisible (multiDecoderControl.get());
    multiDecoderControl->setName ("multiDecoderControl");

    ambiSettingsControl.reset (new AmbiSettingsComponent(pAmbiSettings->singleDecoder.get(), this, pChannelLayout));
    addAndMakeVisible (ambiSettingsControl.get());
    ambiSettingsControl->setName ("ambiSettingsControl");


    //[UserPreSize]
    multiDecoderControl->toBehind(toggleMultiDecoder.get());
    ambiSettingsControl->toBehind(toggleMultiDecoder.get());
    //[/UserPreSize]

    setSize (900, 700);


    //[Constructor] You can add your own custom stuff here..
    labelDevelopmentVersion->setVisible(Constants::isDevelopmentVersion() && !Constants::isNonVisibleVersionPrerelease());

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
    speakerList->addMouseListener(this, true);
	updateComboBox();
	pPointSelection->addChangeListener(this);

    updateUI();
    
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
    pChannelLayout->removeActionListener(this);
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
    btnEditMode = nullptr;
    labelOscPort = nullptr;
    labelTimeout = nullptr;
    toggleOsc = nullptr;
    buttonSpeakerTest = nullptr;
    labelDevelopmentVersion = nullptr;
    buttonManage = nullptr;
    buttonManageFilters = nullptr;
    buttonCsv = nullptr;
    buttonScaling = nullptr;
    sliderPort = nullptr;
    sliderTimeout = nullptr;
    toggleMultiDecoder = nullptr;
    multiDecoderControl = nullptr;
    ambiSettingsControl = nullptr;

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

    groupOsc->setBounds ((8 + 0) + 0, (0 + (getHeight() - 317)) + 249, ((getWidth() - 16) - 0) - 0, 60);
    groupAmbisonics->setBounds (8 + 0, 0 + (getHeight() - 317), (getWidth() - 16) - 0, 249);
    groupSpeakers->setBounds (8, 0, getWidth() - 16, getHeight() - 317);
    comboBoxChannelConfig->setBounds (8 + 192, 0 + 24, getWidth() - 531, 24);
    labelPresets->setBounds ((8 + 192) + -8 - 64, 0 + 24, 64, 24);
    buttonSave->setBounds (8 + (getWidth() - 16) - 224 - 80, 0 + 24, 80, 24);
    speakerList->setBounds (8 + 8, 0 + 56, (getWidth() - 16) - 16, (getHeight() - 317) - 96);
    buttonAdd->setBounds ((8 + 8) + 0, (0 + 56) + ((getHeight() - 317) - 96) - -8, 64, 24);
    buttonRemove->setBounds ((8 + 8) + 72, (0 + 56) + ((getHeight() - 317) - 96) - -8, 64, 24);
    buttonMoveDown->setBounds ((8 + 8) + ((getWidth() - 16) - 16) - 64, (0 + 56) + ((getHeight() - 317) - 96) - -8, 64, 24);
    buttonMoveUp->setBounds ((8 + 8) + ((getWidth() - 16) - 16) - 136, (0 + 56) + ((getHeight() - 317) - 96) - -8, 64, 24);
    btnEditMode->setBounds (8 + 8, 0 + 24, 150, 24);
    labelOscPort->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 408 - 85, ((0 + (getHeight() - 317)) + 249) + 20, 85, 24);
    labelTimeout->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 186 - 105, ((0 + (getHeight() - 317)) + 249) + 20, 105, 24);
    toggleOsc->setBounds (((8 + 0) + 0) + 8, ((0 + (getHeight() - 317)) + 249) + 20, 180, 24);
    buttonSpeakerTest->setBounds (proportionOfWidth (0.4982f) - (120 / 2), (0 + 56) + ((getHeight() - 317) - 96) - -8, 120, 24);
    labelDevelopmentVersion->setBounds (proportionOfWidth (0.5000f) - (proportionOfWidth (0.3986f) / 2), 0, proportionOfWidth (0.3986f), 24);
    buttonManage->setBounds (8 + (getWidth() - 16) - 136 - 80, 0 + 24, 80, 24);
    buttonManageFilters->setBounds (8 + (getWidth() - 16) - 8 - 120, 0 + 24, 120, 24);
    buttonCsv->setBounds ((proportionOfWidth (0.4982f) - (120 / 2)) + -8 - 104, ((0 + 56) + ((getHeight() - 317) - 96) - -8) + 0, 104, 24);
    buttonScaling->setBounds ((proportionOfWidth (0.4982f) - (120 / 2)) + -80 - 104, ((0 + 56) + ((getHeight() - 317) - 96) - -8) + 0, 64, 24);
    sliderPort->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 300 - 100, ((0 + (getHeight() - 317)) + 249) + 20, 100, 24);
    sliderTimeout->setBounds (((8 + 0) + 0) + (((getWidth() - 16) - 0) - 0) - 8 - 170, ((0 + (getHeight() - 317)) + 249) + 20, 170, 24);
    toggleMultiDecoder->setBounds ((8 + 0) + 8, (0 + (getHeight() - 317)) + 20, 150, 24);
    multiDecoderControl->setBounds ((8 + 0) + 8, (0 + (getHeight() - 317)) + 20, ((getWidth() - 16) - 0) - 16, 249 - 30);
    ambiSettingsControl->setBounds ((8 + 0) + 8, (0 + (getHeight() - 317)) + 20, ((getWidth() - 16) - 0) - 16, 249 - 30);
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
        if(pSpeakerSet->size() < pChannelLayout->getNumOutputChannels())
        {
            Uuid newId = Uuid();
            pSpeakerSet->add(new AmbiSpeaker(newId.toString(), Point3D<double>(0.0, 0.0, 0.0), "new", TrackColors::getSpeakerColor()));
            pPointSelection->selectPoint(pSpeakerSet->size() - 1);
            speakerList->updateContent();
            speakerList->repaint();
        }
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
        bool change = ImportExport::importExport(pSpeakerSet, buttonCsv.get(), false);
        if (change)
        {
            sendChangeMessage();
            controlDimming();
            speakerList->updateContent();
            speakerList->repaint();
        }
        //[/UserButtonCode_buttonCsv]
    }
    else if (buttonThatWasClicked == buttonScaling.get())
    {
        //[UserButtonCode_buttonScaling] -- add your button handler code here..
        CallOutBox::launchAsynchronously(std::make_unique<ScalingComponent>(this, pSpeakerSet, pZoomSettings), buttonScaling->getBounds(), this);
        //[/UserButtonCode_buttonScaling]
    }
    else if (buttonThatWasClicked == toggleMultiDecoder.get())
    {
        //[UserButtonCode_toggleMultiDecoder] -- add your button handler code here..
        pAmbiSettings->setMultiDecoderFlag(toggleMultiDecoder->getToggleState());
        controlDimming();
        //[/UserButtonCode_toggleMultiDecoder]
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
        numericBox->setJustificationType(Justification::right);
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
    case COLUMN_ID_MUTE: pSpeakerSet->setMute(rowNumber, !exactlyEqual(newValue, 0.0)); break;

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
    multiDecoderControl->repaint();
    multiDecoderControl->refresh();

    btnEditMode->setToggleState(pDecoderSettings->editMode, dontSendNotification);
    toggleMultiDecoder->setToggleState(pAmbiSettings->getMultiDecoderFlag(), dontSendNotification);
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
    if(message == ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED)
    {
        speakerList->updateContent();
        speakerList->repaint();
        controlDimming();
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
	buttonAdd->setEnabled(en && pSpeakerSet->size() < pChannelLayout->getNumOutputChannels());
	buttonRemove->setEnabled(en);
	buttonMoveUp->setEnabled(en && !multiselection);
	buttonMoveDown->setEnabled(en && !multiselection);
	buttonSpeakerTest->setEnabled(en);

    bool isMulti = pAmbiSettings->getMultiDecoderFlag();
    multiDecoderControl->setVisible(isMulti);
    ambiSettingsControl->setVisible(!isMulti);
}

void SpeakerSettingsComponent::mouseUp(const MouseEvent &event)
{
    if(event.mods.isCtrlDown() || event.mods.isRightButtonDown())
    {
        auto rel = event.getScreenPosition()-speakerList->getScreenPosition();
        int row = speakerList->getRowContainingPosition(rel.x, rel.y);

        // show menu only if multiple rows are selected and the mouse click has hit a row
        if(row != -1 && pPointSelection->getSelectedIndices().size() > 1)
        {
            PopupMenu m;
            m.addItem("Apply Color from CH " + String(row + 1), [this, row](){
                Colour color = pSpeakerSet->get(row)->getColor();
                auto selection = pPointSelection->getSelectedIndices();
                for (int i = 0; i < selection.size(); i++)
                {
                    if (selection[i] != row)
                    {
                        pSpeakerSet->get(selection[i])->setColor(color);
                    }
                }
                speakerList->updateContent();
                speakerList->repaint();
            });

            m.addItem("Apply Filter from CH " + String(row + 1), [this, row](){
                FilterBankInfo* info = pSpeakerSet->get(row)->getFilterInfo();
                bool bypass = pSpeakerSet->get(row)->getFilterBypass();
                auto selection = pPointSelection->getSelectedIndices();
                for (int i = 0; i < selection.size(); i++)
                {
                    if (selection[i] != row)
                    {
                        pSpeakerSet->get(selection[i])->getFilterInfo()->copyFrom(info);
                        pSpeakerSet->get(selection[i])->setFilterBypass(bypass);
                    }
                }
                speakerList->updateContent();
                speakerList->repaint();
            });
            m.addItem("Apply Gain from CH " + String(row + 1), [this, row](){
                double gainFactor = pSpeakerSet->get(row)->getGain();
                auto selection = pPointSelection->getSelectedIndices();
                for (int i = 0; i < selection.size(); i++)
                {
                    if (selection[i] != row)
                    {
                        pSpeakerSet->get(selection[i])->setGain(gainFactor, true);
                    }
                }
                speakerList->updateContent();
                speakerList->repaint();
            });

            m.addItem(TRANS("Gain -0.5 dB"), [this](){
                float gainFactor = float(Decibels::decibelsToGain(-0.5));
                auto selection = pPointSelection->getSelectedIndices();
                for (auto index : selection)
                {
                    pSpeakerSet->get(index)->setGain(pSpeakerSet->get(index)->getGain() * gainFactor, true);
                }
                speakerList->updateContent();
                speakerList->repaint();
            });
            m.addItem(TRANS("Gain +0.5 dB"), [this](){
                float gainFactor = float(Decibels::decibelsToGain(0.5));
                auto selection = pPointSelection->getSelectedIndices();
                for (auto index : selection)
                {
                    pSpeakerSet->get(index)->setGain(pSpeakerSet->get(index)->getGain() * gainFactor, true);
                }
                speakerList->updateContent();
                speakerList->repaint();
            });
            m.show();
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
                 constructorParams="AmbiSpeakerSet* _pSpeakerSet, DecoderPresetHelper* _pPresetHelper, PointSelection* _pPointSelection, AmbiSettingsCollection* _pAmbiSettings, DecoderSettings* _pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* _pFilterSpecification, ZoomSettings* _pZoomSettings, ChannelLayout* _pChannelLayout"
                 variableInitialisers="pSpeakerSet(_pSpeakerSet), pPresetHelper(_pPresetHelper), pPointSelection(_pPointSelection), pAmbiSettings(_pAmbiSettings),pDecoderSettings(_pDecoderSettings), pFilterSpecification(_pFilterSpecification), pZoomSettings(_pZoomSettings), pChannelLayout(_pChannelLayout)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="900" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupOsc" id="f4cf3a53a6ef0d87" memberName="groupOsc" virtualName=""
                  explicitFocusOrder="0" pos="0 0R 0M 60" posRelativeX="17eb4b418501687a"
                  posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                  title="OSC"/>
  <GROUPCOMPONENT name="groupAmbisonics" id="17eb4b418501687a" memberName="groupAmbisonics"
                  virtualName="" explicitFocusOrder="0" pos="0 0R 0M 249" posRelativeX="450188aa0f332e78"
                  posRelativeY="450188aa0f332e78" posRelativeW="450188aa0f332e78"
                  title="Ambisonics"/>
  <GROUPCOMPONENT name="groupSpeakers" id="450188aa0f332e78" memberName="groupSpeakers"
                  virtualName="" explicitFocusOrder="0" pos="8 0 16M 317M" title="Speakers"/>
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
  <TOGGLEBUTTON name="btnEditMode" id="c11c14f07c9141ac" memberName="btnEditMode"
                virtualName="" explicitFocusOrder="0" pos="8 24 150 24" posRelativeX="450188aa0f332e78"
                posRelativeY="450188aa0f332e78" buttonText="Edit mode" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="408Rr 20 85 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="OSC-Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelTimeout" id="628a0500d66bc466" memberName="labelTimeout"
         virtualName="" explicitFocusOrder="0" pos="186Rr 20 105 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Timeout [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="toggleOsc" id="1b103b47888e742b" memberName="toggleOsc"
                virtualName="" explicitFocusOrder="0" pos="8 20 180 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Receive OSC messages"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="buttonSpeakerTest" id="5fad387b688247bf" memberName="buttonSpeakerTest"
              virtualName="" explicitFocusOrder="0" pos="49.817%c -8R 120 24"
              posRelativeY="34ae3e87c64e62da" buttonText="test all speakers"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelDevelopmentVersion" id="c41821090201078b" memberName="labelDevelopmentVersion"
         virtualName="" explicitFocusOrder="0" pos="49.939%c 0 39.805% 24"
         bkgCol="bded0d0d" textCol="ffffff00" outlineCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="Unofficial Pre-Release" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="25.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="buttonManage" id="a4621bea805565b3" memberName="buttonManage"
              virtualName="" explicitFocusOrder="0" pos="136Rr 24 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonManageFilters" id="5b471faa99c7496b" memberName="buttonManageFilters"
              virtualName="" explicitFocusOrder="0" pos="8Rr 24 120 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage filters..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonCsv" id="d9c286724f47cdb0" memberName="buttonCsv"
              virtualName="" explicitFocusOrder="0" pos="-8r 0 104 24" posRelativeX="5fad387b688247bf"
              posRelativeY="5fad387b688247bf" buttonText="import/export" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonScaling" id="d872dac34b0e60ef" memberName="buttonScaling"
              virtualName="" explicitFocusOrder="0" pos="-80r 0 64 24" posRelativeX="5fad387b688247bf"
              posRelativeY="5fad387b688247bf" buttonText="scaling" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="sliderPort" id="32291ecaacc9e3c0" memberName="sliderPort"
          virtualName="" explicitFocusOrder="0" pos="300Rr 20 100 24" posRelativeX="f4cf3a53a6ef0d87"
          posRelativeY="f4cf3a53a6ef0d87" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderTimeout" id="781da96f4673e99c" memberName="sliderTimeout"
          virtualName="" explicitFocusOrder="0" pos="8Rr 20 170 24" posRelativeX="f4cf3a53a6ef0d87"
          posRelativeY="f4cf3a53a6ef0d87" min="10.0" max="10000.0" int="1.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="70" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TOGGLEBUTTON name="toggleMultiDecoder" id="d25afde82619da9f" memberName="toggleMultiDecoder"
                virtualName="" explicitFocusOrder="0" pos="8 20 150 24" posRelativeX="17eb4b418501687a"
                posRelativeY="17eb4b418501687a" buttonText="Multi-Decoder mode"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="multiDecoderControl" id="bb0e70808f450c14" memberName="multiDecoderControl"
                    virtualName="" explicitFocusOrder="0" pos="8 20 16M 30M" posRelativeX="17eb4b418501687a"
                    posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                    posRelativeH="17eb4b418501687a" class="MultiDecoderComponent"
                    params="pAmbiSettings, pSpeakerSet, getFilterPresetHelper(), pFilterSpecification, this, pChannelLayout"/>
  <GENERICCOMPONENT name="ambiSettingsControl" id="bb0e706735450c14" memberName="ambiSettingsControl"
                    virtualName="" explicitFocusOrder="0" pos="8 20 16M 30M" posRelativeX="17eb4b418501687a"
                    posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                    posRelativeH="17eb4b418501687a" class="AmbiSettingsComponent"
                    params="pAmbiSettings->singleDecoder.get(), this, pChannelLayout"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

