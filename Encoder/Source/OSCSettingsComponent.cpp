/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

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
OSCSettingsComponent::OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler)
    : pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    //[/Constructor_pre]

    groupExternal.reset (new GroupComponent ("groupExternal",
                                             TRANS("External")));
    addAndMakeVisible (groupExternal.get());

    groupInternal.reset (new GroupComponent ("groupInternal",
                                             TRANS("Internal")));
    addAndMakeVisible (groupInternal.get());

    toggleReceiveOsc.reset (new ToggleButton ("toggleReceiveOsc"));
    addAndMakeVisible (toggleReceiveOsc.get());
    toggleReceiveOsc->setButtonText (TRANS("Receive OSC"));
    toggleReceiveOsc->addListener (this);

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

    groupLog.reset (new GroupComponent ("groupLog",
                                        TRANS("Log")));
    addAndMakeVisible (groupLog.get());

    textLog.reset (new TextEditor ("textLog"));
    addAndMakeVisible (textLog.get());
    textLog->setMultiLine (true);
    textLog->setReturnKeyStartsNewLine (false);
    textLog->setReadOnly (true);
    textLog->setScrollbarsShown (false);
    textLog->setCaretVisible (false);
    textLog->setPopupMenuEnabled (true);
    textLog->setText (String());

    toggleLog.reset (new ToggleButton ("toggleLog"));
    addAndMakeVisible (toggleLog.get());
    toggleLog->setButtonText (TRANS("Enable (may slow down OSC handling)"));
    toggleLog->addListener (this);


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
    
    textLog->setEnabled(false);
    //[/Constructor]
}

