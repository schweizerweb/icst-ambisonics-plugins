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

//[Headers] You can add your own extra header files here...
#include "PresetInfo.h"
#include "EditableTextCustomComponent.h"
#include "GainColumnCustomComponent.h"
#include "SpeakerTestCustomComponent.h"
//[/Headers]

#include "SpeakerSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define COLUMN_ID_NB		1
#define COLUMN_ID_NAME		2
#define COLUMN_ID_DISTANCE	3
#define COLUMN_ID_DELAY		4
#define COLUMN_ID_GAIN		5
#define	COLUMN_ID_TEST		6
//[/MiscUserDefs]

//==============================================================================
SpeakerSettingsComponent::SpeakerSettingsComponent (Array<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings)
    : pSpeakerArray(pSpeakerArray), pPresets(pPresets), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

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

    addAndMakeVisible (groupAmbisonics = new GroupComponent ("groupAmbisonics",
                                                             TRANS("Ambisonics")));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Distance scaler")));
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderDistanceScaler = new Slider ("sliderDistanceScaler"));
    sliderDistanceScaler->setRange (1, 500, 0.1);
    sliderDistanceScaler->setSliderStyle (Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 700);


    //[Constructor] You can add your own custom stuff here..
	speakerList->setModel(this);
	speakerList->getHeader().addColumn("CH", COLUMN_ID_NB, 30);
	speakerList->getHeader().addColumn("Name", COLUMN_ID_NAME, 100);
	speakerList->getHeader().addColumn("Distance [m]", COLUMN_ID_DISTANCE, 100);
	speakerList->getHeader().addColumn("Delay [ms]", COLUMN_ID_DELAY, 100);
	speakerList->getHeader().addColumn("Gain", COLUMN_ID_GAIN, 100);
	speakerList->getHeader().addColumn("Test", COLUMN_ID_TEST, 30);
	speakerList->getHeader().resizeAllColumnsToFit(getWidth());
	updateComboBox();
	pPointSelection->addChangeListener(this);
	updateDistanceScaler();
    //[/Constructor]
}

