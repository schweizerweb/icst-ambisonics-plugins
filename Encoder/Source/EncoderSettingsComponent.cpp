/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "EncoderSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EncoderSettingsComponent::EncoderSettingsComponent (EncoderSettings* pSettings)
    : pEncoderSettings(pSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    groupOscReceive.reset (new GroupComponent ("groupOscReceive",
                                               TRANS("OSC Receive")));
    addAndMakeVisible (groupOscReceive.get());

    toggleReceiveOsc.reset (new ToggleButton ("toggleReceiveOsc"));
    addAndMakeVisible (toggleReceiveOsc.get());
    toggleReceiveOsc->setButtonText (TRANS("Enable"));
    toggleReceiveOsc->addListener (this);

    toggleReceiveOsc->setBounds (18, 29, 150, 24);

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
                                   TRANS("OSC-Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscPort->setJustificationType (Justification::centredLeft);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (TextEditor::textColourId, Colours::black);
    labelOscPort->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    groupOscSend.reset (new GroupComponent ("groupOscSend",
                                            TRANS("OSC Send for ZEnc -> ZDec communication")));
    addAndMakeVisible (groupOscSend.get());

    toggleSendOsc.reset (new ToggleButton ("toggleSendOsc"));
    addAndMakeVisible (toggleSendOsc.get());
    toggleSendOsc->setButtonText (TRANS("Enable"));
    toggleSendOsc->addListener (this);

    toggleSendOsc->setBounds (17, 101, 150, 24);

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
                                     TRANS("Target Host:")));
    addAndMakeVisible (labelOscSendIp.get());
    labelOscSendIp->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIp->setJustificationType (Justification::centredLeft);
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

    labelOscSendPort.reset (new Label ("labelOscSendPort",
                                       TRANS("OSC-Port:\n")));
    addAndMakeVisible (labelOscSendPort.get());
    labelOscSendPort->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendPort->setJustificationType (Justification::centredLeft);
    labelOscSendPort->setEditable (false, false, false);
    labelOscSendPort->setColour (TextEditor::textColourId, Colours::black);
    labelOscSendPort->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

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
    labelOscSendInterval->setJustificationType (Justification::centredLeft);
    labelOscSendInterval->setEditable (false, false, false);
    labelOscSendInterval->setColour (TextEditor::textColourId, Colours::black);
    labelOscSendInterval->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    groupDistanceEncoding.reset (new GroupComponent ("groupDistanceEncoding",
                                                     TRANS("Distance Encoding")));
    addAndMakeVisible (groupDistanceEncoding.get());

    toggleDistanceEncoding.reset (new ToggleButton ("toggleDistanceEncoding"));
    addAndMakeVisible (toggleDistanceEncoding.get());
    toggleDistanceEncoding->setButtonText (TRANS("Enable"));
    toggleDistanceEncoding->addListener (this);

    toggleDistanceEncoding->setBounds (17, 347, 150, 24);

    textUnitCircleRadius.reset (new TextEditor ("textUnitCircleRadius"));
    addAndMakeVisible (textUnitCircleRadius.get());
    textUnitCircleRadius->setMultiLine (false);
    textUnitCircleRadius->setReturnKeyStartsNewLine (false);
    textUnitCircleRadius->setReadOnly (false);
    textUnitCircleRadius->setScrollbarsShown (true);
    textUnitCircleRadius->setCaretVisible (true);
    textUnitCircleRadius->setPopupMenuEnabled (true);
    textUnitCircleRadius->setText (String());

    labelUnitCircleRadius.reset (new Label ("labelUnitCircleRadius",
                                            TRANS("Unit Circle Radius:")));
    addAndMakeVisible (labelUnitCircleRadius.get());
    labelUnitCircleRadius->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelUnitCircleRadius->setJustificationType (Justification::centredLeft);
    labelUnitCircleRadius->setEditable (false, false, false);
    labelUnitCircleRadius->setColour (TextEditor::textColourId, Colours::black);
    labelUnitCircleRadius->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    groupOrientation.reset (new GroupComponent ("groupOrientation",
                                                TRANS("Orientation")));
    addAndMakeVisible (groupOrientation.get());

    toggleDirectionFlip.reset (new ToggleButton ("toggleDirectionFlip"));
    addAndMakeVisible (toggleDirectionFlip.get());
    toggleDirectionFlip->setButtonText (TRANS("Flip Direction"));
    toggleDirectionFlip->addListener (this);

    toggleDirectionFlip->setBounds (17, 417, 150, 24);

    groupOscSendExt.reset (new GroupComponent ("groupOscSendExt",
                                               TRANS("OSC Send for external usage")));
    addAndMakeVisible (groupOscSendExt.get());

    toggleSendOscExt.reset (new ToggleButton ("toggleSendOscExt"));
    addAndMakeVisible (toggleSendOscExt.get());
    toggleSendOscExt->setButtonText (TRANS("Enable"));
    toggleSendOscExt->addListener (this);

    toggleSendOscExt->setBounds (19, 239, 150, 24);

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
                                        TRANS("Target Host:")));
    addAndMakeVisible (labelOscSendIpExt.get());
    labelOscSendIpExt->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIpExt->setJustificationType (Justification::centredLeft);
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

    labelOscSendPortExt.reset (new Label ("labelOscSendPortExt",
                                          TRANS("OSC-Port:\n")));
    addAndMakeVisible (labelOscSendPortExt.get());
    labelOscSendPortExt->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendPortExt->setJustificationType (Justification::centredLeft);
    labelOscSendPortExt->setEditable (false, false, false);
    labelOscSendPortExt->setColour (TextEditor::textColourId, Colours::black);
    labelOscSendPortExt->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 400);


    //[Constructor] You can add your own custom stuff here..
	textOscReceivePort->addListener(this);
	textOscSendPort->addListener(this);
	textOscSendIp->addListener(this);
	textOscSendInterval->addListener(this);
	textOscSendPortExt->addListener(this);
	textOscSendIpExt->addListener(this);
	textUnitCircleRadius->addListener(this);

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
	textUnitCircleRadius->setText(String(pEncoderSettings->unitCircleRadius));
	toggleDirectionFlip->setToggleState(pEncoderSettings->directionFlip, dontSendNotification);
    //[/Constructor]
}

