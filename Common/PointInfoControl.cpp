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

    textName->setBounds (136, 8, 216, 24);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Name:")));
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (0, 8, 104, 24);

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("X:")));
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (112, 40, 24, 24);

    addAndMakeVisible (textX = new TextEditor ("textX"));
    textX->setMultiLine (false);
    textX->setReturnKeyStartsNewLine (false);
    textX->setReadOnly (false);
    textX->setScrollbarsShown (true);
    textX->setCaretVisible (true);
    textX->setPopupMenuEnabled (true);
    textX->setText (String());

    textX->setBounds (136, 40, 56, 24);

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Y:")));
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (192, 40, 24, 24);

    addAndMakeVisible (textY = new TextEditor ("textY"));
    textY->setMultiLine (false);
    textY->setReturnKeyStartsNewLine (false);
    textY->setReadOnly (false);
    textY->setScrollbarsShown (true);
    textY->setCaretVisible (true);
    textY->setPopupMenuEnabled (true);
    textY->setText (String());

    textY->setBounds (216, 40, 56, 24);

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Z:")));
    label5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (273, 40, 24, 24);

    addAndMakeVisible (textZ = new TextEditor ("textZ"));
    textZ->setMultiLine (false);
    textZ->setReturnKeyStartsNewLine (false);
    textZ->setReadOnly (false);
    textZ->setScrollbarsShown (true);
    textZ->setCaretVisible (true);
    textZ->setPopupMenuEnabled (true);
    textZ->setText (String());

    textZ->setBounds (297, 40, 55, 24);

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("Cartesian:")));
    label6->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (0, 40, 104, 24);

    addAndMakeVisible (label7 = new Label ("new label",
                                           TRANS("A:")));
    label7->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (112, 72, 24, 24);

    addAndMakeVisible (textA = new TextEditor ("textA"));
    textA->setMultiLine (false);
    textA->setReturnKeyStartsNewLine (false);
    textA->setReadOnly (false);
    textA->setScrollbarsShown (true);
    textA->setCaretVisible (true);
    textA->setPopupMenuEnabled (true);
    textA->setText (String());

    textA->setBounds (136, 72, 56, 24);

    addAndMakeVisible (label8 = new Label ("new label",
                                           TRANS("E:")));
    label8->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label8->setBounds (192, 72, 24, 24);

    addAndMakeVisible (textE = new TextEditor ("textE"));
    textE->setMultiLine (false);
    textE->setReturnKeyStartsNewLine (false);
    textE->setReadOnly (false);
    textE->setScrollbarsShown (true);
    textE->setCaretVisible (true);
    textE->setPopupMenuEnabled (true);
    textE->setText (String());

    textE->setBounds (216, 72, 56, 24);

    addAndMakeVisible (label9 = new Label ("new label",
                                           TRANS("D:")));
    label9->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label9->setBounds (273, 72, 24, 24);

    addAndMakeVisible (textD = new TextEditor ("textD"));
    textD->setMultiLine (false);
    textD->setReturnKeyStartsNewLine (false);
    textD->setReadOnly (false);
    textD->setScrollbarsShown (true);
    textD->setCaretVisible (true);
    textD->setPopupMenuEnabled (true);
    textD->setText (String());

    textD->setBounds (297, 72, 55, 24);

    addAndMakeVisible (label10 = new Label ("new label",
                                            TRANS("Polar:")));
    label10->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centredLeft);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label10->setBounds (0, 72, 104, 24);


    //[UserPreSize]
	textName->setReadOnly(!pRadarOptions->nameFieldEditable);
    //[/UserPreSize]

    setSize (360, 100);


    //[Constructor] You can add your own custom stuff here..
	updateSelectedPoint();
	pPointSelection->addChangeListener(this);
    //[/Constructor]
}

PointInfoControl::~PointInfoControl()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textName = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    textX = nullptr;
    label4 = nullptr;
    textY = nullptr;
    label5 = nullptr;
    textZ = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    textA = nullptr;
    label8 = nullptr;
    textE = nullptr;
    label9 = nullptr;
    textD = nullptr;
    label10 = nullptr;


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
              explicitFocusOrder="0" pos="136 8 216 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="7326de7683af8e2f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="0 8 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6fb8ecbc6da5fa76" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="112 40 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="X:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textX" id="4dcd5fc970cdce6c" memberName="textX" virtualName=""
              explicitFocusOrder="0" pos="136 40 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="d8cf1d21dc85896e" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="192 40 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Y:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textY" id="46b4702ec60c236a" memberName="textY" virtualName=""
              explicitFocusOrder="0" pos="216 40 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="385e50956f13effd" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="273 40 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Z:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textZ" id="93f9644500f2da7f" memberName="textZ" virtualName=""
              explicitFocusOrder="0" pos="297 40 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="f2af390154f0c032" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="0 40 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cartesian:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c8e81a0262f1ac25" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="112 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="A:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textA" id="e76184fec37afbfb" memberName="textA" virtualName=""
              explicitFocusOrder="0" pos="136 72 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="6af5bb62c259b445" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="192 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="E:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textE" id="7fb39984576e614e" memberName="textE" virtualName=""
              explicitFocusOrder="0" pos="216 72 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="b1bd9596558e6f35" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="273 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="D:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textD" id="411a4503627ed096" memberName="textD" virtualName=""
              explicitFocusOrder="0" pos="297 72 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="41fcd21e0ce12407" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="0 72 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Polar:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
