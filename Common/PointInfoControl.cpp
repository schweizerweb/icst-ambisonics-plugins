/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "RadarOptions.h"
#include "TrackColors.h"
#include "Constants.h"
//[/Headers]

#include "PointInfoControl.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PointInfoControl::PointInfoControl (AmbiDataSet* pEditablePoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions)
    : pEditablePoints(pEditablePoints), pPointSelection(pPointSelection), pRadarOptions(pRadarOptions)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textName.reset (new juce::TextEditor ("textName"));
    addAndMakeVisible (textName.get());
    textName->setMultiLine (false);
    textName->setReturnKeyStartsNewLine (false);
    textName->setReadOnly (true);
    textName->setScrollbarsShown (true);
    textName->setCaretVisible (false);
    textName->setPopupMenuEnabled (true);
    textName->setText (juce::String());

    labelName.reset (new juce::Label ("labelName",
                                      TRANS("Name:")));
    addAndMakeVisible (labelName.get());
    labelName->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelName->setJustificationType (juce::Justification::centredRight);
    labelName->setEditable (false, false, false);
    labelName->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelName->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelX.reset (new juce::Label ("labelX",
                                   TRANS("X:")));
    addAndMakeVisible (labelX.get());
    labelX->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelX->setJustificationType (juce::Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textX.reset (new juce::TextEditor ("textX"));
    addAndMakeVisible (textX.get());
    textX->setMultiLine (false);
    textX->setReturnKeyStartsNewLine (false);
    textX->setReadOnly (false);
    textX->setScrollbarsShown (true);
    textX->setCaretVisible (true);
    textX->setPopupMenuEnabled (true);
    textX->setText (juce::String());

    labelY.reset (new juce::Label ("labelY",
                                   TRANS("Y:")));
    addAndMakeVisible (labelY.get());
    labelY->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelY->setJustificationType (juce::Justification::centredLeft);
    labelY->setEditable (false, false, false);
    labelY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textY.reset (new juce::TextEditor ("textY"));
    addAndMakeVisible (textY.get());
    textY->setMultiLine (false);
    textY->setReturnKeyStartsNewLine (false);
    textY->setReadOnly (false);
    textY->setScrollbarsShown (true);
    textY->setCaretVisible (true);
    textY->setPopupMenuEnabled (true);
    textY->setText (juce::String());

    labelZ.reset (new juce::Label ("labelZ",
                                   TRANS("Z:")));
    addAndMakeVisible (labelZ.get());
    labelZ->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelZ->setJustificationType (juce::Justification::centredLeft);
    labelZ->setEditable (false, false, false);
    labelZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textZ.reset (new juce::TextEditor ("textZ"));
    addAndMakeVisible (textZ.get());
    textZ->setMultiLine (false);
    textZ->setReturnKeyStartsNewLine (false);
    textZ->setReadOnly (false);
    textZ->setScrollbarsShown (true);
    textZ->setCaretVisible (true);
    textZ->setPopupMenuEnabled (true);
    textZ->setText (juce::String());

    labelCartesian.reset (new juce::Label ("labelCartesian",
                                           TRANS("Cartesian:")));
    addAndMakeVisible (labelCartesian.get());
    labelCartesian->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCartesian->setJustificationType (juce::Justification::centredLeft);
    labelCartesian->setEditable (false, false, false);
    labelCartesian->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCartesian->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCartesian->setBounds (0, 40, 80, 24);

    labelA.reset (new juce::Label ("labelA",
                                   TRANS("A:")));
    addAndMakeVisible (labelA.get());
    labelA->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelA->setJustificationType (juce::Justification::centredLeft);
    labelA->setEditable (false, false, false);
    labelA->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelA->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textA.reset (new juce::TextEditor ("textA"));
    addAndMakeVisible (textA.get());
    textA->setMultiLine (false);
    textA->setReturnKeyStartsNewLine (false);
    textA->setReadOnly (false);
    textA->setScrollbarsShown (true);
    textA->setCaretVisible (true);
    textA->setPopupMenuEnabled (true);
    textA->setText (juce::String());

    labelE.reset (new juce::Label ("labelE",
                                   TRANS("E:")));
    addAndMakeVisible (labelE.get());
    labelE->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelE->setJustificationType (juce::Justification::centredLeft);
    labelE->setEditable (false, false, false);
    labelE->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelE->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textE.reset (new juce::TextEditor ("textE"));
    addAndMakeVisible (textE.get());
    textE->setMultiLine (false);
    textE->setReturnKeyStartsNewLine (false);
    textE->setReadOnly (false);
    textE->setScrollbarsShown (true);
    textE->setCaretVisible (true);
    textE->setPopupMenuEnabled (true);
    textE->setText (juce::String());

    labelD.reset (new juce::Label ("labelD",
                                   TRANS("D:")));
    addAndMakeVisible (labelD.get());
    labelD->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelD->setJustificationType (juce::Justification::centredLeft);
    labelD->setEditable (false, false, false);
    labelD->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelD->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textD.reset (new juce::TextEditor ("textD"));
    addAndMakeVisible (textD.get());
    textD->setMultiLine (false);
    textD->setReturnKeyStartsNewLine (false);
    textD->setReadOnly (false);
    textD->setScrollbarsShown (true);
    textD->setCaretVisible (true);
    textD->setPopupMenuEnabled (true);
    textD->setText (juce::String());

    labelPolar.reset (new juce::Label ("labelPolar",
                                       TRANS("Polar:")));
    addAndMakeVisible (labelPolar.get());
    labelPolar->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPolar->setJustificationType (juce::Justification::centredLeft);
    labelPolar->setEditable (false, false, false);
    labelPolar->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPolar->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPolar->setBounds (0, 72, 80, 24);

    labelCH.reset (new juce::Label ("labelCH",
                                    TRANS("CH:")));
    addAndMakeVisible (labelCH.get());
    labelCH->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCH->setJustificationType (juce::Justification::centredLeft);
    labelCH->setEditable (false, false, false);
    labelCH->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCH->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    textCH.reset (new juce::TextEditor ("textCH"));
    addAndMakeVisible (textCH.get());
    textCH->setMultiLine (false);
    textCH->setReturnKeyStartsNewLine (false);
    textCH->setReadOnly (true);
    textCH->setScrollbarsShown (true);
    textCH->setCaretVisible (false);
    textCH->setPopupMenuEnabled (true);
    textCH->setText (juce::String());

    btnGroupBackup.reset (new juce::ImageButton ("btnGroupBackup"));
    addAndMakeVisible (btnGroupBackup.get());
    btnGroupBackup->setButtonText (TRANS("group"));
    btnGroupBackup->addListener (this);

    btnGroupBackup->setImages (false, true, true,
                               juce::ImageCache::getFromMemory (group_png, group_pngSize), 1.000f, juce::Colour (0x00000000),
                               juce::Image(), 1.000f, juce::Colour (0xa0a01313),
                               juce::Image(), 1.000f, juce::Colour (0x00000000));
    btnGroupBackup->setBounds (56, 0, 36, 36);

    btnUngroupBackup.reset (new juce::ImageButton ("btnUngroupBackup"));
    addAndMakeVisible (btnUngroupBackup.get());
    btnUngroupBackup->setButtonText (TRANS("ungroup"));
    btnUngroupBackup->addListener (this);

    btnUngroupBackup->setImages (false, true, true,
                                 juce::ImageCache::getFromMemory (ungroup_png, ungroup_pngSize), 1.000f, juce::Colour (0x00000000),
                                 juce::Image(), 1.000f, juce::Colour (0xa0a01313),
                                 juce::Image(), 1.000f, juce::Colour (0x00000000));
    btnUngroupBackup->setBounds (56, 0, 36, 36);

    btnGroup.reset (new juce::TextButton ("btnGroup"));
    addAndMakeVisible (btnGroup.get());
    btnGroup->setButtonText (TRANS("group"));
    btnGroup->addListener (this);
    btnGroup->setColour (juce::TextButton::buttonColourId, juce::Colours::green);

    btnGroup->setBounds (8, 8, 64, 24);

    btnUngroup.reset (new juce::TextButton ("btnUngroup"));
    addAndMakeVisible (btnUngroup.get());
    btnUngroup->setButtonText (TRANS("ungroup"));
    btnUngroup->addListener (this);
    btnUngroup->setColour (juce::TextButton::buttonColourId, juce::Colours::crimson);

    btnUngroup->setBounds (8, 8, 64, 24);


    //[UserPreSize]
    btnGroupBackup->setVisible(false);
    btnUngroupBackup->setVisible(false);
	textName->setReadOnly(!pRadarOptions->nameFieldEditable);
    //[/UserPreSize]

    setSize (390, 100);


    //[Constructor] You can add your own custom stuff here..
	textCH->setVisible(pRadarOptions->maxNumberEditablePoints > 1);
	labelCH->setVisible(pRadarOptions->maxNumberEditablePoints > 1);
	updateSelectedPoint();
	pPointSelection->addChangeListener(this);
    //[/Constructor]
}

