/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "PresetInfo.h"
#include "EditableTextCustomComponent.h"
#include "SliderColumnCustomComponent.h"
#include "SpeakerTestCustomComponent.h"
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
#define COLUMN_ID_GAIN		5
#define	COLUMN_ID_TEST		6
//[/MiscUserDefs]

//==============================================================================
SpeakerSettingsComponent::SpeakerSettingsComponent (OwnedArray<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, ActionListener* pTestSoundListener)
    : pSpeakerArray(pSpeakerArray), pPresets(pPresets), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
	OwnedArray<String> ambiChannelNames;
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		ambiChannelNames.add(new String("m = " + String(i)));
	addActionListener(pTestSoundListener);
    //[/Constructor_pre]

    addAndMakeVisible (groupAmbisonics = new GroupComponent ("groupAmbisonics",
                                                             TRANS("Ambisonics")));

    addAndMakeVisible (groupSpeakers = new GroupComponent ("groupSpeakers",
                                                           TRANS("Speakers")));

    addAndMakeVisible (comboBoxChannelConfig = new ComboBox ("channelConfig"));
    comboBoxChannelConfig->setEditableText (false);
    comboBoxChannelConfig->setJustificationType (Justification::centredLeft);
    comboBoxChannelConfig->setTextWhenNothingSelected (TRANS("-"));
    comboBoxChannelConfig->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxChannelConfig->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Presets:")));
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (8, 8, 64, 24);

    addAndMakeVisible (buttonLoad = new TextButton ("buttonLoad"));
    buttonLoad->setButtonText (TRANS("load"));
    buttonLoad->addListener (this);

    addAndMakeVisible (buttonSave = new TextButton ("buttonSave"));
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    addAndMakeVisible (speakerList = new TableListBox());
    speakerList->setName ("speakerList");

    addAndMakeVisible (buttonAdd = new TextButton ("buttonAdd"));
    buttonAdd->setButtonText (TRANS("add"));
    buttonAdd->addListener (this);

    addAndMakeVisible (buttonRemove = new TextButton ("buttonAdd"));
    buttonRemove->setButtonText (TRANS("remove"));
    buttonRemove->addListener (this);

    addAndMakeVisible (buttonMoveDown = new TextButton ("buttonMoveDown"));
    buttonMoveDown->setButtonText (TRANS("down"));
    buttonMoveDown->addListener (this);

    addAndMakeVisible (buttonMoveUp = new TextButton ("buttonMoveUp"));
    buttonMoveUp->setButtonText (TRANS("up"));
    buttonMoveUp->addListener (this);

    addAndMakeVisible (sliderDistanceScaler = new Slider ("sliderDistanceScaler"));
    sliderDistanceScaler->setRange (1, 500, 0.1);
    sliderDistanceScaler->setSliderStyle (Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);

    addAndMakeVisible (ambiChannelControl = new MultiSliderControl (CURRENT_AMBISONICS_ORDER_NB_OF_GAINS, pAmbiSettings->getAmbiOrderWeightPointer(), &ambiChannelNames, 0.0, 1.5, 0.01));
    ambiChannelControl->setName ("ambiChannelControl");

    addAndMakeVisible (buttonBasic = new TextButton ("buttonBasic"));
    buttonBasic->setButtonText (TRANS("reset to basic"));
    buttonBasic->addListener (this);

    buttonBasic->setBounds (8 + 12, (getHeight() - 198) + 64, 120, 24);

    addAndMakeVisible (labelChannelWeights = new Label ("labelChannelWeights",
                                                        TRANS("Channel weights")));
    labelChannelWeights->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelChannelWeights->setJustificationType (Justification::centredLeft);
    labelChannelWeights->setEditable (false, false, false);
    labelChannelWeights->setColour (TextEditor::textColourId, Colours::black);
    labelChannelWeights->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelChannelWeights->setBounds (8 + 8, (getHeight() - 198) + 40, 112, 24);

    addAndMakeVisible (buttonInPhase = new TextButton ("buttonInPhase"));
    buttonInPhase->setButtonText (TRANS("calculate in-phase"));
    buttonInPhase->addListener (this);

    buttonInPhase->setBounds (8 + 12, (getHeight() - 198) + 96, 120, 24);

    addAndMakeVisible (btnFlipDirection = new ToggleButton ("btnFlipDirection"));
    btnFlipDirection->setButtonText (TRANS("Flip Direction"));
    btnFlipDirection->addListener (this);

    btnFlipDirection->setBounds (8 + 12, (getHeight() - 198) + 128, 120, 24);

    addAndMakeVisible (labelDistanceScaler = new Label ("labelDistanceScaler",
                                                        TRANS("Distance scaler")));
    labelDistanceScaler->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceScaler->setJustificationType (Justification::centredLeft);
    labelDistanceScaler->setEditable (false, false, false);
    labelDistanceScaler->setColour (TextEditor::textColourId, Colours::black);
    labelDistanceScaler->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelDistanceScaler->setBounds (8 + 8, (getHeight() - 198) + 15, 150, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 800);


    //[Constructor] You can add your own custom stuff here..
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
	speakerList->getHeader().addColumn("Gain", COLUMN_ID_GAIN, 80);
	speakerList->getHeader().addColumn("Test", COLUMN_ID_TEST, 30);
	speakerList->getHeader().resizeAllColumnsToFit(getWidth());
	updateComboBox();
	pPointSelection->addChangeListener(this);
	updateDistanceScaler();
	updateDirectionFlip();
    //[/Constructor]
}

