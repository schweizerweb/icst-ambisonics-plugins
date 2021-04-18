/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "OSCSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCSettingsComponent::OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, OSCLogDialogManager* pOscLogManager)
    : pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler), pOscLogManager(pOscLogManager)
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

    labelExternalOscInfo.reset (new juce::Label ("labelExternalOscInfo",
                                                 TRANS("no active targets")));
    addAndMakeVisible (labelExternalOscInfo.get());
    labelExternalOscInfo->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelExternalOscInfo->setJustificationType (juce::Justification::centredRight);
    labelExternalOscInfo->setEditable (false, false, false);
    labelExternalOscInfo->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExternalOscInfo->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderReceiveOscPort.reset (new juce::Slider ("sliderReceiveOscPort"));
    addAndMakeVisible (sliderReceiveOscPort.get());
    sliderReceiveOscPort->setRange (0, 65535, 1);
    sliderReceiveOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderReceiveOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderReceiveOscPort->addListener (this);

    sliderSendOscPort.reset (new juce::Slider ("sliderSendOscPort"));
    addAndMakeVisible (sliderSendOscPort.get());
    sliderSendOscPort->setRange (0, 65535, 1);
    sliderSendOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderSendOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderSendOscPort->addListener (this);

    sliderInterval.reset (new juce::Slider ("sliderInterval"));
    addAndMakeVisible (sliderInterval.get());
    sliderInterval->setRange (10, 1000, 1);
    sliderInterval->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderInterval->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderInterval->addListener (this);

    buttonShowOscLog.reset (new juce::TextButton ("buttonShowOscLog"));
    addAndMakeVisible (buttonShowOscLog.get());
    buttonShowOscLog->setButtonText (TRANS("Show OSC Log"));
    buttonShowOscLog->addListener (this);

    oscTargets.reset (new OSCTargetsComponent (this, pSettings));
    addAndMakeVisible (oscTargets.get());
    oscTargets->setName ("oscTargets");

    labelLoadInfo.reset (new juce::Label ("labelLoadInfo",
                                          TRANS("no network traffic")));
    addAndMakeVisible (labelLoadInfo.get());
    labelLoadInfo->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelLoadInfo->setJustificationType (juce::Justification::centredLeft);
    labelLoadInfo->setEditable (false, false, false);
    labelLoadInfo->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelLoadInfo->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    textOscSendIp->addListener(this);
    sliderInterval->setSkewFactorFromMidPoint(200.0);
    toggleReceiveOsc->setToggleState(pSettings->oscReceiveFlag, dontSendNotification);
    sliderReceiveOscPort->setValue(pSettings->oscReceivePort, dontSendNotification);

    toggleSendOsc->setToggleState(pSettings->oscSendFlag, dontSendNotification);
    textOscSendIp->setText(String(pSettings->oscSendTargetHost), false);
    sliderSendOscPort->setValue(pSettings->oscSendPort, dontSendNotification);
    sliderInterval->setValue(pSettings->oscSendIntervalMs, dontSendNotification);

    toggleSendOscExt->setToggleState(pSettings->oscSendExtMasterFlag, dontSendNotification);
    controlDimming();
    //[/Constructor]
}