SpeakerSettingsComponent::~SpeakerSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	pPointSelection->removeChangeListener(this);
    //[/Destructor_pre]

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
    groupAmbisonics = nullptr;
    label2 = nullptr;
    sliderDistanceScaler = nullptr;


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

    groupSpeakers->setBounds (8, 40, getWidth() - 18, getHeight() - 118);
    comboBoxChannelConfig->setBounds (80, 8, getWidth() - 186, 24);
    label->setBounds (8, 8, 64, 24);
    buttonLoad->setBounds (getWidth() - 98, 8, 40, 24);
    buttonSave->setBounds (getWidth() - 50, 8, 40, 24);
    speakerList->setBounds (8 + 8, 40 + 16, (getWidth() - 18) - 16, (getHeight() - 118) - 56);
    buttonAdd->setBounds ((8 + 8) + 0, (40 + 16) + ((getHeight() - 118) - 56) - -8, 64, 24);
    buttonRemove->setBounds ((8 + 8) + 72, (40 + 16) + ((getHeight() - 118) - 56) - -8, 64, 24);
    buttonMoveDown->setBounds ((8 + 8) + ((getWidth() - 18) - 16) - 64, (40 + 16) + ((getHeight() - 118) - 56) - -8, 64, 24);
    buttonMoveUp->setBounds ((8 + 8) + ((getWidth() - 18) - 16) - 136, (40 + 16) + ((getHeight() - 118) - 56) - -8, 64, 24);
    groupAmbisonics->setBounds (8, getHeight() - 70, 512, 64);
    label2->setBounds (8 + 16, (getHeight() - 70) + 24, 104, 24);
    sliderDistanceScaler->setBounds (8 + 128, (getHeight() - 70) + 24, 368, 24);
    //[UserResized] Add your own custom resize handling here..
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
		PresetInfo* namedPreset = nullptr;
		for (PresetInfo* info : *pPresets)
		{
			if (info->getName() == presetName)
			{
				namedPreset = info;
				break;
			}
		}

		if (namedPreset != nullptr)
		{
			loadPreset(namedPreset);
		}
		else
		{
			// standard presets
			int nbChannels = comboBoxChannelConfig->getText().getIntValue();

			pSpeakerArray->clear();
			if (nbChannels == 2)
			{
				pSpeakerArray->add(AmbiPoint(Point3D<double>(0.7, -0.7, 0.0), "L", 0));
				pSpeakerArray->add(AmbiPoint(Point3D<double>(0.7, 0.7, 0.0), "R", 0));
			}
			else
			{
				Point<float> projectedPoint(1.0, 0.0);
				projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI / nbChannels));
				for (int i = 0; i < nbChannels; i++)
				{
					pSpeakerArray->add(AmbiPoint(Point3D<double>(projectedPoint.getX(), projectedPoint.getY(), 0.0), String(i + 1), 0));
					projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI * 2 / nbChannels));
				}
			}
			speakerList->updateContent();
			speakerList->repaint();
			pAmbiSettings->setDistanceScaler(DEFAULT_DISTANCE_SCALER);
			updateDistanceScaler();
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
			for (AmbiPoint pt : *pSpeakerArray)
				preset->getPoints()->add(new AmbiPoint(pt));
			preset->getAmbiSettings()->setDistanceScaler(pAmbiSettings->getDistanceScaler());
			preset->SaveToFile(fileChooser->getResult());

			pPresets->add(preset);
			updateComboBox(preset->getName());
		}
        //[/UserButtonCode_buttonSave]
    }
    else if (buttonThatWasClicked == buttonAdd)
    {
        //[UserButtonCode_buttonAdd] -- add your button handler code here..
		pSpeakerArray->add(AmbiPoint(Point3D<double>(0.0, 0.0, 0.0), "new", 0));
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
			AmbiPoint bak = pSpeakerArray->getReference(selection);
			pSpeakerArray->getReference(selection) = pSpeakerArray->getReference(selection + 1);
			pSpeakerArray->getReference(selection + 1) = bak;
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
			AmbiPoint bak = pSpeakerArray->getReference(selection);
			pSpeakerArray->getReference(selection) = pSpeakerArray->getReference(selection - 1);
			pSpeakerArray->getReference(selection - 1) = bak;
			pPointSelection->selectPoint(selection - 1);
		}
        //[/UserButtonCode_buttonMoveUp]
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
		speakerList->repaint();
        //[/UserSliderCode_sliderDistanceScaler]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpeakerSettingsComponent::showAsDialog(Array<AmbiPoint>* pSpeakerArray, OwnedArray<PresetInfo>* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings)
{
	SpeakerSettingsComponent *p = new SpeakerSettingsComponent(pSpeakerArray, pPresets, pPointSelection, pAmbiSettings);
	p->setSize(800, 700);

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

void SpeakerSettingsComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	const Colour alternateColour(getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
}

void SpeakerSettingsComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
	String text;
	switch(columnId)
	{
	case COLUMN_ID_NB: text = String(rowNumber); break;
	case COLUMN_ID_NAME: text = pSpeakerArray->getReference(rowNumber).getName(); break;
	case COLUMN_ID_DISTANCE: text = String(pSpeakerArray->getReference(rowNumber).getPoint()->getDistance() * pAmbiSettings->getDistanceScaler(), 2); break;
	case COLUMN_ID_DELAY: text = String(pSpeakerArray->getReference(rowNumber).getPoint()->getDistance() * pAmbiSettings->getDistanceScaler() * SOUND_SPEED_M_PER_MS, 2); break;
	case COLUMN_ID_GAIN: text = String(pSpeakerArray->getReference(rowNumber).getGain(), 2); break;
	default: text = "";
	}
	g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

Component* SpeakerSettingsComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate)
{
	if(columnId == COLUMN_ID_GAIN)
	{
		GainColumnCustomComponent* gainBox = static_cast<GainColumnCustomComponent*> (existingComponentToUpdate);
		if (gainBox == nullptr)
			gainBox = new GainColumnCustomComponent(*this);

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
	case COLUMN_ID_NAME: return pSpeakerArray->getReference(rowNumber).getName();
	case COLUMN_ID_GAIN: return String(pSpeakerArray->getReference(rowNumber).getGain(), 3);
	default: return "";
	}
}

