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
//[/Headers]

#include "OSCSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCSettingsComponent::OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings)
    : pSettings(pSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    //[/Constructor_pre]

    toggleReceiveOsc.reset (new ToggleButton ("toggleReceiveOsc"));
    addAndMakeVisible (toggleReceiveOsc.get());
    toggleReceiveOsc->setButtonText (TRANS("Receive OSC"));
    toggleReceiveOsc->addListener (this);

    toggleReceiveOsc->setBounds (6, 11, 150, 24);

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

    toggleSendOsc->setBounds (6, 71, 199, 24);

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

    toggleSendOscExt.reset (new ToggleButton ("toggleSendOscExt"));
    addAndMakeVisible (toggleSendOscExt.get());
    toggleSendOscExt->setButtonText (TRANS("Send Positions for External Usage"));
    toggleSendOscExt->addListener (this);

    toggleSendOscExt->setBounds (6, 41, 245, 24);

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


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    textOscReceivePort->addListener(this);
    textOscSendPort->addListener(this);
    textOscSendIp->addListener(this);
    textOscSendInterval->addListener(this);
    textOscSendPortExt->addListener(this);
    textOscSendIpExt->addListener(this);

    toggleReceiveOsc->setToggleState(pSettings->oscReceiveFlag, dontSendNotification);
    textOscReceivePort->setText(String(pSettings->oscReceivePort));

    toggleSendOsc->setToggleState(pSettings->oscSendFlag, dontSendNotification);
    textOscSendIp->setText(String(pSettings->oscSendTargetHost));
    textOscSendPort->setText(String(pSettings->oscSendPort));
    textOscSendInterval->setText(String(pSettings->oscSendIntervalMs));

    toggleSendOscExt->setToggleState(pSettings->oscSendExtFlag, dontSendNotification);
    textOscSendIpExt->setText(String(pSettings->oscSendExtTargetHost));
    textOscSendPortExt->setText(String(pSettings->oscSendExtPort));

    //[/Constructor]
}