OSCSettingsComponent::~OSCSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
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
    labelExternalOscInfo = nullptr;
    sliderReceiveOscPort = nullptr;
    sliderSendOscPort = nullptr;
    sliderInterval = nullptr;
    buttonShowOscLog = nullptr;
    oscTargets = nullptr;
    labelLoadInfo = nullptr;


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

    groupExternal->setBounds (8, 113, getWidth() - 16, getHeight() - 155);
    groupInternal->setBounds (8, 8, getWidth() - 16, 96);
    toggleReceiveOsc->setBounds (8 + 14, 113 + 27, 150, 24);
    labelOscPort->setBounds (8 + (getWidth() - 16) - 123 - 49, 113 + 27, 49, 24);
    toggleSendOsc->setBounds (8 + 14, 8 + 27, 199, 24);
    textOscSendIp->setBounds (8 + (getWidth() - 16) - 123 - 106, 8 + 27, 106, 24);
    labelOscSendIp->setBounds (8 + (getWidth() - 16) - 236 - 124, 8 + 27, 124, 24);
    labelOscSendInterval->setBounds (8 + (getWidth() - 16) - 236 - 110, 8 + 57, 110, 24);
    toggleSendOscExt->setBounds (8 + 14, 113 + 57, 245, 24);
    labelExternalOscInfo->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 280), 113 + 57, (getWidth() - 16) - 280, 24);
    sliderReceiveOscPort->setBounds (8 + (getWidth() - 16) - 16 - 100, 113 + 27, 100, 24);
    sliderSendOscPort->setBounds (8 + (getWidth() - 16) - 16 - 100, 8 + 27, 100, 24);
    sliderInterval->setBounds (8 + (getWidth() - 16) - 16 - 210, 8 + 57, 210, 24);
    buttonShowOscLog->setBounds (getWidth() - 8 - 150, getHeight() - 8 - 24, 150, 24);
    oscTargets->setBounds (8 + 24, 113 + 87, (getWidth() - 16) - 32, (getHeight() - 155) - 95);
    labelLoadInfo->setBounds (8, getHeight() - 8 - 24, getWidth() - 182, 24);
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
    else if (buttonThatWasClicked == buttonShowOscLog.get())
    {
        //[UserButtonCode_buttonShowOscLog] -- add your button handler code here..
        pOscLogManager->show(pStatusMessageHandler, this);
        //[/UserButtonCode_buttonShowOscLog]
    }

    //[UserbuttonClicked_Post]
    controlDimming();
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
    controlDimming();
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
    labelOscSendIp->setEnabled(pSettings->oscSendFlag);
    labelOscSendInterval->setEnabled(pSettings->oscSendFlag);
    textOscSendIp->setEnabled(pSettings->oscSendFlag);
    sliderSendOscPort->setEnabled(pSettings->oscSendFlag);
    sliderInterval->setEnabled(pSettings->oscSendFlag);
    oscTargets->setEnabled(pSettings->oscSendExtMasterFlag);
    labelExternalOscInfo->setEnabled(pSettings->oscSendExtMasterFlag);
    int targetCount = 0;
    if(pSettings->oscSendExtMasterFlag)
    {
        targetCount += (pSettings->oscSendExtXyz->enabledFlag ? 1 : 0) + (pSettings->oscSendExtAed->enabledFlag ? 1 : 0) +
            (pSettings->oscSendExtXyzIndex->enabledFlag ? 1 : 0) +
            (pSettings->oscSendExtAedIndex->enabledFlag ? 1 : 0);
        for (auto target : pSettings->customOscTargets)
        {
            if (target->enabledFlag) targetCount++;
        }
    }

    Colour textColor = pSettings->oscSendExtMasterFlag ? (targetCount > 0 ? Colours::green : Colours::orangered) : Colours::grey;
    labelExternalOscInfo->setText((targetCount > 0 ? (String(targetCount) + " active targets") : "no active targets"), dontSendNotification);
    labelExternalOscInfo->setColour(Label::textColourId, textColor);

    double packetsPerSecond = targetCount / (pSettings->oscSendExtIntervalMs * 0.001) + (pSettings->oscSendFlag ? (1.0 / (pSettings->oscSendIntervalMs * 0.001)) : 0.0);
    String info = packetsPerSecond == 0.0
        ? "no network traffic"
        : ("estimated network traffic: " + String(!pSettings->oscSendExtContinuousFlag ? "max. " : "") + (packetsPerSecond < 1.0
                                            ? "<1/s"
                                            : String(roundToInt(packetsPerSecond)) + "/s"));
    labelLoadInfo->setText(info, dontSendNotification);
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
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, OSCLogDialogManager* pOscLogManager"
                 variableInitialisers="pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler), pOscLogManager(pOscLogManager)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupExternal" id="64cdd18a28c39177" memberName="groupExternal"
                  virtualName="" explicitFocusOrder="0" pos="8 113 16M 155M" title="External"/>
  <GROUPCOMPONENT name="groupInternal" id="99fa521234eba1bd" memberName="groupInternal"
                  virtualName="" explicitFocusOrder="0" pos="8 8 16M 96" title="Internal"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="14 27 150 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Receive OSC" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="123Rr 27 49 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOsc" id="309574c60d08c55e" memberName="toggleSendOsc"
                virtualName="" explicitFocusOrder="0" pos="14 27 199 24" posRelativeX="99fa521234eba1bd"
                posRelativeY="99fa521234eba1bd" buttonText="Send Positions to Decoder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIp" id="f5d167c87a68db4c" memberName="textOscSendIp"
              virtualName="" explicitFocusOrder="0" pos="123Rr 27 106 24" posRelativeX="99fa521234eba1bd"
              posRelativeY="99fa521234eba1bd" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIp" id="ba6c86d4711d8bd0" memberName="labelOscSendIp"
         virtualName="" explicitFocusOrder="0" pos="236Rr 27 124 24" posRelativeX="99fa521234eba1bd"
         posRelativeY="99fa521234eba1bd" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="labelOscSendInterval" id="104391e6ca4d3f12" memberName="labelOscSendInterval"
         virtualName="" explicitFocusOrder="0" pos="236Rr 57 110 24" posRelativeX="99fa521234eba1bd"
         posRelativeY="99fa521234eba1bd" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="toggleSendOscExt" id="4c50ad9f06c0feaf" memberName="toggleSendOscExt"
                virtualName="" explicitFocusOrder="0" pos="14 57 245 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Send Positions for External Usage"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelExternalOscInfo" id="d8016ede58eb1df8" memberName="labelExternalOscInfo"
         virtualName="" explicitFocusOrder="0" pos="8Rr 57 280M 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" posRelativeW="64cdd18a28c39177"
         edTextCol="ff000000" edBkgCol="0" labelText="no active targets"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <SLIDER name="sliderReceiveOscPort" id="591bcc850e858bff" memberName="sliderReceiveOscPort"
          virtualName="" explicitFocusOrder="0" pos="16Rr 27 100 24" posRelativeX="64cdd18a28c39177"
          posRelativeY="64cdd18a28c39177" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderSendOscPort" id="b9eb26813403a118" memberName="sliderSendOscPort"
          virtualName="" explicitFocusOrder="0" pos="16Rr 27 100 24" posRelativeX="99fa521234eba1bd"
          posRelativeY="99fa521234eba1bd" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderInterval" id="d02cfad6a67a7536" memberName="sliderInterval"
          virtualName="" explicitFocusOrder="0" pos="16Rr 57 210 24" posRelativeX="99fa521234eba1bd"
          posRelativeY="99fa521234eba1bd" min="10.0" max="1000.0" int="1.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="buttonShowOscLog" id="680b48d522ce99b2" memberName="buttonShowOscLog"
              virtualName="" explicitFocusOrder="0" pos="8Rr 8Rr 150 24" buttonText="Show OSC Log"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="oscTargets" id="cd238de4b9ac3b7e" memberName="oscTargets"
                    virtualName="" explicitFocusOrder="0" pos="24 87 32M 95M" posRelativeX="64cdd18a28c39177"
                    posRelativeY="64cdd18a28c39177" posRelativeW="64cdd18a28c39177"
                    posRelativeH="64cdd18a28c39177" class="OSCTargetsComponent" params="this, pSettings"/>
  <LABEL name="labelLoadInfo" id="32af9a6495d86f3e" memberName="labelLoadInfo"
         virtualName="" explicitFocusOrder="0" pos="8 8Rr 182M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="no network traffic" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