PointInfoControl::~PointInfoControl()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPointSelection->removeChangeListener(this);
    //[/Destructor_pre]

    textName = nullptr;
    labelName = nullptr;
    labelX = nullptr;
    textX = nullptr;
    labelY = nullptr;
    textY = nullptr;
    labelZ = nullptr;
    textZ = nullptr;
    labelCartesian = nullptr;
    labelA = nullptr;
    textA = nullptr;
    labelE = nullptr;
    textE = nullptr;
    labelD = nullptr;
    textD = nullptr;
    labelPolar = nullptr;
    labelCH = nullptr;
    textCH = nullptr;
    btnGroupBackup = nullptr;
    btnUngroupBackup = nullptr;
    btnGroup = nullptr;
    btnUngroup = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PointInfoControl::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PointInfoControl::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textName->setBounds ((proportionOfWidth (0.6051f) + 0) + 0, (8 + 0) + 0, proportionOfWidth (0.3590f), 24);
    labelName->setBounds (((proportionOfWidth (0.6051f) + 0) + 0) + 0 - 56, (8 + 0) + 0, 56, 24);
    labelX->setBounds ((proportionOfWidth (0.3487f) + 0) + 0 - 24, 40 + 0, 24, 24);
    textX->setBounds (proportionOfWidth (0.3487f) + 0, 40 + 0, juce::roundToInt ((juce::roundToInt (proportionOfWidth (0.1359f) * 1.0000f)) * 1.0000f), 24);
    labelY->setBounds ((proportionOfWidth (0.6051f) + 0) + 0 - 24, 40 + 0, 24, 24);
    textY->setBounds (proportionOfWidth (0.6051f) + 0, 40 + 0, juce::roundToInt ((juce::roundToInt (proportionOfWidth (0.1359f) * 1.0000f)) * 1.0000f), 24);
    labelZ->setBounds ((proportionOfWidth (0.8410f) + 0) + 0 - 24, 40 + 0, 24, 24);
    textZ->setBounds (proportionOfWidth (0.8410f) + 0, 40 + 0, juce::roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    labelA->setBounds (proportionOfWidth (0.3487f) + 0 - 24, 72 + 0, 24, 24);
    textA->setBounds (proportionOfWidth (0.3487f), 72 + 0, juce::roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    labelE->setBounds (proportionOfWidth (0.6051f) + 0 - 24, 72 + 0, 24, 24);
    textE->setBounds (proportionOfWidth (0.6051f), 72 + 0, juce::roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    labelD->setBounds (proportionOfWidth (0.8410f) + 0 - 24, 72 + 0, 24, 24);
    textD->setBounds (proportionOfWidth (0.8410f), 72 + 0, proportionOfWidth (0.1359f), 24);
    labelCH->setBounds (((proportionOfWidth (0.3487f) + 0) + 0) + 0 - 32, 8 + 0, 32, 24);
    textCH->setBounds ((proportionOfWidth (0.3487f) + 0) + 0, 8, juce::roundToInt (proportionOfWidth (0.1359f) * 0.4528f), 24);
    //[UserResized] Add your own custom resize handling here..

	// workaround for labelName
	textName->setBounds(textY->getX(), textCH->getY(), textZ->getRight() - textY->getX(), textCH->getHeight());

    //[/UserResized]
}

void PointInfoControl::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnGroupBackup.get())
    {
        //[UserButtonCode_btnGroupBackup] -- add your button handler code here..
        //[/UserButtonCode_btnGroupBackup]
    }
    else if (buttonThatWasClicked == btnUngroupBackup.get())
    {
        //[UserButtonCode_btnUngroupBackup] -- add your button handler code here..
        //[/UserButtonCode_btnUngroupBackup]
    }
    else if (buttonThatWasClicked == btnGroup.get())
    {
        //[UserButtonCode_btnGroup] -- add your button handler code here..
        Array<int> selection = pPointSelection->getSelectedIndices();
        if (pPointSelection->getSelectionMode() == PointSelection::Point && selection.size() > 1)
        {
            double minX = pRadarOptions->scalingInfo->CartesianMax();
            double maxX = pRadarOptions->scalingInfo->CartesianMin();
            double minY = pRadarOptions->scalingInfo->CartesianMax();
            double maxY = pRadarOptions->scalingInfo->CartesianMin();
            double minZ = pRadarOptions->scalingInfo->CartesianMax();
            double maxZ = pRadarOptions->scalingInfo->CartesianMin();

            for (int i : selection)
            {
                minX = jmin(minX, pEditablePoints->get(i)->getPoint()->getX());
                maxX = jmax(maxX, pEditablePoints->get(i)->getPoint()->getX());
                minY = jmin(minY, pEditablePoints->get(i)->getPoint()->getY());
                maxY = jmax(maxY, pEditablePoints->get(i)->getPoint()->getY());
                minZ = jmin(minZ, pEditablePoints->get(i)->getPoint()->getZ());
                maxZ = jmax(maxZ, pEditablePoints->get(i)->getPoint()->getZ());
            }
            Point<double> centerXY = Rectangle<double>(minX, minY, maxX - minX, maxY - minY).getCentre();
            double centerZ = (minZ + maxZ) / 2.0;

            AmbiGroup* g = pEditablePoints->addGroup(Uuid().toString(), Point3D<double>(centerXY.getX(), centerXY.getY(), centerZ, pRadarOptions->getAudioParamForIndex(pEditablePoints->groupCount(), true)), "G", Colours::orange);
            for (int i : selection)
            {
                g->addPointToGroup(pEditablePoints->get(i));
            }

            pPointSelection->selectGroup(pEditablePoints->groupCount() - 1, false);
        }
        //[/UserButtonCode_btnGroup]
    }
    else if (buttonThatWasClicked == btnUngroup.get())
    {
        //[UserButtonCode_btnUngroup] -- add your button handler code here..
        if (pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int indexToRemove = pPointSelection->getMainSelectedPointIndex();
            pPointSelection->unselectPoint();
            pEditablePoints->removeGroup(indexToRemove);
        }
        //[/UserButtonCode_btnUngroup]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PointInfoControl::updateSelectedPoint(String exceptField)
{
	disableListeners();

	int selection = pPointSelection->getMainSelectedPointIndex();
	if (pPointSelection->getSelectionMode() != PointSelection::None && selection >= 0 && selection < pEditablePoints->size())
	{
		AmbiPoint* point = pPointSelection->getSelectionMode() == PointSelection::Group ? pEditablePoints->getGroup(selection) : pEditablePoints->get(selection);

		setFieldsEnabled(true);
		textName->setText(point->getName());
		textCH->setText(String(selection + 1));
		if (exceptField != textX->getName()) textX->setText(String(point->getPoint()->getX(), 3));
		if (exceptField != textY->getName()) textY->setText(String(point->getPoint()->getY(), 3));
		if (exceptField != textZ->getName()) textZ->setText(String(point->getPoint()->getZ(), 3));
		if (exceptField != textA->getName()) textA->setText(String(Constants::RadToGrad(point->getPoint()->getAzimuth()), 2));
		if (exceptField != textE->getName()) textE->setText(String(Constants::RadToGrad(point->getPoint()->getElevation()), 2));
		if (exceptField != textD->getName()) textD->setText(String(point->getPoint()->getDistance(), 3));
	}
	else
	{
		setFieldsEnabled(false);
		textName->setText("-");
		textCH->setText("-");
		textX->setText("-");
		textY->setText("-");
		textZ->setText("-");
		textA->setText("-");
		textE->setText("-");
		textD->setText("-");
	}

	btnGroup->setVisible(pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getSelectedIndices().size() > 1
                         && ( pRadarOptions->audioParams == nullptr || pEditablePoints->groupCount() < pRadarOptions->audioParams->groupParams.size()));
	btnUngroup->setVisible(pPointSelection->getSelectionMode() == PointSelection::Group && pPointSelection->getSelectedIndices().size() == 1);
	textX->setReadOnly(pPointSelection->getSelectionMode() != PointSelection::Point);
	textY->setReadOnly(pPointSelection->getSelectionMode() != PointSelection::Point);
	textZ->setReadOnly(pPointSelection->getSelectionMode() != PointSelection::Point);
	textA->setReadOnly(pPointSelection->getSelectionMode() != PointSelection::Point);
	textE->setReadOnly(pPointSelection->getSelectionMode() != PointSelection::Point);
	textD->setReadOnly(pPointSelection->getSelectionMode() != PointSelection::Point);
	textName->setReadOnly(!pRadarOptions->nameFieldEditable || (pRadarOptions->dawParameter != nullptr && pRadarOptions->dawParameter->updateTrackPropertiesWorking));
	enableListeners();
}

void PointInfoControl::changeListenerCallback(ChangeBroadcaster* source)
{
	if(source == pPointSelection)
		updateSelectedPoint();
}

void PointInfoControl::textEditorTextChanged(TextEditor& source)
{
	int selection = pPointSelection->getMainSelectedPointIndex();

	if(pPointSelection->getSelectionMode() == PointSelection::Group)
	{
		if (source.getName() == textName->getName())
		{
			pEditablePoints->setGroupName(selection, textName->getText());
		}
	}
	else
	{
		if (source.getName() == textName->getName())
		{
			pEditablePoints->setChannelName(selection, textName->getText());
			if (pRadarOptions->setTrackColorAccordingToName)
			{
				pEditablePoints->setChannelColor(selection, TrackColors::getColor(textName->getText().initialSectionContainingOnly("0123456789").getIntValue()));
			}
		}

		if (source.getName() == textX->getName())
		{
			pEditablePoints->setX(selection, textX->getText().getFloatValue());
		}
		if (source.getName() == textY->getName())
		{
			pEditablePoints->setY(selection, textY->getText().getFloatValue());
		}
		if (source.getName() == textZ->getName())
		{
			pEditablePoints->setZ(selection, textZ->getText().getFloatValue());
		}

		if (source.getName() == textA->getName())
		{
			pEditablePoints->setAzimuth(selection, Constants::GradToRad(textA->getText().getFloatValue()));
		}
		if (source.getName() == textE->getName())
		{
			pEditablePoints->setElevation(selection, Constants::GradToRad(textE->getText().getFloatValue()));
		}
		if (source.getName() == textD->getName())
		{
			pEditablePoints->setDistance(selection, textD->getText().getFloatValue());
		}
	}

	updateSelectedPoint(source.getName());
}

void PointInfoControl::disableListeners()
{
	textName->removeListener(this);
	textX->removeListener(this);
	textY->removeListener(this);
	textZ->removeListener(this);
	textA->removeListener(this);
	textE->removeListener(this);
	textD->removeListener(this);
}

void PointInfoControl::enableListeners()
{
	textName->addListener(this);
	textX->addListener(this);
	textY->addListener(this);
	textZ->addListener(this);
	textA->addListener(this);
	textE->addListener(this);
	textD->addListener(this);
}

void PointInfoControl::setFieldsEnabled(bool enable) const
{
	textName->setEnabled(enable);
	textX->setEnabled(enable);
	textY->setEnabled(enable);
	textZ->setEnabled(enable);
	textA->setEnabled(enable);
	textE->setEnabled(enable);
	textD->setEnabled(enable);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PointInfoControl" componentName=""
                 parentClasses="public Component, public ChangeListener, public TextEditor::Listener"
                 constructorParams="AmbiDataSet* pEditablePoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions"
                 variableInitialisers="pEditablePoints(pEditablePoints), pPointSelection(pPointSelection), pRadarOptions(pRadarOptions)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="390" initialHeight="100">
  <BACKGROUND backgroundColour="ff505050"/>
  <TEXTEDITOR name="textName" id="2b706cdb3232f1d2" memberName="textName" virtualName=""
              explicitFocusOrder="0" pos="0 0 35.897% 24" posRelativeX="46b4702ec60c236a"
              posRelativeY="6db02d6b63396cba" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <LABEL name="labelName" id="7326de7683af8e2f" memberName="labelName"
         virtualName="" explicitFocusOrder="0" pos="0r 0 56 24" posRelativeX="2b706cdb3232f1d2"
         posRelativeY="6db02d6b63396cba" edTextCol="ff000000" edBkgCol="0"
         labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="labelX" id="6fb8ecbc6da5fa76" memberName="labelX" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="4dcd5fc970cdce6c"
         posRelativeY="f2af390154f0c032" edTextCol="ff000000" edBkgCol="0"
         labelText="X:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textX" id="4dcd5fc970cdce6c" memberName="textX" virtualName=""
              explicitFocusOrder="0" pos="0 0 100% 24" posRelativeX="e76184fec37afbfb"
              posRelativeY="f2af390154f0c032" posRelativeW="e76184fec37afbfb"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelY" id="d8cf1d21dc85896e" memberName="labelY" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="46b4702ec60c236a"
         posRelativeY="f2af390154f0c032" edTextCol="ff000000" edBkgCol="0"
         labelText="Y:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textY" id="46b4702ec60c236a" memberName="textY" virtualName=""
              explicitFocusOrder="0" pos="0 0 100% 24" posRelativeX="7fb39984576e614e"
              posRelativeY="f2af390154f0c032" posRelativeW="7fb39984576e614e"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelZ" id="385e50956f13effd" memberName="labelZ" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="93f9644500f2da7f"
         posRelativeY="f2af390154f0c032" edTextCol="ff000000" edBkgCol="0"
         labelText="Z:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textZ" id="93f9644500f2da7f" memberName="textZ" virtualName=""
              explicitFocusOrder="0" pos="0 0 100% 24" posRelativeX="411a4503627ed096"
              posRelativeY="f2af390154f0c032" posRelativeW="411a4503627ed096"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelCartesian" id="f2af390154f0c032" memberName="labelCartesian"
         virtualName="" explicitFocusOrder="0" pos="0 40 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cartesian:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelA" id="c8e81a0262f1ac25" memberName="labelA" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="e76184fec37afbfb"
         posRelativeY="41fcd21e0ce12407" edTextCol="ff000000" edBkgCol="0"
         labelText="A:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textA" id="e76184fec37afbfb" memberName="textA" virtualName=""
              explicitFocusOrder="0" pos="34.872% 0 100% 24" posRelativeY="41fcd21e0ce12407"
              posRelativeW="411a4503627ed096" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelE" id="6af5bb62c259b445" memberName="labelE" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="7fb39984576e614e"
         posRelativeY="41fcd21e0ce12407" edTextCol="ff000000" edBkgCol="0"
         labelText="E:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textE" id="7fb39984576e614e" memberName="textE" virtualName=""
              explicitFocusOrder="0" pos="60.513% 0 100% 24" posRelativeY="41fcd21e0ce12407"
              posRelativeW="411a4503627ed096" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelD" id="b1bd9596558e6f35" memberName="labelD" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="411a4503627ed096"
         posRelativeY="41fcd21e0ce12407" edTextCol="ff000000" edBkgCol="0"
         labelText="D:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textD" id="411a4503627ed096" memberName="textD" virtualName=""
              explicitFocusOrder="0" pos="84.103% 0 13.59% 24" posRelativeY="41fcd21e0ce12407"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelPolar" id="41fcd21e0ce12407" memberName="labelPolar"
         virtualName="" explicitFocusOrder="0" pos="0 72 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Polar:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCH" id="6db02d6b63396cba" memberName="labelCH" virtualName=""
         explicitFocusOrder="0" pos="0r 0 32 24" posRelativeX="d9bdcb7a01c25c33"
         posRelativeY="d9bdcb7a01c25c33" edTextCol="ff000000" edBkgCol="0"
         labelText="CH:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textCH" id="d9bdcb7a01c25c33" memberName="textCH" virtualName=""
              explicitFocusOrder="0" pos="0 8 45.283% 24" posRelativeX="4dcd5fc970cdce6c"
              posRelativeW="411a4503627ed096" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <IMAGEBUTTON name="btnGroupBackup" id="304aaa821d1b17f7" memberName="btnGroupBackup"
               virtualName="" explicitFocusOrder="0" pos="56 0 36 36" buttonText="group"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="group_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="a0a01313" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="btnUngroupBackup" id="6bdf324b7d527d88" memberName="btnUngroupBackup"
               virtualName="" explicitFocusOrder="0" pos="56 0 36 36" buttonText="ungroup"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="ungroup_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="a0a01313" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <TEXTBUTTON name="btnGroup" id="2f4a1f39797e6cb8" memberName="btnGroup" virtualName=""
              explicitFocusOrder="0" pos="8 8 64 24" bgColOff="ff008000" buttonText="group"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnUngroup" id="fd997826e7fd532" memberName="btnUngroup"
              virtualName="" explicitFocusOrder="0" pos="8 8 64 24" bgColOff="ffdc143c"
              buttonText="ungroup" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: group_png, 482, "../Resources/Group.png"
static const unsigned char resource_PointInfoControl_group_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,24,0,0,0,24,8,6,0,0,0,224,119,61,248,0,0,1,169,73,68,65,84,120,94,181,149,193,74,
195,64,16,134,103,76,136,55,189,74,239,125,9,95,160,52,82,240,33,90,168,80,240,98,139,167,30,234,69,72,105,161,189,104,159,194,147,245,212,67,31,193,107,223,160,13,68,197,170,77,211,174,195,176,176,139,
144,117,132,248,195,71,114,217,249,103,126,38,89,248,111,33,1,97,179,23,33,226,21,20,40,165,84,255,241,174,123,205,46,103,23,55,138,30,135,132,7,133,200,212,244,193,40,35,20,129,76,78,99,140,92,153,109,
176,111,52,26,151,203,229,114,152,166,41,216,242,60,15,74,165,82,111,50,153,220,2,64,74,236,64,166,157,109,0,171,213,106,24,69,17,148,203,101,176,181,88,44,160,221,110,119,1,224,158,72,136,47,2,156,147,
106,217,6,184,217,108,76,113,3,27,234,169,142,136,117,189,94,111,82,51,3,247,164,182,129,28,159,240,226,56,30,252,62,41,11,15,224,111,66,233,164,110,3,55,146,51,190,56,162,217,108,6,163,209,136,223,171,
213,234,51,104,213,106,53,104,181,90,80,169,84,114,38,21,26,140,199,99,112,228,205,6,226,113,131,32,224,131,182,4,121,139,178,98,146,36,57,239,116,58,15,84,20,228,146,27,168,249,124,254,68,207,19,226,
152,8,8,228,220,101,226,233,125,223,207,253,208,20,177,37,94,136,79,194,35,144,112,42,12,67,208,226,226,136,216,231,58,57,91,180,39,82,13,74,86,114,58,157,158,234,95,71,102,53,249,166,13,148,239,248,143,
40,16,137,139,199,196,135,110,112,103,77,160,220,29,154,205,114,229,157,17,107,226,85,243,206,166,174,239,64,176,89,118,222,169,54,217,51,90,78,3,247,102,177,240,71,222,91,19,169,196,192,189,89,70,38,
239,212,101,192,23,52,223,161,5,95,250,118,67,94,177,151,190,169,249,13,11,19,202,94,114,104,117,143,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PointInfoControl::group_png = (const char*) resource_PointInfoControl_group_png;
const int PointInfoControl::group_pngSize = 482;

// JUCER_RESOURCE: ungroup_png, 462, "../Resources/Ungroup.png"
static const unsigned char resource_PointInfoControl_ungroup_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,24,0,0,0,24,8,6,0,0,0,224,119,61,248,0,0,1,149,73,68,65,84,120,94,181,149,207,
78,194,64,16,198,191,110,87,52,141,122,86,145,35,15,83,53,225,45,236,205,11,129,35,7,15,94,56,225,133,224,83,144,16,77,76,120,19,94,0,244,104,81,91,105,235,180,89,157,148,205,166,127,8,191,228,75,152,
221,205,44,147,111,118,138,125,99,129,240,60,239,110,185,92,142,194,48,4,3,216,182,141,86,171,53,24,143,199,15,0,34,212,64,130,88,173,86,163,225,112,136,118,187,13,6,88,44,22,232,245,122,247,0,30,73,126,
221,75,224,186,110,146,50,1,50,17,185,24,192,57,233,128,227,12,152,99,70,96,139,219,36,201,253,158,186,238,255,57,15,176,82,129,48,197,216,66,106,201,75,98,242,110,74,186,177,237,228,207,59,137,154,148,
245,78,128,193,147,101,161,10,65,16,112,114,206,145,173,169,170,28,129,253,34,4,136,70,163,145,149,197,30,112,169,82,74,84,129,114,232,38,175,215,235,171,126,191,255,146,150,204,32,75,238,56,206,0,64,
140,234,240,5,243,249,252,21,192,201,4,248,240,128,38,242,237,22,145,62,139,31,25,123,192,85,112,155,198,42,73,202,59,116,162,186,85,88,48,80,102,62,117,58,157,141,169,77,187,221,46,102,179,217,165,220,
165,199,125,223,191,38,239,158,75,123,167,230,8,207,39,3,233,158,154,79,135,164,99,210,25,233,130,212,84,57,154,106,237,84,162,62,130,148,40,239,2,228,121,83,65,196,23,240,176,170,66,162,204,143,77,57,
4,246,140,208,60,216,145,130,239,129,58,192,143,38,147,22,243,217,112,251,131,83,250,29,80,143,39,69,61,174,204,252,41,24,21,58,37,231,211,87,253,151,204,123,71,234,79,8,189,123,176,33,125,23,141,144,
95,208,216,251,27,114,207,252,116,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PointInfoControl::ungroup_png = (const char*) resource_PointInfoControl_ungroup_png;
const int PointInfoControl::ungroup_pngSize = 462;


//[EndFile] You can add extra defines here...
//[/EndFile]

