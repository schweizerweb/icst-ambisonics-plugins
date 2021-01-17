/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "OSCTargetsComponent.h"
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

    groupExternal.reset (new juce::GroupComponent ("groupExternal",
                                                   TRANS("External")));
    addAndMakeVisible (groupExternal.get());

    groupInternal.reset (new juce::GroupComponent ("groupInternal",
                                                   TRANS("Internal")));
    addAndMakeVisible (groupInternal.get());

    toggleReceiveOsc.reset (new juce::ToggleButton ("toggleReceiveOsc"));
    addAndMakeVisible (toggleReceiveOsc.get());
    toggleReceiveOsc->setButtonText (TRANS("Receive OSC"));
    toggleReceiveOsc->addListener (this);

    labelOscPort.reset (new juce::Label ("labelOscPort",
                                         TRANS("Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscPort->setJustificationType (juce::Justification::centredRight);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleSendOsc.reset (new juce::ToggleButton ("toggleSendOsc"));
    addAndMakeVisible (toggleSendOsc.get());
    toggleSendOsc->setButtonText (TRANS("Send Positions to Decoder"));
    toggleSendOsc->addListener (this);

    textOscSendIp.reset (new juce::TextEditor ("textOscSendIp"));
    addAndMakeVisible (textOscSendIp.get());
    textOscSendIp->setMultiLine (false);
    textOscSendIp->setReturnKeyStartsNewLine (false);
    textOscSendIp->setReadOnly (false);
    textOscSendIp->setScrollbarsShown (true);
    textOscSendIp->setCaretVisible (true);
    textOscSendIp->setPopupMenuEnabled (true);
    textOscSendIp->setText (juce::String());

    labelOscSendIp.reset (new juce::Label ("labelOscSendIp",
                                           TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIp.get());
    labelOscSendIp->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIp->setJustificationType (juce::Justification::centredRight);
    labelOscSendIp->setEditable (false, false, false);
    labelOscSendIp->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendIp->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelOscSendInterval.reset (new juce::Label ("labelOscSendInterval",
                                                 TRANS("Interval [ms]:")));
    addAndMakeVisible (labelOscSendInterval.get());
    labelOscSendInterval->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendInterval->setJustificationType (juce::Justification::centredRight);
    labelOscSendInterval->setEditable (false, false, false);
    labelOscSendInterval->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendInterval->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleSendOscExt.reset (new juce::ToggleButton ("toggleSendOscExt"));
    addAndMakeVisible (toggleSendOscExt.get());
    toggleSendOscExt->setButtonText (TRANS("Send Positions for External Usage"));
    toggleSendOscExt->addListener (this);

    groupLog.reset (new juce::GroupComponent ("groupLog",
                                              TRANS("Log")));
    addAndMakeVisible (groupLog.get());

    textLog.reset (new juce::TextEditor ("textLog"));
    addAndMakeVisible (textLog.get());
    textLog->setMultiLine (true);
    textLog->setReturnKeyStartsNewLine (false);
    textLog->setReadOnly (true);
    textLog->setScrollbarsShown (false);
    textLog->setCaretVisible (false);
    textLog->setPopupMenuEnabled (true);
    textLog->setText (juce::String());

    toggleLog.reset (new juce::ToggleButton ("toggleLog"));
    addAndMakeVisible (toggleLog.get());
    toggleLog->setButtonText (TRANS("Enable (may slow down OSC handling)"));
    toggleLog->addListener (this);

    labelExternalOscInfo.reset (new juce::Label ("labelExternalOscInfo",
                                                 TRANS("no active targets")));
    addAndMakeVisible (labelExternalOscInfo.get());
    labelExternalOscInfo->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelExternalOscInfo->setJustificationType (juce::Justification::centredRight);
    labelExternalOscInfo->setEditable (false, false, false);
    labelExternalOscInfo->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExternalOscInfo->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnEdit.reset (new juce::TextButton ("btnEdit"));
    addAndMakeVisible (btnEdit.get());
    btnEdit->setButtonText (TRANS("edit..."));
    btnEdit->addListener (this);

    sliderReceiveOscPort.reset (new juce::Slider ("sliderReceiveOscPort"));
    addAndMakeVisible (sliderReceiveOscPort.get());
    sliderReceiveOscPort->setRange (0, 65535, 1);
    sliderReceiveOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderReceiveOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderReceiveOscPort->addListener (this);

    sliderSendOscPort.reset (new juce::Slider ("sliderSendOscPort"));
    addAndMakeVisible (sliderSendOscPort.get());
    sliderSendOscPort->setRange (0, 65535, 1);
    sliderSendOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderSendOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderSendOscPort->addListener (this);

    sliderInterval.reset (new juce::Slider ("sliderInterval"));
    addAndMakeVisible (sliderInterval.get());
    sliderInterval->setRange (10, 10000, 1);
    sliderInterval->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderInterval->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderInterval->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    textOscSendIp->addListener(this);
    sliderReceiveOscPort->setSkewFactorFromMidPoint(50.0);
    sliderSendOscPort->setSkewFactorFromMidPoint(50.0);

    toggleReceiveOsc->setToggleState(pSettings->oscReceiveFlag, dontSendNotification);
    sliderReceiveOscPort->setValue(pSettings->oscReceivePort, dontSendNotification);

    toggleSendOsc->setToggleState(pSettings->oscSendFlag, dontSendNotification);
    textOscSendIp->setText(String(pSettings->oscSendTargetHost), false);
    sliderSendOscPort->setValue(pSettings->oscSendPort, dontSendNotification);
    sliderInterval->setValue(pSettings->oscSendIntervalMs, dontSendNotification);

    toggleSendOscExt->setToggleState(pSettings->oscSendExtMasterFlag, dontSendNotification);

    textLog->setEnabled(false);
    controlDimming();
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
    labelOscPort = nullptr;
    toggleSendOsc = nullptr;
    textOscSendIp = nullptr;
    labelOscSendIp = nullptr;
    labelOscSendInterval = nullptr;
    toggleSendOscExt = nullptr;
    groupLog = nullptr;
    textLog = nullptr;
    toggleLog = nullptr;
    labelExternalOscInfo = nullptr;
    btnEdit = nullptr;
    sliderReceiveOscPort = nullptr;
    sliderSendOscPort = nullptr;
    sliderInterval = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

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
    labelOscPort->setBounds (8 + (getWidth() - 16) - 108 - 49, 8 + 27, 49, 24);
    toggleSendOsc->setBounds (8 + 14, 112 + 27, 199, 24);
    textOscSendIp->setBounds (8 + (getWidth() - 16) - 111 - 106, 112 + 27, 106, 24);
    labelOscSendIp->setBounds (8 + (getWidth() - 16) - 221 - 124, 112 + 27, 124, 24);
    labelOscSendInterval->setBounds (8 + (getWidth() - 16) - 221 - 110, 112 + 57, 110, 24);
    toggleSendOscExt->setBounds (8 + 14, 8 + 57, 245, 24);
    groupLog->setBounds (8, 216, getWidth() - 16, getHeight() - 221);
    textLog->setBounds (8 + 16, 216 + 56, (getWidth() - 16) - 30, (getHeight() - 221) - 72);
    toggleLog->setBounds (8 + 16, 216 + 24, 280, 24);
    labelExternalOscInfo->setBounds (8 + (getWidth() - 16) - 115 - 224, 8 + 57, 224, 24);
    btnEdit->setBounds (8 + (getWidth() - 16) - 16 - 86, 8 + 57, 86, 24);
    sliderReceiveOscPort->setBounds (8 + (getWidth() - 16) - 16 - 86, 8 + 27, 86, 24);
    sliderSendOscPort->setBounds (8 + (getWidth() - 16) - 16 - 86, 112 + 27, 86, 24);
    sliderInterval->setBounds (8 + (getWidth() - 16) - 16 - 200, 112 + 57, 200, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
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
        pSettings->oscSendExtMasterFlag = toggleSendOscExt->getToggleState();
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
    else if (buttonThatWasClicked == btnEdit.get())
    {
        //[UserButtonCode_btnEdit] -- add your button handler code here..
        CallOutBox::launchAsynchronously(std::make_unique<OSCTargetsComponent>(this, pSettings), getScreenBounds(), this);
        //[/UserButtonCode_btnEdit]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void OSCSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderReceiveOscPort.get())
    {
        //[UserSliderCode_sliderReceiveOscPort] -- add your slider handling code here..
        pSettings->oscReceivePort = int(sliderReceiveOscPort->getValue());
        //[/UserSliderCode_sliderReceiveOscPort]
    }
    else if (sliderThatWasMoved == sliderSendOscPort.get())
    {
        //[UserSliderCode_sliderSendOscPort] -- add your slider handling code here..
        pSettings->oscSendPort = int(sliderSendOscPort->getValue());
        //[/UserSliderCode_sliderSendOscPort]
    }
    else if (sliderThatWasMoved == sliderInterval.get())
    {
        //[UserSliderCode_sliderInterval] -- add your slider handling code here..
        pSettings->oscSendIntervalMs = int(sliderInterval->getValue());
        //[/UserSliderCode_sliderInterval]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void OSCSettingsComponent::textEditorTextChanged(TextEditor& textEditor)
{
    if (&textEditor == textOscSendIp.get())
        pSettings->oscSendTargetHost = textEditor.getText();

    sendChangeMessage();
}

void OSCSettingsComponent::controlDimming() const
{
    sliderReceiveOscPort->setEnabled(pSettings->oscReceiveFlag);
    textOscSendIp->setEnabled(pSettings->oscSendFlag);
    sliderSendOscPort->setEnabled(pSettings->oscSendFlag);
    sliderInterval->setEnabled(pSettings->oscSendFlag);
    btnEdit->setEnabled(pSettings->oscSendExtMasterFlag);
    labelExternalOscInfo->setEnabled(pSettings->oscSendExtMasterFlag);
    int targetCount = (pSettings->oscSendExtXyzFlag ? 1 : 0) + (pSettings->oscSendExtAedFlag ? 1 : 0);
    for (auto target : pSettings->customOscTargets)
    {
        if (target->enabledFlag) targetCount++;
    }

    Colour textColor = pSettings->oscSendExtMasterFlag ? (targetCount > 0 ? Colours::green : Colours::orangered) : Colours::grey;
    labelExternalOscInfo->setText((targetCount > 0 ? (String(targetCount) + " active targets") : "no active targets"), dontSendNotification);
    labelExternalOscInfo->setColour(Label::textColourId, textColor);
}

void OSCSettingsComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    controlDimming();
    sendChangeMessage();
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCSettingsComponent" componentName=""
                 parentClasses="public Component, public TextEditor::Listener, public ChangeBroadcaster, public ChangeListener"
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
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="108Rr 27 49 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOsc" id="309574c60d08c55e" memberName="toggleSendOsc"
                virtualName="" explicitFocusOrder="0" pos="14 27 199 24" posRelativeX="99fa521234eba1bd"
                posRelativeY="99fa521234eba1bd" buttonText="Send Positions to Decoder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIp" id="f5d167c87a68db4c" memberName="textOscSendIp"
              virtualName="" explicitFocusOrder="0" pos="111Rr 27 106 24" posRelativeX="99fa521234eba1bd"
              posRelativeY="99fa521234eba1bd" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIp" id="ba6c86d4711d8bd0" memberName="labelOscSendIp"
         virtualName="" explicitFocusOrder="0" pos="221Rr 27 124 24" posRelativeX="99fa521234eba1bd"
         posRelativeY="99fa521234eba1bd" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="labelOscSendInterval" id="104391e6ca4d3f12" memberName="labelOscSendInterval"
         virtualName="" explicitFocusOrder="0" pos="221Rr 57 110 24" posRelativeX="99fa521234eba1bd"
         posRelativeY="99fa521234eba1bd" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOscExt" id="4c50ad9f06c0feaf" memberName="toggleSendOscExt"
                virtualName="" explicitFocusOrder="0" pos="14 57 245 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Send Positions for External Usage"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
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
  <LABEL name="labelExternalOscInfo" id="d8016ede58eb1df8" memberName="labelExternalOscInfo"
         virtualName="" explicitFocusOrder="0" pos="115Rr 57 224 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="no active targets" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="btnEdit" id="39868884fdb013a6" memberName="btnEdit" virtualName=""
              explicitFocusOrder="0" pos="16Rr 57 86 24" posRelativeX="64cdd18a28c39177"
              posRelativeY="64cdd18a28c39177" buttonText="edit..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="sliderReceiveOscPort" id="591bcc850e858bff" memberName="sliderReceiveOscPort"
          virtualName="" explicitFocusOrder="0" pos="16Rr 27 86 24" posRelativeX="64cdd18a28c39177"
          posRelativeY="64cdd18a28c39177" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderSendOscPort" id="b9eb26813403a118" memberName="sliderSendOscPort"
          virtualName="" explicitFocusOrder="0" pos="16Rr 27 86 24" posRelativeX="99fa521234eba1bd"
          posRelativeY="99fa521234eba1bd" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderInterval" id="d02cfad6a67a7536" memberName="sliderInterval"
          virtualName="" explicitFocusOrder="0" pos="16Rr 57 200 24" posRelativeX="99fa521234eba1bd"
          posRelativeY="99fa521234eba1bd" min="10.0" max="10000.0" int="1.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

