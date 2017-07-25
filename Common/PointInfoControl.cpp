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
//[/Headers]

#include "PointInfoControl.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PointInfoControl::PointInfoControl (Array<AmbiPoint>* pAmbiPointArray, PointSelection* pPointSelection)
    : pAmbiPointArray(pAmbiPointArray), pPointSelection(pPointSelection)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (comboBoxSelctedPoint = new ComboBox ("selectedPoint"));
    comboBoxSelctedPoint->setEditableText (false);
    comboBoxSelctedPoint->setJustificationType (Justification::centredLeft);
    comboBoxSelctedPoint->setTextWhenNothingSelected (String());
    comboBoxSelctedPoint->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxSelctedPoint->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Selected Point:")));
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textName = new TextEditor ("textName"));
    textName->setMultiLine (false);
    textName->setReturnKeyStartsNewLine (false);
    textName->setReadOnly (false);
    textName->setScrollbarsShown (true);
    textName->setCaretVisible (true);
    textName->setPopupMenuEnabled (true);
    textName->setText (String());

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Name:")));
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("X:")));
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textX = new TextEditor ("textX"));
    textX->setMultiLine (false);
    textX->setReturnKeyStartsNewLine (false);
    textX->setReadOnly (false);
    textX->setScrollbarsShown (true);
    textX->setCaretVisible (true);
    textX->setPopupMenuEnabled (true);
    textX->setText (String());

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Y:")));
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textY = new TextEditor ("textY"));
    textY->setMultiLine (false);
    textY->setReturnKeyStartsNewLine (false);
    textY->setReadOnly (false);
    textY->setScrollbarsShown (true);
    textY->setCaretVisible (true);
    textY->setPopupMenuEnabled (true);
    textY->setText (String());

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Z:")));
    label5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textZ = new TextEditor ("textZ"));
    textZ->setMultiLine (false);
    textZ->setReturnKeyStartsNewLine (false);
    textZ->setReadOnly (false);
    textZ->setScrollbarsShown (true);
    textZ->setCaretVisible (true);
    textZ->setPopupMenuEnabled (true);
    textZ->setText (String());

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("Cartesian:")));
    label6->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label7 = new Label ("new label",
                                           TRANS("A:")));
    label7->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textA = new TextEditor ("textA"));
    textA->setMultiLine (false);
    textA->setReturnKeyStartsNewLine (false);
    textA->setReadOnly (false);
    textA->setScrollbarsShown (true);
    textA->setCaretVisible (true);
    textA->setPopupMenuEnabled (true);
    textA->setText (String());

    addAndMakeVisible (label8 = new Label ("new label",
                                           TRANS("E:")));
    label8->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textE = new TextEditor ("textE"));
    textE->setMultiLine (false);
    textE->setReturnKeyStartsNewLine (false);
    textE->setReadOnly (false);
    textE->setScrollbarsShown (true);
    textE->setCaretVisible (true);
    textE->setPopupMenuEnabled (true);
    textE->setText (String());

    addAndMakeVisible (label9 = new Label ("new label",
                                           TRANS("D:")));
    label9->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textD = new TextEditor ("textD"));
    textD->setMultiLine (false);
    textD->setReturnKeyStartsNewLine (false);
    textD->setReadOnly (false);
    textD->setScrollbarsShown (true);
    textD->setCaretVisible (true);
    textD->setPopupMenuEnabled (true);
    textD->setText (String());

    addAndMakeVisible (label10 = new Label ("new label",
                                            TRANS("Polar:")));
    label10->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centredLeft);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText (TRANS("refresh"));
    textButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (360, 140);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PointInfoControl::~PointInfoControl()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBoxSelctedPoint = nullptr;
    label = nullptr;
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
    textButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PointInfoControl::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PointInfoControl::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBoxSelctedPoint->setBounds (112, 0, 192, 24);
    label->setBounds (0, 0, 104, 24);
    textName->setBounds (112, 32, 192, 24);
    label2->setBounds (0, 32, 104, 24);
    label3->setBounds (112, 64, 24, 24);
    textX->setBounds (136, 64, 56, 24);
    label4->setBounds (192, 64, 24, 24);
    textY->setBounds (216, 64, 56, 24);
    label5->setBounds (273, 64, 24, 24);
    textZ->setBounds (297, 64, 55, 24);
    label6->setBounds (0, 64, 104, 24);
    label7->setBounds (112, 96, 24, 24);
    textA->setBounds (136, 96, 56, 24);
    label8->setBounds (192, 96, 24, 24);
    textE->setBounds (216, 96, 56, 24);
    label9->setBounds (273, 96, 24, 24);
    textD->setBounds (297, 96, 55, 24);
    label10->setBounds (0, 96, 104, 24);
    textButton->setBounds (312, 16, 48, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PointInfoControl::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxSelctedPoint)
    {
        //[UserComboBoxCode_comboBoxSelctedPoint] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBoxSelctedPoint]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void PointInfoControl::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
		updateSelectedPoint();
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PointInfoControl::updateSelectedPoint()
{
	int selection = pPointSelection->getSelectedPointIndex();
	if (selection >= 0 && selection < pAmbiPointArray->size())
	{
		AmbiPoint point = pAmbiPointArray->getReference(selection);
		textX->setText(String(point.getPoint()->getX()));
		textY->setText(String(point.getPoint()->getY()));
		textZ->setText(String(point.getPoint()->getZ()));
	}
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PointInfoControl" componentName=""
                 parentClasses="public Component" constructorParams="Array&lt;AmbiPoint&gt;* pAmbiPointArray, PointSelection* pPointSelection"
                 variableInitialisers="pAmbiPointArray(pAmbiPointArray), pPointSelection(pPointSelection)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="360" initialHeight="140">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="selectedPoint" id="7c4eb87f9d5c91a7" memberName="comboBoxSelctedPoint"
            virtualName="" explicitFocusOrder="0" pos="112 0 192 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="ca93cd001ab1b76c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 0 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Selected Point:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textName" id="2b706cdb3232f1d2" memberName="textName" virtualName=""
              explicitFocusOrder="0" pos="112 32 192 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="7326de7683af8e2f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="0 32 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6fb8ecbc6da5fa76" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="112 64 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="X:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textX" id="4dcd5fc970cdce6c" memberName="textX" virtualName=""
              explicitFocusOrder="0" pos="136 64 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="d8cf1d21dc85896e" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="192 64 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Y:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textY" id="46b4702ec60c236a" memberName="textY" virtualName=""
              explicitFocusOrder="0" pos="216 64 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="385e50956f13effd" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="273 64 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Z:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textZ" id="93f9644500f2da7f" memberName="textZ" virtualName=""
              explicitFocusOrder="0" pos="297 64 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="f2af390154f0c032" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="0 64 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cartesian:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c8e81a0262f1ac25" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="112 96 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="A:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textA" id="e76184fec37afbfb" memberName="textA" virtualName=""
              explicitFocusOrder="0" pos="136 96 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="6af5bb62c259b445" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="192 96 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="E:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textE" id="7fb39984576e614e" memberName="textE" virtualName=""
              explicitFocusOrder="0" pos="216 96 56 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="b1bd9596558e6f35" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="273 96 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="D:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textD" id="411a4503627ed096" memberName="textD" virtualName=""
              explicitFocusOrder="0" pos="297 96 55 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="41fcd21e0ce12407" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="0 96 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Polar:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="f00695df023ca647" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="312 16 48 24" buttonText="refresh"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
