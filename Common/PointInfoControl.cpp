/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
    : pEditablePoints(pEditablePoints), pPointSelection(pPointSelection)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textName.reset (new TextEditor ("textName"));
    addAndMakeVisible (textName.get());
    textName->setMultiLine (false);
    textName->setReturnKeyStartsNewLine (false);
    textName->setReadOnly (true);
    textName->setScrollbarsShown (true);
    textName->setCaretVisible (false);
    textName->setPopupMenuEnabled (true);
    textName->setText (String());

    labelName.reset (new Label ("labelName",
                                TRANS("Name:")));
    addAndMakeVisible (labelName.get());
    labelName->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelName->setJustificationType (Justification::centredLeft);
    labelName->setEditable (false, false, false);
    labelName->setColour (TextEditor::textColourId, Colours::black);
    labelName->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelName->setBounds (0, 8, 80, 24);

    labelX.reset (new Label ("labelX",
                             TRANS("X:")));
    addAndMakeVisible (labelX.get());
    labelX->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelX->setJustificationType (Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (TextEditor::textColourId, Colours::black);
    labelX->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textX.reset (new TextEditor ("textX"));
    addAndMakeVisible (textX.get());
    textX->setMultiLine (false);
    textX->setReturnKeyStartsNewLine (false);
    textX->setReadOnly (false);
    textX->setScrollbarsShown (true);
    textX->setCaretVisible (true);
    textX->setPopupMenuEnabled (true);
    textX->setText (String());

    labelY.reset (new Label ("labelY",
                             TRANS("Y:")));
    addAndMakeVisible (labelY.get());
    labelY->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelY->setJustificationType (Justification::centredLeft);
    labelY->setEditable (false, false, false);
    labelY->setColour (TextEditor::textColourId, Colours::black);
    labelY->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textY.reset (new TextEditor ("textY"));
    addAndMakeVisible (textY.get());
    textY->setMultiLine (false);
    textY->setReturnKeyStartsNewLine (false);
    textY->setReadOnly (false);
    textY->setScrollbarsShown (true);
    textY->setCaretVisible (true);
    textY->setPopupMenuEnabled (true);
    textY->setText (String());

    labelZ.reset (new Label ("labelZ",
                             TRANS("Z:")));
    addAndMakeVisible (labelZ.get());
    labelZ->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelZ->setJustificationType (Justification::centredLeft);
    labelZ->setEditable (false, false, false);
    labelZ->setColour (TextEditor::textColourId, Colours::black);
    labelZ->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textZ.reset (new TextEditor ("textZ"));
    addAndMakeVisible (textZ.get());
    textZ->setMultiLine (false);
    textZ->setReturnKeyStartsNewLine (false);
    textZ->setReadOnly (false);
    textZ->setScrollbarsShown (true);
    textZ->setCaretVisible (true);
    textZ->setPopupMenuEnabled (true);
    textZ->setText (String());

    labelCartesian.reset (new Label ("labelCartesian",
                                     TRANS("Cartesian:")));
    addAndMakeVisible (labelCartesian.get());
    labelCartesian->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCartesian->setJustificationType (Justification::centredLeft);
    labelCartesian->setEditable (false, false, false);
    labelCartesian->setColour (TextEditor::textColourId, Colours::black);
    labelCartesian->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelA.reset (new Label ("labelA",
                             TRANS("A:")));
    addAndMakeVisible (labelA.get());
    labelA->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelA->setJustificationType (Justification::centredLeft);
    labelA->setEditable (false, false, false);
    labelA->setColour (TextEditor::textColourId, Colours::black);
    labelA->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textA.reset (new TextEditor ("textA"));
    addAndMakeVisible (textA.get());
    textA->setMultiLine (false);
    textA->setReturnKeyStartsNewLine (false);
    textA->setReadOnly (false);
    textA->setScrollbarsShown (true);
    textA->setCaretVisible (true);
    textA->setPopupMenuEnabled (true);
    textA->setText (String());

    labelE.reset (new Label ("labelE",
                             TRANS("E:")));
    addAndMakeVisible (labelE.get());
    labelE->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelE->setJustificationType (Justification::centredLeft);
    labelE->setEditable (false, false, false);
    labelE->setColour (TextEditor::textColourId, Colours::black);
    labelE->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textE.reset (new TextEditor ("textE"));
    addAndMakeVisible (textE.get());
    textE->setMultiLine (false);
    textE->setReturnKeyStartsNewLine (false);
    textE->setReadOnly (false);
    textE->setScrollbarsShown (true);
    textE->setCaretVisible (true);
    textE->setPopupMenuEnabled (true);
    textE->setText (String());

    labelD.reset (new Label ("labelD",
                             TRANS("D:")));
    addAndMakeVisible (labelD.get());
    labelD->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelD->setJustificationType (Justification::centredLeft);
    labelD->setEditable (false, false, false);
    labelD->setColour (TextEditor::textColourId, Colours::black);
    labelD->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textD.reset (new TextEditor ("textD"));
    addAndMakeVisible (textD.get());
    textD->setMultiLine (false);
    textD->setReturnKeyStartsNewLine (false);
    textD->setReadOnly (false);
    textD->setScrollbarsShown (true);
    textD->setCaretVisible (true);
    textD->setPopupMenuEnabled (true);
    textD->setText (String());

    labelPolar.reset (new Label ("labelPolar",
                                 TRANS("Polar:")));
    addAndMakeVisible (labelPolar.get());
    labelPolar->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPolar->setJustificationType (Justification::centredLeft);
    labelPolar->setEditable (false, false, false);
    labelPolar->setColour (TextEditor::textColourId, Colours::black);
    labelPolar->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelPolar->setBounds (0, 8 + 64, 80, 24);

    labelCH.reset (new Label ("labelCH",
                              TRANS("CH:")));
    addAndMakeVisible (labelCH.get());
    labelCH->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCH->setJustificationType (Justification::centredLeft);
    labelCH->setEditable (false, false, false);
    labelCH->setColour (TextEditor::textColourId, Colours::black);
    labelCH->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textCH.reset (new TextEditor ("textCH"));
    addAndMakeVisible (textCH.get());
    textCH->setMultiLine (false);
    textCH->setReturnKeyStartsNewLine (false);
    textCH->setReadOnly (true);
    textCH->setScrollbarsShown (true);
    textCH->setCaretVisible (false);
    textCH->setPopupMenuEnabled (true);
    textCH->setText (String());


    //[UserPreSize]
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


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PointInfoControl::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PointInfoControl::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textName->setBounds (proportionOfWidth (0.4000f) + 0, 8, proportionOfWidth (0.3487f), 24);
    labelX->setBounds ((proportionOfWidth (0.4000f) + 0) + 0 - 24, (8 + 24 - -8) + 0, 24, 24);
    textX->setBounds (proportionOfWidth (0.4000f) + 0, (8 + 24 - -8) + 0, roundToInt ((roundToInt (proportionOfWidth (0.1359f) * 1.0000f)) * 1.0000f), 24);
    labelY->setBounds ((proportionOfWidth (0.6205f) + 0) + 0 - 24, (8 + 24 - -8) + 0, 24, 24);
    textY->setBounds (proportionOfWidth (0.6205f) + 0, 40, roundToInt ((roundToInt (proportionOfWidth (0.1359f) * 1.0000f)) * 1.0000f), 24);
    labelZ->setBounds ((proportionOfWidth (0.8410f) + 0) + 0 - 24, 40, 24, 24);
    textZ->setBounds (proportionOfWidth (0.8410f) + 0, 40, roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    labelCartesian->setBounds (0, 8 + 24 - -8, 80, 24);
    labelA->setBounds (proportionOfWidth (0.4000f) + 0 - 24, (8 + 64) + 0, 24, 24);
    textA->setBounds (proportionOfWidth (0.4000f), 72, roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    labelE->setBounds (proportionOfWidth (0.6205f) + 0 - 24, 72, 24, 24);
    textE->setBounds (proportionOfWidth (0.6205f), 72, roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    labelD->setBounds (proportionOfWidth (0.8410f) + 0 - 24, 72, 24, 24);
    textD->setBounds (proportionOfWidth (0.8410f), 72, proportionOfWidth (0.1359f), 24);
    labelCH->setBounds ((proportionOfWidth (0.8410f) + 0) + 0 - 32, 8, 32, 24);
    textCH->setBounds (proportionOfWidth (0.8410f) + 0, 8, roundToInt (proportionOfWidth (0.1359f) * 1.0000f), 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PointInfoControl::updateSelectedPoint(String exceptField)
{
	disableListeners();

	int selection = pPointSelection->getSelectedPointIndex();
	if (selection >= 0 && selection < pEditablePoints->size())
	{
		AmbiPoint* point = pEditablePoints->get(selection);

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

	enableListeners();
}

void PointInfoControl::changeListenerCallback(ChangeBroadcaster* source)
{
	if(source == pPointSelection)
		updateSelectedPoint();
}

void PointInfoControl::textEditorTextChanged(TextEditor& source)
{
	int selection = pPointSelection->getSelectedPointIndex();

	if (source.getName() == textName->getName())
	{
		pEditablePoints->setChannelName(selection, textName->getText());
		pEditablePoints->setChannelColor(selection, TrackColors::getColor(textName->getText().initialSectionContainingOnly("0123456789").getIntValue()));
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
                 variableInitialisers="pEditablePoints(pEditablePoints), pPointSelection(pPointSelection)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="390" initialHeight="100">
  <BACKGROUND backgroundColour="ff505050"/>
  <TEXTEDITOR name="textName" id="2b706cdb3232f1d2" memberName="textName" virtualName=""
              explicitFocusOrder="0" pos="0 8 34.872% 24" posRelativeX="e76184fec37afbfb"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="1" caret="0" popupmenu="1"/>
  <LABEL name="labelName" id="7326de7683af8e2f" memberName="labelName"
         virtualName="" explicitFocusOrder="0" pos="0 8 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <LABEL name="labelX" id="6fb8ecbc6da5fa76" memberName="labelX" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="4dcd5fc970cdce6c"
         posRelativeY="f2af390154f0c032" edTextCol="ff000000" edBkgCol="0"
         labelText="X:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textX" id="4dcd5fc970cdce6c" memberName="textX" virtualName=""
              explicitFocusOrder="0" pos="0 0 100% 24" posRelativeX="e76184fec37afbfb"
              posRelativeY="f2af390154f0c032" posRelativeW="e76184fec37afbfb"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelY" id="d8cf1d21dc85896e" memberName="labelY" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="46b4702ec60c236a"
         posRelativeY="f2af390154f0c032" edTextCol="ff000000" edBkgCol="0"
         labelText="Y:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textY" id="46b4702ec60c236a" memberName="textY" virtualName=""
              explicitFocusOrder="0" pos="0 40 100% 24" posRelativeX="7fb39984576e614e"
              posRelativeW="7fb39984576e614e" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelZ" id="385e50956f13effd" memberName="labelZ" virtualName=""
         explicitFocusOrder="0" pos="0r 40 24 24" posRelativeX="93f9644500f2da7f"
         edTextCol="ff000000" edBkgCol="0" labelText="Z:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textZ" id="93f9644500f2da7f" memberName="textZ" virtualName=""
              explicitFocusOrder="0" pos="0 40 100% 24" posRelativeX="411a4503627ed096"
              posRelativeW="411a4503627ed096" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelCartesian" id="f2af390154f0c032" memberName="labelCartesian"
         virtualName="" explicitFocusOrder="0" pos="0 -8R 80 24" posRelativeY="7326de7683af8e2f"
         edTextCol="ff000000" edBkgCol="0" labelText="Cartesian:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <LABEL name="labelA" id="c8e81a0262f1ac25" memberName="labelA" virtualName=""
         explicitFocusOrder="0" pos="0r 0 24 24" posRelativeX="e76184fec37afbfb"
         posRelativeY="41fcd21e0ce12407" edTextCol="ff000000" edBkgCol="0"
         labelText="A:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textA" id="e76184fec37afbfb" memberName="textA" virtualName=""
              explicitFocusOrder="0" pos="40% 72 100% 24" posRelativeW="411a4503627ed096"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelE" id="6af5bb62c259b445" memberName="labelE" virtualName=""
         explicitFocusOrder="0" pos="0r 72 24 24" posRelativeX="7fb39984576e614e"
         edTextCol="ff000000" edBkgCol="0" labelText="E:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textE" id="7fb39984576e614e" memberName="textE" virtualName=""
              explicitFocusOrder="0" pos="62.051% 72 100% 24" posRelativeW="411a4503627ed096"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelD" id="b1bd9596558e6f35" memberName="labelD" virtualName=""
         explicitFocusOrder="0" pos="0r 72 24 24" posRelativeX="411a4503627ed096"
         edTextCol="ff000000" edBkgCol="0" labelText="D:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textD" id="411a4503627ed096" memberName="textD" virtualName=""
              explicitFocusOrder="0" pos="84.103% 72 13.59% 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelPolar" id="41fcd21e0ce12407" memberName="labelPolar"
         virtualName="" explicitFocusOrder="0" pos="0 64 80 24" posRelativeY="7326de7683af8e2f"
         edTextCol="ff000000" edBkgCol="0" labelText="Polar:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <LABEL name="labelCH" id="6db02d6b63396cba" memberName="labelCH" virtualName=""
         explicitFocusOrder="0" pos="0r 8 32 24" posRelativeX="d9bdcb7a01c25c33"
         edTextCol="ff000000" edBkgCol="0" labelText="CH:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textCH" id="d9bdcb7a01c25c33" memberName="textCH" virtualName=""
              explicitFocusOrder="0" pos="0 8 100% 24" posRelativeX="411a4503627ed096"
              posRelativeW="411a4503627ed096" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