EncoderSettingsComponent::~EncoderSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupOscReceive = nullptr;
    toggleReceiveOsc = nullptr;
    textOscReceivePort = nullptr;
    labelOscPort = nullptr;
    groupOscSend = nullptr;
    toggleSendOsc = nullptr;
    textOscSendIp = nullptr;
    labelOscSendIp = nullptr;
    textOscSendPort = nullptr;
    labelOscSendPort = nullptr;
    textOscSendInterval = nullptr;
    labelOscSendInterval = nullptr;
    groupDistanceEncoding = nullptr;
    toggleDistanceEncoding = nullptr;
    textUnitCircleRadius = nullptr;
    labelUnitCircleRadius = nullptr;
    groupOrientation = nullptr;
    toggleDirectionFlip = nullptr;
    groupOscSendExt = nullptr;
    toggleSendOscExt = nullptr;
    textOscSendIpExt = nullptr;
    labelOscSendIpExt = nullptr;
    textOscSendPortExt = nullptr;
    labelOscSendPortExt = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
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

    groupOscReceive->setBounds (2, 5, getWidth() - 8, 64);
    textOscReceivePort->setBounds (2 + (getWidth() - 8) - 20 - 105, 5 + 24, 105, 24);
    labelOscPort->setBounds (2 + (getWidth() - 8) - 141 - 109, 29, 109, 24);
    groupOscSend->setBounds (1, 77, getWidth() - 8, 131);
    textOscSendIp->setBounds (2 + (getWidth() - 8) - 21 - 136, 5 + 96, 136, 24);
    labelOscSendIp->setBounds (2 + (getWidth() - 8) - 165 - 86, 101, 86, 24);
    textOscSendPort->setBounds (2 + (getWidth() - 8) - 21 - 104, 5 + 132, 104, 24);
    labelOscSendPort->setBounds (2 + (getWidth() - 8) - 141 - 110, 137, 110, 24);
    textOscSendInterval->setBounds (2 + (getWidth() - 8) - 21 - 104, 5 + 167, 104, 24);
    labelOscSendInterval->setBounds (2 + (getWidth() - 8) - 141 - 110, 172, 110, 24);
    groupDistanceEncoding->setBounds (1, 323, getWidth() - 8, 64);
    textUnitCircleRadius->setBounds (2 + (getWidth() - 8) - 21 - 105, 5 + 342, 105, 24);
    labelUnitCircleRadius->setBounds (2 + (getWidth() - 8) - 142 - 109, 347, 109, 24);
    groupOrientation->setBounds (1, 393, getWidth() - 8, 64);
    groupOscSendExt->setBounds (3, 215, getWidth() - 8, 97);
    textOscSendIpExt->setBounds (2 + (getWidth() - 8) - 19 - 136, 5 + 234, 136, 24);
    labelOscSendIpExt->setBounds (2 + (getWidth() - 8) - 163 - 86, 239, 86, 24);
    textOscSendPortExt->setBounds (2 + (getWidth() - 8) - 19 - 104, 5 + 270, 104, 24);
    labelOscSendPortExt->setBounds (2 + (getWidth() - 8) - 139 - 110, 275, 110, 24);
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
        //[/UserButtonCode_toggleReceiveOsc]
    }
    else if (buttonThatWasClicked == toggleSendOsc.get())
    {
        //[UserButtonCode_toggleSendOsc] -- add your button handler code here..
		pEncoderSettings->oscSendFlag = toggleSendOsc->getToggleState();
        //[/UserButtonCode_toggleSendOsc]
    }
    else if (buttonThatWasClicked == toggleDistanceEncoding.get())
    {
        //[UserButtonCode_toggleDistanceEncoding] -- add your button handler code here..
		pEncoderSettings->distanceEncodingFlag = toggleDistanceEncoding->getToggleState();
        //[/UserButtonCode_toggleDistanceEncoding]
    }
    else if (buttonThatWasClicked == toggleDirectionFlip.get())
    {
        //[UserButtonCode_toggleDirectionFlip] -- add your button handler code here..
		pEncoderSettings->directionFlip = toggleDirectionFlip->getToggleState();
        //[/UserButtonCode_toggleDirectionFlip]
    }
    else if (buttonThatWasClicked == toggleSendOscExt.get())
    {
        //[UserButtonCode_toggleSendOscExt] -- add your button handler code here..
		pEncoderSettings->oscSendExtFlag = toggleSendOscExt->getToggleState();
		//[/UserButtonCode_toggleSendOscExt]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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
	else if (&textEditor == textUnitCircleRadius.get())
		checkForNumbers(&textEditor, &pEncoderSettings->unitCircleRadius);
}