SpeakerSettingsComponent::~SpeakerSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	pPointSelection->removeChangeListener(this);
    //[/Destructor_pre]

    groupAmbisonics = nullptr;
    groupSpeakers = nullptr;
    comboBoxChannelConfig = nullptr;
    label = nullptr;
    buttonLoad = nullptr;
    buttonSave = nullptr;
    speakerList = nullptr;
    buttonAdd = nullptr;
    buttonRemove = nullptr;
    buttonMoveDown = nullptr;
    buttonMoveUp = nullptr;
    sliderDistanceScaler = nullptr;
    ambiChannelControl = nullptr;
    buttonBasic = nullptr;
    labelChannelWeights = nullptr;
    buttonInPhase = nullptr;
    btnFlipDirection = nullptr;
    labelDistanceScaler = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SpeakerSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpeakerSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupAmbisonics->setBounds (8, getHeight() - 198, getWidth() - 18, 192);
    groupSpeakers->setBounds (8, 40, getWidth() - 18, getHeight() - 246);
    comboBoxChannelConfig->setBounds (80, 8, getWidth() - 186, 24);
    buttonLoad->setBounds (getWidth() - 98, 8, 40, 24);
    buttonSave->setBounds (getWidth() - 50, 8, 40, 24);
    speakerList->setBounds (8 + 16, 40 + 16, (getWidth() - 18) - 32, (getHeight() - 246) - 56);
    buttonAdd->setBounds ((8 + 16) + 0, (40 + 16) + ((getHeight() - 246) - 56) - -8, 64, 24);
    buttonRemove->setBounds ((8 + 16) + 72, (40 + 16) + ((getHeight() - 246) - 56) - -8, 64, 24);
    buttonMoveDown->setBounds ((8 + 16) + ((getWidth() - 18) - 32) - 64, (40 + 16) + ((getHeight() - 246) - 56) - -8, 64, 24);
    buttonMoveUp->setBounds ((8 + 16) + ((getWidth() - 18) - 32) - 136, (40 + 16) + ((getHeight() - 246) - 56) - -8, 64, 24);
    sliderDistanceScaler->setBounds (8 + 144, (getHeight() - 198) + 16, getWidth() - 178, 24);
    ambiChannelControl->setBounds (8 + 144, (getHeight() - 198) + 40, (getWidth() - 18) - 160, 192 - 56);
    //[UserResized] Add your own custom resize handling here..
	Rectangle<int> groupBounds = groupAmbisonics->getBounds();
	labelDistanceScaler->setBounds(groupBounds.getX() + 8, groupBounds.getY() + 12, 150, 24);
	labelChannelWeights->setBounds(groupBounds.getX() + 8, groupBounds.getY() + 40, 112, 24);
	buttonBasic->setBounds(groupBounds.getX() + 12, groupBounds.getY() + 64, 120, 24);
	buttonInPhase->setBounds(groupBounds.getX() + 12, groupBounds.getY() + 96, 120, 24);
	btnFlipDirection->setBounds(groupBounds.getX() + 12, groupBounds.getY() + 128, 120, 24);
    //[/UserResized]
}

void SpeakerSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxChannelConfig)
    {
        //[UserComboBoxCode_comboBoxChannelConfig] -- add your combo box handling code here..
		String presetName = comboBoxChannelConfig->getText();
		PresetInfo* preset = nullptr;
		for (PresetInfo* info : *pPresets)
		{
			if (info->getName() == presetName)
			{
				preset = info;
				break;
			}
		}
		if (preset != nullptr)
		{
			loadPreset(preset);
		}
		else
		{
			preset = new PresetInfo();

			int nbChannels = comboBoxChannelConfig->getText().getIntValue();
			if (nbChannels == 2)
			{
				ScopedPointer<Uuid> newId1 = new Uuid();
				ScopedPointer<Uuid> newId2 = new Uuid();
				preset->getPoints()->add(new AmbiPoint(newId1->toString(), Point3D<double>(0.0, -1.0, 0.0), "L", 0));
				preset->getPoints()->add(new AmbiPoint(newId2->toString(), Point3D<double>(0.0, 1.0, 0.0), "R", 0));
			}
			else
			{
				Point<float> projectedPoint(1.0, 0.0);
				projectedPoint = projectedPoint.rotatedAboutOrigin(-float(PI / nbChannels));
				for (int i = 0; i < nbChannels; i++)
				{
					ScopedPointer<Uuid> newId = new Uuid();
					preset->getPoints()->add(new AmbiPoint(newId->toString(), Point3D<double>(projectedPoint.getX(), projectedPoint.getY(), 0.0), String(i + 1), 0));
					projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI * 2 / nbChannels));
				}
			}
			preset->getAmbiSettings()->setDistanceScaler(DEFAULT_DISTANCE_SCALER);
			preset->getAmbiSettings()->setDirectionFlip(false);
			setInPhaseWeighting(preset->getAmbiSettings());
			loadPreset(preset);
			delete(preset);
		}
        //[/UserComboBoxCode_comboBoxChannelConfig]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SpeakerSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonLoad)
    {
        //[UserButtonCode_buttonLoad] -- add your button handler code here..
		ScopedPointer<FileChooser> fileChooser = new FileChooser("Load Preset", File(), "*.xml");
		if (fileChooser->browseForFileToOpen())
		{
			PresetInfo* preset = new PresetInfo();
			if (preset->LoadFromFile(fileChooser->getResult()))
			{
				if (CheckForExistingPreset(preset->getName()))
					return;
				loadPreset(preset);
				pPresets->add(preset);
				updateComboBox(preset->getName());
			}
		}
        //[/UserButtonCode_buttonLoad]
    }
    else if (buttonThatWasClicked == buttonSave)
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
		ScopedPointer<FileChooser> fileChooser = new FileChooser("Save Preset", File(), "*.xml");
		if (fileChooser->browseForFileToSave(true))
		{
			String newPresetName = fileChooser->getResult().getFileNameWithoutExtension();
			if (CheckForExistingPreset(newPresetName))
				return;

			PresetInfo* preset = new PresetInfo();
			preset->setName(newPresetName);
			for (AmbiPoint* pt : *pSpeakerArray)
				preset->getPoints()->add(new AmbiPoint(*pt));
			preset->getAmbiSettings()->setDistanceScaler(pAmbiSettings->getDistanceScaler());
			preset->getAmbiSettings()->setDirectionFlip(pAmbiSettings->getDirectionFlip());
			for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
				preset->getAmbiSettings()->getAmbiOrderWeightPointer()[i] = pAmbiSettings->getAmbiOrderWeightPointer()[i];
			preset->SaveToFile(fileChooser->getResult());

			pPresets->add(preset);
			updateComboBox(preset->getName());
		}
        //[/UserButtonCode_buttonSave]
    }
    else if (buttonThatWasClicked == buttonAdd)
    {
        //[UserButtonCode_buttonAdd] -- add your button handler code here..
		ScopedPointer<Uuid> newId = new Uuid();
		pSpeakerArray->add(new AmbiPoint(newId->toString(), Point3D<double>(0.0, 0.0, 0.0), "new", 0));
		pPointSelection->selectPoint(pSpeakerArray->size() - 1);
		speakerList->updateContent();
		speakerList->repaint();
        //[/UserButtonCode_buttonAdd]
    }
    else if (buttonThatWasClicked == buttonRemove)
    {
        //[UserButtonCode_buttonRemove] -- add your button handler code here..
		int selection = pPointSelection->getSelectedPointIndex();
		if (selection >= 0 && selection < pSpeakerArray->size())
		{
			pPointSelection->unselectPoint();
			pSpeakerArray->remove(selection);
			speakerList->updateContent();
			speakerList->repaint();
		}
        //[/UserButtonCode_buttonRemove]
    }
    else if (buttonThatWasClicked == buttonMoveDown)
    {
        //[UserButtonCode_buttonMoveDown] -- add your button handler code here..
		int selection = pPointSelection->getSelectedPointIndex();
		if(selection >= 0 && selection < pSpeakerArray->size() - 1)
		{
			pPointSelection->unselectPoint();
			pSpeakerArray->swap(selection, selection + 1);
			pPointSelection->selectPoint(selection + 1);
		}
        //[/UserButtonCode_buttonMoveDown]
    }
    else if (buttonThatWasClicked == buttonMoveUp)
    {
        //[UserButtonCode_buttonMoveUp] -- add your button handler code here..
		int selection = pPointSelection->getSelectedPointIndex();
		if (selection >= 1 && selection < pSpeakerArray->size())
		{
			pPointSelection->unselectPoint();
			pSpeakerArray->swap(selection, selection - 1);
			pPointSelection->selectPoint(selection - 1);
		}
        //[/UserButtonCode_buttonMoveUp]
    }
    else if (buttonThatWasClicked == buttonBasic)
    {
        //[UserButtonCode_buttonBasic] -- add your button handler code here..
		for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
			pAmbiSettings->getAmbiOrderWeightPointer()[i] = 1.0;
		ambiChannelControl->updateValues();
        //[/UserButtonCode_buttonBasic]
    }
    else if (buttonThatWasClicked == buttonInPhase)
    {
        //[UserButtonCode_buttonInPhase] -- add your button handler code here..
		setInPhaseWeighting(pAmbiSettings);
		ambiChannelControl->updateValues();
        //[/UserButtonCode_buttonInPhase]
    }
    else if (buttonThatWasClicked == btnFlipDirection)
    {
        //[UserButtonCode_btnFlipDirection] -- add your button handler code here..
		pAmbiSettings->setDirectionFlip(btnFlipDirection->getToggleState());
        //[/UserButtonCode_btnFlipDirection]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SpeakerSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderDistanceScaler)
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
void SpeakerSettingsComponent::showAsDialog(OwnedArray<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, ActionListener* pTestSoundListener)
{
	SpeakerSettingsComponent *p = new SpeakerSettingsComponent(pSpeakerArray, pPresets, pPointSelection, pAmbiSettings, pTestSoundListener);
	p->setSize(850, 600);

	DialogWindow::LaunchOptions options;
	options.content.setOwned(p);
	options.dialogTitle = "Speaker settings";
	options.dialogBackgroundColour = Colours::white;
	options.escapeKeyTriggersCloseButton = false;
	options.useNativeTitleBar = false;
	options.resizable = true;

	options.launchAsync();
}

int SpeakerSettingsComponent::getNumRows()
{
	return pSpeakerArray->size();
}

void SpeakerSettingsComponent::selectedRowsChanged(int lastRowSelected)
{
	if(lastRowSelected >= 0 && lastRowSelected < pSpeakerArray->size())
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
	g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
	String text;
	switch(columnId)
	{
	case COLUMN_ID_NB: text = String(rowNumber + 1); break;
	case COLUMN_ID_NAME: text = pSpeakerArray->getUnchecked(rowNumber)->getName(); break;
	case COLUMN_ID_DISTANCE: text = String(pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getDistance() * pAmbiSettings->getDistanceScaler(), 2); break;
	case COLUMN_ID_DELAY: text = String(delayHelper.getTotalDelayMs(pAmbiSettings, pSpeakerArray, rowNumber), 2); break;
	case COLUMN_ID_DELAY_COMPENSATION: text = String(delayHelper.getDelayCompensationMs(pAmbiSettings, pSpeakerArray, rowNumber), 2); break;
	default: text = "";
	}
	g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

Component* SpeakerSettingsComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate)
{
	if(columnId == COLUMN_ID_GAIN
		|| columnId == COLUMN_ID_X
		|| columnId == COLUMN_ID_Y
		|| columnId == COLUMN_ID_Z
		|| columnId == COLUMN_ID_A
		|| columnId == COLUMN_ID_E
		|| columnId == COLUMN_ID_D
		|| columnId == COLUMN_ID_DISTANCE)
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

	return nullptr;
}

String SpeakerSettingsComponent::getTableText(const int columnId, const int rowNumber) const
{
	switch(columnId)
	{
	case COLUMN_ID_NAME: return pSpeakerArray->getUnchecked(rowNumber)->getName();
	default: return "";
	}
}

void SpeakerSettingsComponent::setTableText(const int columnId, const int rowNumber, const String& newText) const
{
	switch(columnId)
	{
	case COLUMN_ID_NAME: pSpeakerArray->getUnchecked(rowNumber)->setName(newText); break;
	default: throw;
	}
}

void SpeakerSettingsComponent::setValue(int columnId, int rowNumber, double newValue) const
{
	switch (columnId)
	{
	case COLUMN_ID_GAIN: pSpeakerArray->getUnchecked(rowNumber)->setGain(newValue); break;
	case COLUMN_ID_X: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setX(newValue); break;
	case COLUMN_ID_Y: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setY(newValue); break;
	case COLUMN_ID_Z: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setZ(newValue); break;
	case COLUMN_ID_A: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setAzimuth(newValue * PI / 180.0); break;
	case COLUMN_ID_E: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setElevation(newValue * PI / 180.0); break;
	case COLUMN_ID_D: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setDistance(newValue); break;
	case COLUMN_ID_DISTANCE: pSpeakerArray->getUnchecked(rowNumber)->getPoint()->setDistance(newValue / pAmbiSettings->getDistanceScaler()); break;
	default: throw;
	}

	speakerList->updateContent();
	speakerList->repaint();
}

double SpeakerSettingsComponent::getValue(int columnId, int rowNumber) const
{
	switch (columnId)
	{
	case COLUMN_ID_GAIN: return pSpeakerArray->getUnchecked(rowNumber)->getGain();
	case COLUMN_ID_X: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getX();
	case COLUMN_ID_Y: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getY();
	case COLUMN_ID_Z: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getZ();
	case COLUMN_ID_A: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getAzimuth() * 180.0 / PI;
	case COLUMN_ID_E: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getElevation() * 180.0 / PI;
	case COLUMN_ID_D: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getDistance();
	case COLUMN_ID_DISTANCE: return pSpeakerArray->getUnchecked(rowNumber)->getPoint()->getDistance() * pAmbiSettings->getDistanceScaler();
	default: return 0.0;
	}
}

void SpeakerSettingsComponent::speakerTest(int rowNumber) const
{
	sendActionMessage(String(pSpeakerArray->getUnchecked(rowNumber)->getGain()) + ";" + String(rowNumber));
}

TableListBox* SpeakerSettingsComponent::getTable() const
{
	return speakerList;
}

SliderRange SpeakerSettingsComponent::getSliderRange(int columnId) const
{
	switch(columnId)
	{
	case COLUMN_ID_X:
	case COLUMN_ID_Y:
		return SliderRange(-1.0, 1.0, 0.001);

	case COLUMN_ID_Z:
	case COLUMN_ID_D:
		return SliderRange(0.0, 1.0, 0.001);

	case COLUMN_ID_A:
		return SliderRange(-360.0, 360.0, 0.1);

	case COLUMN_ID_E:
		return SliderRange(0.0, 180.0, 0.1);

	case COLUMN_ID_GAIN:
		return SliderRange(0.0, 1.2, 0.001);

	case COLUMN_ID_DISTANCE:
		return SliderRange(0.0, pAmbiSettings->getDistanceScaler(), 0.001);
	}

	return SliderRange(0.0, 1.0, 0.001);
}

void SpeakerSettingsComponent::updateDirectionFlip() const
{
	btnFlipDirection->setToggleState(pAmbiSettings->getDirectionFlip(), dontSendNotification);
}

void SpeakerSettingsComponent::updateDistanceScaler() const
{
	sliderDistanceScaler->setValue(pAmbiSettings->getDistanceScaler());
}

int SpeakerSettingsComponent::fact(int n)
{
	if (n == 0)
		return 1;
	int ret = n;
	for (int i = n - 1; i > 1; i--)
		ret *= i;
	return ret;
}

void SpeakerSettingsComponent::loadPreset(PresetInfo* preset) const
{
	pSpeakerArray->clear();
	for (AmbiPoint* pt : *preset->getPoints())
	{
		pSpeakerArray->add(new AmbiPoint(*pt));
	}
	speakerList->updateContent();
	speakerList->repaint();

	pAmbiSettings->setDistanceScaler(preset->getAmbiSettings()->getDistanceScaler());
	updateDistanceScaler();
	pAmbiSettings->setDirectionFlip(preset->getAmbiSettings()->getDirectionFlip());
	updateDirectionFlip();
	for(int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
	{
		pAmbiSettings->getAmbiOrderWeightPointer()[i] = preset->getAmbiSettings()->getAmbiOrderWeightPointer()[i];
	}
	ambiChannelControl->updateValues();
}

void SpeakerSettingsComponent::updateComboBox(String elementToSelect) const
{
	comboBoxChannelConfig->clear();
	int i = 1;
	for (PresetInfo* preset : *pPresets)
	{
		comboBoxChannelConfig->addItem(preset->getName(), i++);
	}
	comboBoxChannelConfig->addItem("2", i++);
	comboBoxChannelConfig->addItem("4", i++);
	comboBoxChannelConfig->addItem("6", i++);
	comboBoxChannelConfig->addItem("8", i++);
	comboBoxChannelConfig->addItem("16", i++);
	comboBoxChannelConfig->addItem("32", i++);

	for (i = 0; i < comboBoxChannelConfig->getNumItems(); i++)
	{
		if (comboBoxChannelConfig->getItemText(i) == elementToSelect)
		{
			comboBoxChannelConfig->setSelectedItemIndex(i);
			break;
		}
	}
}

void SpeakerSettingsComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == pPointSelection)
	{
		speakerList->updateContent();
		speakerList->repaint();
		speakerList->selectRow(pPointSelection->getSelectedPointIndex());
	}
}

