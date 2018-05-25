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
#include "RadarOptions.h"
//[/Headers]

#include "PointInfoControl.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PointInfoControl::PointInfoControl (OwnedArray<AmbiPoint>* pEditablePointsArray, PointSelection* pPointSelection, RadarOptions* pRadarOptions)
    : pEditablePointsArray(pEditablePointsArray), pPointSelection(pPointSelection)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (textName = new TextEditor ("textName"));
    textName->setMultiLine (false);
    textName->setReturnKeyStartsNewLine (false);
    textName->setReadOnly (true);
    textName->setScrollbarsShown (true);
    textName->setCaretVisible (false);
    textName->setPopupMenuEnabled (true);
    textName->setText (String());

    textName->setBounds (120, 8, 144, 24);

    addAndMakeVisible (labelName = new Label ("labelName",
                                              TRANS("Name:")));
    labelName->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelName->setJustificationType (Justification::centredLeft);
    labelName->setEditable (false, false, false);
    labelName->setColour (TextEditor::textColourId, Colours::black);
    labelName->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelName->setBounds (0, 8, 80, 24);

    addAndMakeVisible (labelX = new Label ("labelX",
                                           TRANS("X:")));
    labelX->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelX->setJustificationType (Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (TextEditor::textColourId, Colours::black);
    labelX->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelX->setBounds (88, 40, 24, 24);

    addAndMakeVisible (textX = new TextEditor ("textX"));
    textX->setMultiLine (false);
    textX->setReturnKeyStartsNewLine (false);
    textX->setReadOnly (false);
    textX->setScrollbarsShown (true);
    textX->setCaretVisible (true);
    textX->setPopupMenuEnabled (true);
    textX->setText (String());

    textX->setBounds (120, 40, 56, 24);

    addAndMakeVisible (labelY = new Label ("labelY",
                                           TRANS("Y:")));
    labelY->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelY->setJustificationType (Justification::centredLeft);
    labelY->setEditable (false, false, false);
    labelY->setColour (TextEditor::textColourId, Colours::black);
    labelY->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelY->setBounds (176, 40, 24, 24);

    addAndMakeVisible (textY = new TextEditor ("textY"));
    textY->setMultiLine (false);
    textY->setReturnKeyStartsNewLine (false);
    textY->setReadOnly (false);
    textY->setScrollbarsShown (true);
    textY->setCaretVisible (true);
    textY->setPopupMenuEnabled (true);
    textY->setText (String());

    textY->setBounds (208, 40, 56, 24);

    addAndMakeVisible (labelZ = new Label ("labelZ",
                                           TRANS("Z:")));
    labelZ->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelZ->setJustificationType (Justification::centredLeft);
    labelZ->setEditable (false, false, false);
    labelZ->setColour (TextEditor::textColourId, Colours::black);
    labelZ->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelZ->setBounds (265, 40, 24, 24);

    addAndMakeVisible (textZ = new TextEditor ("textZ"));
    textZ->setMultiLine (false);
    textZ->setReturnKeyStartsNewLine (false);
    textZ->setReadOnly (false);
    textZ->setScrollbarsShown (true);
    textZ->setCaretVisible (true);
    textZ->setPopupMenuEnabled (true);
    textZ->setText (String());

    textZ->setBounds (297, 40, 55, 24);

    addAndMakeVisible (labelCartesian = new Label ("labelCartesian",
                                                   TRANS("Cartesian:")));
    labelCartesian->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCartesian->setJustificationType (Justification::centredLeft);
    labelCartesian->setEditable (false, false, false);
    labelCartesian->setColour (TextEditor::textColourId, Colours::black);
    labelCartesian->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelCartesian->setBounds (0, 40, 80, 24);

    addAndMakeVisible (labelA = new Label ("labelA",
                                           TRANS("A:")));
    labelA->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelA->setJustificationType (Justification::centredLeft);
    labelA->setEditable (false, false, false);
    labelA->setColour (TextEditor::textColourId, Colours::black);
    labelA->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelA->setBounds (88, 72, 24, 24);

    addAndMakeVisible (textA = new TextEditor ("textA"));
    textA->setMultiLine (false);
    textA->setReturnKeyStartsNewLine (false);
    textA->setReadOnly (false);
    textA->setScrollbarsShown (true);
    textA->setCaretVisible (true);
    textA->setPopupMenuEnabled (true);
    textA->setText (String());

    textA->setBounds (120, 72, 56, 24);

    addAndMakeVisible (labelE = new Label ("labelE",
                                           TRANS("E:")));
    labelE->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelE->setJustificationType (Justification::centredLeft);
    labelE->setEditable (false, false, false);
    labelE->setColour (TextEditor::textColourId, Colours::black);
    labelE->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelE->setBounds (176, 72, 24, 24);

    addAndMakeVisible (textE = new TextEditor ("textE"));
    textE->setMultiLine (false);
    textE->setReturnKeyStartsNewLine (false);
    textE->setReadOnly (false);
    textE->setScrollbarsShown (true);
    textE->setCaretVisible (true);
    textE->setPopupMenuEnabled (true);
    textE->setText (String());

    textE->setBounds (208, 72, 56, 24);

    addAndMakeVisible (labelD = new Label ("labelD",
                                           TRANS("D:")));
    labelD->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelD->setJustificationType (Justification::centredLeft);
    labelD->setEditable (false, false, false);
    labelD->setColour (TextEditor::textColourId, Colours::black);
    labelD->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelD->setBounds (265, 72, 24, 24);

    addAndMakeVisible (textD = new TextEditor ("textD"));
    textD->setMultiLine (false);
    textD->setReturnKeyStartsNewLine (false);
    textD->setReadOnly (false);
    textD->setScrollbarsShown (true);
    textD->setCaretVisible (true);
    textD->setPopupMenuEnabled (true);
    textD->setText (String());

    textD->setBounds (297, 72, 55, 24);

    addAndMakeVisible (labelPolar = new Label ("labelPolar",
                                               TRANS("Polar:")));
    labelPolar->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPolar->setJustificationType (Justification::centredLeft);
    labelPolar->setEditable (false, false, false);
    labelPolar->setColour (TextEditor::textColourId, Colours::black);
    labelPolar->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelPolar->setBounds (0, 72, 80, 24);

    addAndMakeVisible (labelCH = new Label ("labelCH",
                                            TRANS("CH:")));
    labelCH->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCH->setJustificationType (Justification::centredLeft);
    labelCH->setEditable (false, false, false);
    labelCH->setColour (TextEditor::textColourId, Colours::black);
    labelCH->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelCH->setBounds (264, 8, 32, 24);

    addAndMakeVisible (textCH = new TextEditor ("textCH"));
    textCH->setMultiLine (false);
    textCH->setReturnKeyStartsNewLine (false);
    textCH->setReadOnly (true);
    textCH->setScrollbarsShown (true);
    textCH->setCaretVisible (false);
    textCH->setPopupMenuEnabled (true);
    textCH->setText (String());

    textCH->setBounds (296, 8, 55, 24);


    //[UserPreSize]
	textName->setReadOnly(!pRadarOptions->nameFieldEditable);
    //[/UserPreSize]

    setSize (360, 100);


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

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PointInfoControl::updateSelectedPoint(String exceptField)
{
	disableListeners();

	int selection = pPointSelection->getSelectedPointIndex();
	if (selection >= 0 && selection < pEditablePointsArray->size())
	{
		AmbiPoint* point = pEditablePointsArray->getUnchecked(selection);

		setFieldsEnabled(true);
		textName->setText(point->getName());
		textCH->setText(String(selection + 1));
		if (exceptField != textX->getName()) textX->setText(String(point->getPoint()->getX(), 3));
		if (exceptField != textY->getName()) textY->setText(String(point->getPoint()->getY(), 3));
		if (exceptField != textZ->getName()) textZ->setText(String(point->getPoint()->getZ(), 3));
		if (exceptField != textA->getName()) textA->setText(String(RadToGrad(point->getPoint()->getAzimuth()), 2));
		if (exceptField != textE->getName()) textE->setText(String(RadToGrad(point->getPoint()->getElevation()), 2));
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
	if (selection < 0 || selection >= pEditablePointsArray->size())
		return;

	if (source.getName() == textName->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->setName(textName->getText());
		pEditablePointsArray->getUnchecked(selection)->setColorIndex(textName->getText().initialSectionContainingOnly("0123456789").getIntValue());
	}

	if (source.getName() == textX->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->getPoint()->setX(textX->getText().getFloatValue());
	}
	if (source.getName() == textY->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->getPoint()->setY(textY->getText().getFloatValue());
	}
	if (source.getName() == textZ->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->getPoint()->setZ(textZ->getText().getFloatValue());
	}

	if (source.getName() == textA->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->getPoint()->setAzimuth(GradToRad(textA->getText().getFloatValue()));
	}
	if (source.getName() == textE->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->getPoint()->setElevation(GradToRad(textE->getText().getFloatValue()));
	}
	if (source.getName() == textD->getName())
	{
		pEditablePointsArray->getUnchecked(selection)->getPoint()->setDistance(textD->getText().getFloatValue());
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

double PointInfoControl::RadToGrad(double rad) const
{
	return rad * 180.0 / PI;
}

double PointInfoControl::GradToRad(float grad) const
{
	return grad * PI / 180.0;
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
                 constructorParams="OwnedArray&lt;AmbiPoint&gt;* pEditablePointsArray, PointSelection* pPointSelection, RadarOptions* pRadarOptions"
                 variableInitialisers="pEditablePointsArray(pEditablePointsArray), pPointSelection(pPointSelection)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="360" initialHeight="100">
  <BACKGROUND backgroundColour="ff505050"/>
  <TEXTEDITOR name="textName" id="2b706cdb3232f1d2" memberName="textName" virtualName=""
              explicitFocusOrder="0" pos="120 8 144 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <LABEL name="labelName" id="7326de7683af8e2f" memberName="labelName"
         virtualName="" explicitFocusOrder="0" pos="0 8 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <LABEL name="labelX" id="6fb8ecbc6da5fa76" memberName="labelX" virtualName=""
         explicitFocusOrder="0" pos="88 40 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="X:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textX" id="4dcd5fc970cdce6c" memberName="textX" virtualName=""
              explicitFocusOrder="0" pos="120 40 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelY" id="d8cf1d21dc85896e" memberName="labelY" virtualName=""
         explicitFocusOrder="0" pos="176 40 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Y:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textY" id="46b4702ec60c236a" memberName="textY" virtualName=""
              explicitFocusOrder="0" pos="208 40 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelZ" id="385e50956f13effd" memberName="labelZ" virtualName=""
         explicitFocusOrder="0" pos="265 40 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Z:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textZ" id="93f9644500f2da7f" memberName="textZ" virtualName=""
              explicitFocusOrder="0" pos="297 40 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelCartesian" id="f2af390154f0c032" memberName="labelCartesian"
         virtualName="" explicitFocusOrder="0" pos="0 40 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cartesian:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <LABEL name="labelA" id="c8e81a0262f1ac25" memberName="labelA" virtualName=""
         explicitFocusOrder="0" pos="88 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="A:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textA" id="e76184fec37afbfb" memberName="textA" virtualName=""
              explicitFocusOrder="0" pos="120 72 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelE" id="6af5bb62c259b445" memberName="labelE" virtualName=""
         explicitFocusOrder="0" pos="176 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="E:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textE" id="7fb39984576e614e" memberName="textE" virtualName=""
              explicitFocusOrder="0" pos="208 72 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelD" id="b1bd9596558e6f35" memberName="labelD" virtualName=""
         explicitFocusOrder="0" pos="265 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="D:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textD" id="411a4503627ed096" memberName="textD" virtualName=""
              explicitFocusOrder="0" pos="297 72 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelPolar" id="41fcd21e0ce12407" memberName="labelPolar"
         virtualName="" explicitFocusOrder="0" pos="0 72 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Polar:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCH" id="6db02d6b63396cba" memberName="labelCH" virtualName=""
         explicitFocusOrder="0" pos="264 8 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="CH:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textCH" id="d9bdcb7a01c25c33" memberName="textCH" virtualName=""
              explicitFocusOrder="0" pos="296 8 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