OSCSettingsComponent::~OSCSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    toggleReceiveOsc = nullptr;
    textOscReceivePort = nullptr;
    labelOscPort = nullptr;
    toggleSendOsc = nullptr;
    textOscSendIp = nullptr;
    labelOscSendIp = nullptr;
    textOscSendPort = nullptr;
    textOscSendInterval = nullptr;
    labelOscSendInterval = nullptr;
    toggleSendOscExt = nullptr;
    textOscSendIpExt = nullptr;
    labelOscSendIpExt = nullptr;
    textOscSendPortExt = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textOscReceivePort->setBounds (getWidth() - 8 - 48, 11, 48, 24);
    labelOscPort->setBounds (getWidth() - 59 - 49, 11, 49, 24);
    textOscSendIp->setBounds (getWidth() - 62 - 106, 71, 106, 24);
    labelOscSendIp->setBounds (getWidth() - 172 - 124, 71, 124, 24);
    textOscSendPort->setBounds (getWidth() - 8 - 48, 71, 48, 24);
    textOscSendInterval->setBounds (getWidth() - 8 - 48, 101, 48, 24);
    labelOscSendInterval->setBounds (getWidth() - 60 - 110, 101, 110, 24);
    textOscSendIpExt->setBounds (getWidth() - 62 - 106, 41, 106, 24);
    labelOscSendIpExt->setBounds (getWidth() - 172 - 126, 41, 126, 24);
    textOscSendPortExt->setBounds (getWidth() - 8 - 48, 41, 48, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleReceiveOsc.get())
    {
        //[UserButtonCode_toggleReceiveOsc] -- add your button handler code here..
        pSettings->oscReceiveFlag = toggleReceiveOsc->getToggleState();
        sendChangeMessage();
        //[/UserButtonCode_toggleReceiveOsc]
    }
    else if (buttonThatWasClicked == toggleSendOsc.get())
    {
        //[UserButtonCode_toggleSendOsc] -- add your button handler code here..
        pSettings->oscSendFlag = toggleSendOsc->getToggleState();
        sendChangeMessage();
        //[/UserButtonCode_toggleSendOsc]
    }
    else if (buttonThatWasClicked == toggleSendOscExt.get())
    {
        //[UserButtonCode_toggleSendOscExt] -- add your button handler code here..
        pSettings->oscSendExtFlag = toggleSendOscExt->getToggleState();
        sendChangeMessage();

        //[/UserButtonCode_toggleSendOscExt]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void OSCSettingsComponent::textEditorTextChanged(TextEditor& textEditor)
{
    if (&textEditor == textOscReceivePort.get())
        checkForNumbers(&textEditor, &pSettings->oscReceivePort);
    else if (&textEditor == textOscSendPort.get())
        checkForNumbers(&textEditor, &pSettings->oscSendPort);
    else if (&textEditor == textOscSendInterval.get())
        checkForNumbers(&textEditor, &pSettings->oscSendIntervalMs);
    else if (&textEditor == textOscSendIp.get())
        pSettings->oscSendTargetHost = textEditor.getText();
    else if (&textEditor == textOscSendPortExt.get())
        checkForNumbers(&textEditor, &pSettings->oscSendExtPort);
    else if (&textEditor == textOscSendIpExt.get())
        pSettings->oscSendExtTargetHost = textEditor.getText();

    sendChangeMessage();
}

void OSCSettingsComponent::controlDimming() const
{
    textOscReceivePort->setEnabled(pSettings->oscReceiveFlag);
    textOscSendIp->setEnabled(pSettings->oscSendFlag);
    textOscSendPort->setEnabled(pSettings->oscSendFlag);
    textOscSendInterval->setEnabled(pSettings->oscSendFlag);
    textOscSendIpExt->setEnabled(pSettings->oscSendExtFlag);
    textOscSendPortExt->setEnabled(pSettings->oscSendExtFlag);
}


void OSCSettingsComponent::checkForNumbers(TextEditor* pEditor, int* pParameter) const
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

void OSCSettingsComponent::checkForNumbers(TextEditor* pEditor, float* pParameter) const
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

<JUCER_COMPONENT documentType="Component" className="OSCSettingsComponent" componentName=""
                 parentClasses="public Component, public TextEditor::Listener, public ChangeBroadcaster"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings"
                 variableInitialisers="pSettings(pSettings)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="6 11 150 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Receive OSC" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscReceivePort" id="ef3c7d2594795ec7" memberName="textOscReceivePort"
              virtualName="" explicitFocusOrder="0" pos="8Rr 11 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="59Rr 11 49 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOsc" id="309574c60d08c55e" memberName="toggleSendOsc"
                virtualName="" explicitFocusOrder="0" pos="6 71 199 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Send Positions to Decoder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIp" id="f5d167c87a68db4c" memberName="textOscSendIp"
              virtualName="" explicitFocusOrder="0" pos="62Rr 71 106 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIp" id="ba6c86d4711d8bd0" memberName="labelOscSendIp"
         virtualName="" explicitFocusOrder="0" pos="172Rr 71 124 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textOscSendPort" id="c2dff2f42ed301d1" memberName="textOscSendPort"
              virtualName="" explicitFocusOrder="0" pos="8Rr 71 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textOscSendInterval" id="2c20c78495fbd07c" memberName="textOscSendInterval"
              virtualName="" explicitFocusOrder="0" pos="8Rr 101 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendInterval" id="104391e6ca4d3f12" memberName="labelOscSendInterval"
         virtualName="" explicitFocusOrder="0" pos="60Rr 101 110 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOscExt" id="4c50ad9f06c0feaf" memberName="toggleSendOscExt"
                virtualName="" explicitFocusOrder="0" pos="6 41 245 24" posRelativeX="f4cf3a53a6ef0d87"
                posRelativeY="f4cf3a53a6ef0d87" buttonText="Send Positions for External Usage"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIpExt" id="ed3cbf6e9d145d08" memberName="textOscSendIpExt"
              virtualName="" explicitFocusOrder="0" pos="62Rr 41 106 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExt" id="ee4b90142f7a53f4" memberName="labelOscSendIpExt"
         virtualName="" explicitFocusOrder="0" pos="172Rr 41 126 24" posRelativeX="f4cf3a53a6ef0d87"
         posRelativeY="f4cf3a53a6ef0d87" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textOscSendPortExt" id="8f6b9c97cd3fa86f" memberName="textOscSendPortExt"
              virtualName="" explicitFocusOrder="0" pos="8Rr 41 48 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