OSCSettingsComponent::~OSCSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pStatusMessageHandler->unregisterDetailLog();
    //[/Destructor_pre]

    groupExternal = nullptr;
    groupInternal = nullptr;
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
    groupLog = nullptr;
    textLog = nullptr;
    toggleLog = nullptr;


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

    groupExternal->setBounds (8, 8, getWidth() - 16, 96);
    groupInternal->setBounds (8, 112, getWidth() - 16, 96);
    toggleReceiveOsc->setBounds (8 + 14, 8 + 27, 150, 24);
    textOscReceivePort->setBounds (8 + (getWidth() - 16) - 16 - 48, 8 + 27, 48, 24);
    labelOscPort->setBounds (8 + (getWidth() - 16) - 67 - 49, 8 + 27, 49, 24);
    toggleSendOsc->setBounds (8 + 14, 112 + 23, 199, 24);
    textOscSendIp->setBounds (8 + (getWidth() - 16) - 70 - 106, 112 + 23, 106, 24);
    labelOscSendIp->setBounds (8 + (getWidth() - 16) - 180 - 124, 112 + 23, 124, 24);
    textOscSendPort->setBounds (8 + (getWidth() - 16) - 16 - 48, 112 + 23, 48, 24);
    textOscSendInterval->setBounds (8 + (getWidth() - 16) - 16 - 48, 112 + 53, 48, 24);
    labelOscSendInterval->setBounds (8 + (getWidth() - 16) - 68 - 110, 112 + 53, 110, 24);
    toggleSendOscExt->setBounds (8 + 14, 8 + 57, 245, 24);
    textOscSendIpExt->setBounds (8 + (getWidth() - 16) - 70 - 106, 8 + 57, 106, 24);
    labelOscSendIpExt->setBounds (8 + (getWidth() - 16) - 180 - 126, 8 + 57, 126, 24);
    textOscSendPortExt->setBounds (8 + (getWidth() - 16) - 16 - 48, 8 + 57, 48, 24);
    groupLog->setBounds (8, 216, getWidth() - 16, getHeight() - 221);
    textLog->setBounds (8 + 16, 216 + 56, (getWidth() - 16) - 30, (getHeight() - 221) - 72);
    toggleLog->setBounds (8 + 16, 216 + 24, 280, 24);
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
    else if (buttonThatWasClicked == toggleLog.get())
    {
        //[UserButtonCode_toggleLog] -- add your button handler code here..
    if(toggleLog->getToggleState())
    {
        textLog->setEnabled(true);
        pStatusMessageHandler->registerDetailLog(textLog.get());
    }
    else{
        pStatusMessageHandler->unregisterDetailLog();
        textLog->setEnabled(false);
    }
        //[/UserButtonCode_toggleLog]
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
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler"
                 variableInitialisers="pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupExternal" id="64cdd18a28c39177" memberName="groupExternal"
                  virtualName="" explicitFocusOrder="0" pos="8 8 16M 96" title="External"/>
  <GROUPCOMPONENT name="groupInternal" id="99fa521234eba1bd" memberName="groupInternal"
                  virtualName="" explicitFocusOrder="0" pos="8 112 16M 96" title="Internal"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="14 27 150 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Receive OSC" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscReceivePort" id="ef3c7d2594795ec7" memberName="textOscReceivePort"
              virtualName="" explicitFocusOrder="0" pos="16Rr 27 48 24" posRelativeX="64cdd18a28c39177"
              posRelativeY="64cdd18a28c39177" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="67Rr 27 49 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOsc" id="309574c60d08c55e" memberName="toggleSendOsc"
                virtualName="" explicitFocusOrder="0" pos="14 23 199 24" posRelativeX="99fa521234eba1bd"
                posRelativeY="99fa521234eba1bd" buttonText="Send Positions to Decoder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIp" id="f5d167c87a68db4c" memberName="textOscSendIp"
              virtualName="" explicitFocusOrder="0" pos="70Rr 23 106 24" posRelativeX="99fa521234eba1bd"
              posRelativeY="99fa521234eba1bd" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIp" id="ba6c86d4711d8bd0" memberName="labelOscSendIp"
         virtualName="" explicitFocusOrder="0" pos="180Rr 23 124 24" posRelativeX="99fa521234eba1bd"
         posRelativeY="99fa521234eba1bd" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textOscSendPort" id="c2dff2f42ed301d1" memberName="textOscSendPort"
              virtualName="" explicitFocusOrder="0" pos="16Rr 23 48 24" posRelativeX="99fa521234eba1bd"
              posRelativeY="99fa521234eba1bd" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textOscSendInterval" id="2c20c78495fbd07c" memberName="textOscSendInterval"
              virtualName="" explicitFocusOrder="0" pos="16Rr 53 48 24" posRelativeX="99fa521234eba1bd"
              posRelativeY="99fa521234eba1bd" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendInterval" id="104391e6ca4d3f12" memberName="labelOscSendInterval"
         virtualName="" explicitFocusOrder="0" pos="68Rr 53 110 24" posRelativeX="99fa521234eba1bd"
         posRelativeY="99fa521234eba1bd" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOscExt" id="4c50ad9f06c0feaf" memberName="toggleSendOscExt"
                virtualName="" explicitFocusOrder="0" pos="14 57 245 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Send Positions for External Usage"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIpExt" id="ed3cbf6e9d145d08" memberName="textOscSendIpExt"
              virtualName="" explicitFocusOrder="0" pos="70Rr 57 106 24" posRelativeX="64cdd18a28c39177"
              posRelativeY="64cdd18a28c39177" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExt" id="ee4b90142f7a53f4" memberName="labelOscSendIpExt"
         virtualName="" explicitFocusOrder="0" pos="180Rr 57 126 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textOscSendPortExt" id="8f6b9c97cd3fa86f" memberName="textOscSendPortExt"
              virtualName="" explicitFocusOrder="0" pos="16Rr 57 48 24" posRelativeX="64cdd18a28c39177"
              posRelativeY="64cdd18a28c39177" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <GROUPCOMPONENT name="groupLog" id="1f10ffbcf9f07a8c" memberName="groupLog" virtualName=""
                  explicitFocusOrder="0" pos="8 216 16M 221M" title="Log"/>
  <TEXTEDITOR name="textLog" id="c68a1d41e12fc01" memberName="textLog" virtualName=""
              explicitFocusOrder="0" pos="16 56 30M 72M" posRelativeX="1f10ffbcf9f07a8c"
              posRelativeY="1f10ffbcf9f07a8c" posRelativeW="1f10ffbcf9f07a8c"
              posRelativeH="1f10ffbcf9f07a8c" initialText="" multiline="1"
              retKeyStartsLine="0" readonly="1" scrollbars="0" caret="0" popupmenu="1"/>
  <TOGGLEBUTTON name="toggleLog" id="3acf4be5e0798efd" memberName="toggleLog"
                virtualName="" explicitFocusOrder="0" pos="16 24 280 24" posRelativeX="1f10ffbcf9f07a8c"
                posRelativeY="1f10ffbcf9f07a8c" buttonText="Enable (may slow down OSC handling)"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