void EncoderSettingsComponent::showAsDialog(EncoderSettings* pSettings)
{
	EncoderSettingsComponent *p = new EncoderSettingsComponent(pSettings);
	p->setSize(500, 465);

	DialogWindow::LaunchOptions options;
	options.content.setOwned(p);
	options.dialogTitle = "Encoder-Plugin settings";
	options.dialogBackgroundColour = Colours::white;
	options.escapeKeyTriggersCloseButton = false;
	options.useNativeTitleBar = false;
	options.resizable = false;
	options.runModal();
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
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="EncoderSettings* pSettings" variableInitialisers="pEncoderSettings(pSettings)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="500" initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupOscReceive" id="f4cf3a53a6ef0d87" memberName="groupOscReceive"
                  virtualName="" explicitFocusOrder="0" pos="2 5 8M 64" title="OSC Receive"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="18 29 150 24" buttonText="Enable"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscReceivePort" id="ef3c7d2594795ec7" memberName="textOscReceivePort"
              virtualName="" explicitFocusOrder="0" pos="20Rr 24 105 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="141Rr 29 109 24" posRelativeX="f4cf3a53a6ef0d87"
         edTextCol="ff000000" edBkgCol="0" labelText="OSC-Port:&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <GROUPCOMPONENT name="groupOscSend" id="6e42d11d1b0564d7" memberName="groupOscSend"
                  virtualName="" explicitFocusOrder="0" pos="1 77 8M 131" title="OSC Send for ZEnc -&gt; ZDec communication"/>
  <TOGGLEBUTTON name="toggleSendOsc" id="309574c60d08c55e" memberName="toggleSendOsc"
                virtualName="" explicitFocusOrder="0" pos="17 101 150 24" buttonText="Enable"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIp" id="f5d167c87a68db4c" memberName="textOscSendIp"
              virtualName="" explicitFocusOrder="0" pos="21Rr 96 136 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIp" id="ba6c86d4711d8bd0" memberName="labelOscSendIp"
         virtualName="" explicitFocusOrder="0" pos="165Rr 101 86 24" posRelativeX="f4cf3a53a6ef0d87"
         edTextCol="ff000000" edBkgCol="0" labelText="Target Host:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textOscSendPort" id="c2dff2f42ed301d1" memberName="textOscSendPort"
              virtualName="" explicitFocusOrder="0" pos="21Rr 132 104 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendPort" id="3a8b128e5f4be049" memberName="labelOscSendPort"
         virtualName="" explicitFocusOrder="0" pos="141Rr 137 110 24"
         posRelativeX="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="OSC-Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textOscSendInterval" id="2c20c78495fbd07c" memberName="textOscSendInterval"
              virtualName="" explicitFocusOrder="0" pos="21Rr 167 104 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendInterval" id="104391e6ca4d3f12" memberName="labelOscSendInterval"
         virtualName="" explicitFocusOrder="0" pos="141Rr 172 110 24"
         posRelativeX="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="groupDistanceEncoding" id="b72378bdfe4e130" memberName="groupDistanceEncoding"
                  virtualName="" explicitFocusOrder="0" pos="1 323 8M 64" title="Distance Encoding"/>
  <TOGGLEBUTTON name="toggleDistanceEncoding" id="c46d0c7f045490ec" memberName="toggleDistanceEncoding"
                virtualName="" explicitFocusOrder="0" pos="17 347 150 24" buttonText="Enable"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textUnitCircleRadius" id="9b1288d93f3bb201" memberName="textUnitCircleRadius"
              virtualName="" explicitFocusOrder="0" pos="21Rr 342 105 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelUnitCircleRadius" id="efb161958642a965" memberName="labelUnitCircleRadius"
         virtualName="" explicitFocusOrder="0" pos="142Rr 347 109 24"
         posRelativeX="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Unit Circle Radius:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="groupOrientation" id="3d25e935657cd603" memberName="groupOrientation"
                  virtualName="" explicitFocusOrder="0" pos="1 393 8M 64" title="Orientation"/>
  <TOGGLEBUTTON name="toggleDirectionFlip" id="261d6104440c6519" memberName="toggleDirectionFlip"
                virtualName="" explicitFocusOrder="0" pos="17 417 150 24" buttonText="Flip Direction"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <GROUPCOMPONENT name="groupOscSendExt" id="645904a2472668e1" memberName="groupOscSendExt"
                  virtualName="" explicitFocusOrder="0" pos="3 215 8M 97" title="OSC Send for external usage"/>
  <TOGGLEBUTTON name="toggleSendOscExt" id="4c50ad9f06c0feaf" memberName="toggleSendOscExt"
                virtualName="" explicitFocusOrder="0" pos="19 239 150 24" buttonText="Enable"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIpExt" id="ed3cbf6e9d145d08" memberName="textOscSendIpExt"
              virtualName="" explicitFocusOrder="0" pos="19Rr 234 136 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExt" id="ee4b90142f7a53f4" memberName="labelOscSendIpExt"
         virtualName="" explicitFocusOrder="0" pos="163Rr 239 86 24" posRelativeX="f4cf3a53a6ef0d87"
         edTextCol="ff000000" edBkgCol="0" labelText="Target Host:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textOscSendPortExt" id="8f6b9c97cd3fa86f" memberName="textOscSendPortExt"
              virtualName="" explicitFocusOrder="0" pos="19Rr 270 104 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendPortExt" id="4d22d18a70ccb67a" memberName="labelOscSendPortExt"
         virtualName="" explicitFocusOrder="0" pos="139Rr 275 110 24"
         posRelativeX="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="OSC-Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