bool SpeakerSettingsComponent::CheckForExistingPreset(String newPresetName) const
{
	// check for existing presets
	for (int i = 0; i < pPresets->size(); i++)
	{
		if (pPresets->getUnchecked(i)->getName() == newPresetName)
		{
			int ret = AlertWindow::showYesNoCancelBox(AlertWindow::AlertIconType::QuestionIcon, "Existing preset", "Replace existing preset " + newPresetName + "?");
			if (ret == 1) // YES
			{
				pPresets->remove(i);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}
void SpeakerSettingsComponent::setInPhaseWeighting(AmbiSettings* pSettings) const
{
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
	{
		if (i < CURRENT_AMBISONICS_ORDER_NB_OF_GAINS)
		{
			double nom = fact(CURRENT_AMBISONICS_ORDER) * fact(CURRENT_AMBISONICS_ORDER + 1);
			double denom = fact(CURRENT_AMBISONICS_ORDER + i + 1)*fact(CURRENT_AMBISONICS_ORDER - i);
			pSettings->getAmbiOrderWeightPointer()[i] = nom / denom;
		}
		else
		{
			pSettings->getAmbiOrderWeightPointer()[i] = 0.0;
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
                 componentName="" parentClasses="public Component, public TableListBoxModel, public ChangeListener, public ActionBroadcaster"
                 constructorParams="OwnedArray&lt;AmbiPoint&gt;* pSpeakerArray, OwnedArray&lt;PresetInfo&gt;* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings, ActionListener* pTestSoundListener"
                 variableInitialisers="pSpeakerArray(pSpeakerArray), pPresets(pPresets), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="800" initialHeight="800">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupAmbisonics" id="17eb4b418501687a" memberName="groupAmbisonics"
                  virtualName="" explicitFocusOrder="0" pos="8 198R 18M 192" title="Ambisonics"/>
  <GROUPCOMPONENT name="groupSpeakers" id="450188aa0f332e78" memberName="groupSpeakers"
                  virtualName="" explicitFocusOrder="0" pos="8 40 18M 246M" title="Speakers"/>
  <COMBOBOX name="channelConfig" id="4b25adf5b07e9492" memberName="comboBoxChannelConfig"
            virtualName="" explicitFocusOrder="0" pos="80 8 186M 24" editable="0"
            layout="33" items="" textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="107b43efebb2a5c8" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 64 24" edTextCol="ff000000" edBkgCol="0"
         labelText="Presets:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonLoad" id="5a786eb91323df32" memberName="buttonLoad"
              virtualName="" explicitFocusOrder="0" pos="98R 8 40 24" buttonText="load"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="50R 8 40 24" buttonText="save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="speakerList" id="34ae3e87c64e62da" memberName="speakerList"
                    virtualName="" explicitFocusOrder="0" pos="16 16 32M 56M" posRelativeX="450188aa0f332e78"
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
          virtualName="" explicitFocusOrder="0" pos="144 16 178M 24" posRelativeX="17eb4b418501687a"
          posRelativeY="17eb4b418501687a" min="1.00000000000000000000"
          max="500.00000000000000000000" int="0.10000000000000000555" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <GENERICCOMPONENT name="ambiChannelControl" id="4ec5a32a175ea48d" memberName="ambiChannelControl"
                    virtualName="" explicitFocusOrder="0" pos="144 40 160M 56M" posRelativeX="17eb4b418501687a"
                    posRelativeY="17eb4b418501687a" posRelativeW="17eb4b418501687a"
                    posRelativeH="17eb4b418501687a" class="MultiSliderControl" params="CURRENT_AMBISONICS_ORDER_NB_OF_GAINS, pAmbiSettings-&gt;getAmbiOrderWeightPointer(), &amp;ambiChannelNames, 0.0, 1.5, 0.01"/>
  <TEXTBUTTON name="buttonBasic" id="d53e1a131a389be4" memberName="buttonBasic"
              virtualName="" explicitFocusOrder="0" pos="12 64 120 24" posRelativeX="17eb4b418501687a"
              posRelativeY="17eb4b418501687a" buttonText="reset to basic" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelChannelWeights" id="ce2f83213d847908" memberName="labelChannelWeights"
         virtualName="" explicitFocusOrder="0" pos="8 40 112 24" posRelativeX="17eb4b418501687a"
         posRelativeY="17eb4b418501687a" edTextCol="ff000000" edBkgCol="0"
         labelText="Channel weights" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonInPhase" id="434ed99be63f9ea5" memberName="buttonInPhase"
              virtualName="" explicitFocusOrder="0" pos="12 96 120 24" posRelativeX="17eb4b418501687a"
              posRelativeY="17eb4b418501687a" buttonText="calculate in-phase"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="btnFlipDirection" id="b6567f77e6a2e40e" memberName="btnFlipDirection"
                virtualName="" explicitFocusOrder="0" pos="12 128 120 24" posRelativeX="17eb4b418501687a"
                posRelativeY="17eb4b418501687a" buttonText="Flip Direction" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelDistanceScaler" id="bbbf87bcedfbda85" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="8 15 150 24" posRelativeX="17eb4b418501687a"
         posRelativeY="17eb4b418501687a" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance scaler" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