void SpeakerSettingsComponent::setTableText(const int columnId, const int rowNumber, const String& newText) const
{
	switch(columnId)
	{
	case COLUMN_ID_NAME: pSpeakerArray->getReference(rowNumber).setName(newText); break;
	case COLUMN_ID_GAIN: pSpeakerArray->getReference(rowNumber).setGain(newText.getDoubleValue()); break;
	default: throw;
	}
}

void SpeakerSettingsComponent::setGain(int rowNumber, double newValue) const
{
	pSpeakerArray->getReference(rowNumber).setGain(newValue);
}

double SpeakerSettingsComponent::getGain(int rowNumber) const
{
	return pSpeakerArray->getReference(rowNumber).getGain();
}

void SpeakerSettingsComponent::speakerTest(int rowNumber) const
{
	AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Speaker Test", "Test sound for channel " + String(rowNumber) + " - Not implemented yet!");
}

TableListBox* SpeakerSettingsComponent::getTable() const
{
	return speakerList;
}

void SpeakerSettingsComponent::updateDistanceScaler() const
{
	sliderDistanceScaler->setValue(pAmbiSettings->getDistanceScaler());
}

void SpeakerSettingsComponent::loadPreset(PresetInfo* preset) const
{
	pSpeakerArray->clear();
	for (AmbiPoint* pt : *preset->getPoints())
	{
		pSpeakerArray->add(AmbiPoint(*pt));
	}
	speakerList->updateContent();
	speakerList->repaint();

	pAmbiSettings->setDistanceScaler(preset->getAmbiSettings()->getDistanceScaler());
	updateDistanceScaler();
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

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpeakerSettingsComponent"
                 componentName="" parentClasses="public Component, public TableListBoxModel, public ChangeListener"
                 constructorParams="Array&lt;AmbiPoint&gt;* pSpeakerArray, OwnedArray&lt;PresetInfo&gt;* pPresets, PointSelection* pPointSelection, AmbiSettings* pAmbiSettings"
                 variableInitialisers="pSpeakerArray(pSpeakerArray), pPresets(pPresets), pPointSelection(pPointSelection), pAmbiSettings(pAmbiSettings)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="800" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupSpeakers" id="450188aa0f332e78" memberName="groupSpeakers"
                  virtualName="" explicitFocusOrder="0" pos="8 40 18M 118M" title="Speakers"/>
  <COMBOBOX name="channelConfig" id="4b25adf5b07e9492" memberName="comboBoxChannelConfig"
            virtualName="" explicitFocusOrder="0" pos="80 8 186M 24" editable="0"
            layout="33" items="" textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="107b43efebb2a5c8" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 64 24" edTextCol="ff000000" edBkgCol="0"
         labelText="Presets:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonLoad" id="5a786eb91323df32" memberName="buttonLoad"
              virtualName="" explicitFocusOrder="0" pos="98R 8 40 24" buttonText="load"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="50R 8 40 24" buttonText="save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="speakerList" id="34ae3e87c64e62da" memberName="speakerList"
                    virtualName="" explicitFocusOrder="0" pos="8 16 16M 56M" posRelativeX="450188aa0f332e78"
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
  <GROUPCOMPONENT name="groupAmbisonics" id="17eb4b418501687a" memberName="groupAmbisonics"
                  virtualName="" explicitFocusOrder="0" pos="8 70R 512 64" title="Ambisonics"/>
  <LABEL name="new label" id="b7b6f80386dfdff3" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="16 24 104 24" posRelativeX="17eb4b418501687a"
         posRelativeY="17eb4b418501687a" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance scaler" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderDistanceScaler" id="8ae6ec5973e2470e" memberName="sliderDistanceScaler"
          virtualName="" explicitFocusOrder="0" pos="128 24 368 24" posRelativeX="17eb4b418501687a"
          posRelativeY="17eb4b418501687a" min="1" max="500" int="0.10000000000000000555"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
