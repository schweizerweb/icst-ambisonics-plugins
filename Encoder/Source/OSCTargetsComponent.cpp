/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "OSCTargetsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCTargetsComponent::OSCTargetsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings)
    : pSettings(pSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    //[/Constructor_pre]

    groupStandard.reset (new juce::GroupComponent ("groupStandard",
                                                   TRANS("ICST AmbiPlugins Standard OSC")));
    addAndMakeVisible (groupStandard.get());

    toggleEnableStandardXyz.reset (new juce::ToggleButton ("toggleEnableStandardXyz"));
    addAndMakeVisible (toggleEnableStandardXyz.get());
    toggleEnableStandardXyz->setButtonText (TRANS("Enable XYZ"));
    toggleEnableStandardXyz->addListener (this);

    textOscSendIpExtXyz.reset (new juce::TextEditor ("textOscSendIpExtXyz"));
    addAndMakeVisible (textOscSendIpExtXyz.get());
    textOscSendIpExtXyz->setMultiLine (false);
    textOscSendIpExtXyz->setReturnKeyStartsNewLine (false);
    textOscSendIpExtXyz->setReadOnly (false);
    textOscSendIpExtXyz->setScrollbarsShown (true);
    textOscSendIpExtXyz->setCaretVisible (true);
    textOscSendIpExtXyz->setPopupMenuEnabled (true);
    textOscSendIpExtXyz->setText (juce::String());

    labelOscSendIpExt.reset (new juce::Label ("labelOscSendIpExt",
                                              TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIpExt.get());
    labelOscSendIpExt->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIpExt->setJustificationType (juce::Justification::centredRight);
    labelOscSendIpExt->setEditable (false, false, false);
    labelOscSendIpExt->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendIpExt->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    groupCustom.reset (new juce::GroupComponent ("groupCustom",
                                                 TRANS("Custom OSC")));
    addAndMakeVisible (groupCustom.get());

    targetList.reset (new TableListBox());
    addAndMakeVisible (targetList.get());
    targetList->setName ("targetList");

    groupGeneral.reset (new juce::GroupComponent ("groupGeneral",
                                                  TRANS("General")));
    addAndMakeVisible (groupGeneral.get());

    sliderInterval.reset (new juce::Slider ("sliderInterval"));
    addAndMakeVisible (sliderInterval.get());
    sliderInterval->setRange (10, 10000, 1);
    sliderInterval->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderInterval->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderInterval->addListener (this);

    labelInterval.reset (new juce::Label ("labelInterval",
                                          TRANS("Interval [ms]")));
    addAndMakeVisible (labelInterval.get());
    labelInterval->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelInterval->setJustificationType (juce::Justification::centredLeft);
    labelInterval->setEditable (false, false, false);
    labelInterval->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelInterval->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleEnableStandardAed.reset (new juce::ToggleButton ("toggleEnableStandardAed"));
    addAndMakeVisible (toggleEnableStandardAed.get());
    toggleEnableStandardAed->setButtonText (TRANS("Enable AED"));
    toggleEnableStandardAed->addListener (this);

    textOscSendIpExtAed.reset (new juce::TextEditor ("textOscSendIpExtAed"));
    addAndMakeVisible (textOscSendIpExtAed.get());
    textOscSendIpExtAed->setMultiLine (false);
    textOscSendIpExtAed->setReturnKeyStartsNewLine (false);
    textOscSendIpExtAed->setReadOnly (false);
    textOscSendIpExtAed->setScrollbarsShown (true);
    textOscSendIpExtAed->setCaretVisible (true);
    textOscSendIpExtAed->setPopupMenuEnabled (true);
    textOscSendIpExtAed->setText (juce::String());

    labelOscSendIpExt2.reset (new juce::Label ("labelOscSendIpExt",
                                               TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIpExt2.get());
    labelOscSendIpExt2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIpExt2->setJustificationType (juce::Justification::centredRight);
    labelOscSendIpExt2->setEditable (false, false, false);
    labelOscSendIpExt2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendIpExt2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderPortExtXyz.reset (new juce::Slider ("sliderPortExtXyz"));
    addAndMakeVisible (sliderPortExtXyz.get());
    sliderPortExtXyz->setRange (0, 65535, 1);
    sliderPortExtXyz->setSliderStyle (juce::Slider::IncDecButtons);
    sliderPortExtXyz->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderPortExtXyz->addListener (this);

    sliderPortExtAed.reset (new juce::Slider ("sliderPortExtAed"));
    addAndMakeVisible (sliderPortExtAed.get());
    sliderPortExtAed->setRange (0, 65535, 1);
    sliderPortExtAed->setSliderStyle (juce::Slider::IncDecButtons);
    sliderPortExtAed->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderPortExtAed->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sliderInterval->setSkewFactorFromMidPoint(50.0);
    sliderInterval->setValue(pSettings->oscSendExtIntervalMs);

    toggleEnableStandardXyz->setToggleState(pSettings->oscSendExtXyzFlag, dontSendNotification);
    textOscSendIpExtXyz->setText(pSettings->oscSendExtXyzHost, false);
    sliderPortExtXyz->setValue(pSettings->oscSendExtXyzPort, dontSendNotification);

    toggleEnableStandardAed->setToggleState(pSettings->oscSendExtAedFlag, dontSendNotification);
    textOscSendIpExtAed->setText(pSettings->oscSendExtAedHost, false);
    sliderPortExtAed->setValue(pSettings->oscSendExtAedPort, dontSendNotification);

    controlDimming();
    //[/Constructor]
}

OSCTargetsComponent::~OSCTargetsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupStandard = nullptr;
    toggleEnableStandardXyz = nullptr;
    textOscSendIpExtXyz = nullptr;
    labelOscSendIpExt = nullptr;
    groupCustom = nullptr;
    targetList = nullptr;
    groupGeneral = nullptr;
    sliderInterval = nullptr;
    labelInterval = nullptr;
    toggleEnableStandardAed = nullptr;
    textOscSendIpExtAed = nullptr;
    labelOscSendIpExt2 = nullptr;
    sliderPortExtXyz = nullptr;
    sliderPortExtAed = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCTargetsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCTargetsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupStandard->setBounds (0, 72, getWidth() - 0, 96);
    toggleEnableStandardXyz->setBounds (0 + 16, 72 + 24, 150, 24);
    textOscSendIpExtXyz->setBounds (0 + (getWidth() - 0) - 109 - 106, 72 + 24, 106, 24);
    labelOscSendIpExt->setBounds (0 + (getWidth() - 0) - 219 - 126, 72 + 24, 126, 24);
    groupCustom->setBounds (0, 176, getWidth() - 0, getHeight() - 176);
    targetList->setBounds (0 + 16, 176 + 24, (getWidth() - 0) - 32, (getHeight() - 176) - 40);
    groupGeneral->setBounds (0, 0, getWidth() - 0, 64);
    sliderInterval->setBounds (0 + (getWidth() - 0) - 16 - 270, 0 + 24, 270, 24);
    labelInterval->setBounds (0 + 16, 0 + 24, 150, 24);
    toggleEnableStandardAed->setBounds (0 + 16, 72 + 55, 150, 24);
    textOscSendIpExtAed->setBounds (0 + (getWidth() - 0) - 109 - 106, 72 + 55, 106, 24);
    labelOscSendIpExt2->setBounds (0 + (getWidth() - 0) - 219 - 126, 72 + 55, 126, 24);
    sliderPortExtXyz->setBounds (0 + (getWidth() - 0) - 16 - 86, 72 + 24, 86, 24);
    sliderPortExtAed->setBounds (0 + (getWidth() - 0) - 16 - 86, 72 + 56, 86, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCTargetsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleEnableStandardXyz.get())
    {
        //[UserButtonCode_toggleEnableStandardXyz] -- add your button handler code here..
        pSettings->oscSendExtXyzFlag = toggleEnableStandardXyz->getToggleState();
        //[/UserButtonCode_toggleEnableStandardXyz]
    }
    else if (buttonThatWasClicked == toggleEnableStandardAed.get())
    {
        //[UserButtonCode_toggleEnableStandardAed] -- add your button handler code here..
        pSettings->oscSendExtAedFlag = toggleEnableStandardAed->getToggleState();
        //[/UserButtonCode_toggleEnableStandardAed]
    }

    //[UserbuttonClicked_Post]
    sendChangeMessage();
    controlDimming();
    //[/UserbuttonClicked_Post]
}

void OSCTargetsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderInterval.get())
    {
        //[UserSliderCode_sliderInterval] -- add your slider handling code here..
        pSettings->oscSendExtIntervalMs = int(sliderInterval->getValue());
        //[/UserSliderCode_sliderInterval]
    }
    else if (sliderThatWasMoved == sliderPortExtXyz.get())
    {
        //[UserSliderCode_sliderPortExtXyz] -- add your slider handling code here..
        pSettings->oscSendExtXyzPort = int(sliderPortExtXyz->getValue());
        //[/UserSliderCode_sliderPortExtXyz]
    }
    else if (sliderThatWasMoved == sliderPortExtAed.get())
    {
        //[UserSliderCode_sliderPortExtAed] -- add your slider handling code here..
        pSettings->oscSendExtAedPort = int(sliderPortExtAed->getValue());
        //[/UserSliderCode_sliderPortExtAed]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OSCTargetsComponent::controlDimming()
{
    textOscSendIpExtXyz->setEnabled(toggleEnableStandardXyz->getToggleState());
    textOscSendIpExtAed->setEnabled(toggleEnableStandardAed->getToggleState());

    sliderPortExtXyz->setEnabled(toggleEnableStandardXyz->getToggleState());
    sliderPortExtAed->setEnabled(toggleEnableStandardAed->getToggleState());
}

void OSCTargetsComponent::textEditorTextChanged(TextEditor& textEditor)
{
    if (&textEditor == textOscSendIpExtXyz.get())
        pSettings->oscSendExtXyzHost = textOscSendIpExtXyz->getText();

    if (&textEditor == textOscSendIpExtAed.get())
        pSettings->oscSendExtAedHost = textOscSendIpExtAed->getText();

    sendChangeMessage();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCTargetsComponent" componentName=""
                 parentClasses="public juce::Component, public ChangeBroadcaster, public TextEditor::Listener"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings"
                 variableInitialisers="pSettings(pSettings)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupStandard" id="a452293a001780b9" memberName="groupStandard"
                  virtualName="" explicitFocusOrder="0" pos="0 72 0M 96" title="ICST AmbiPlugins Standard OSC"/>
  <TOGGLEBUTTON name="toggleEnableStandardXyz" id="81256bcfb1a0151d" memberName="toggleEnableStandardXyz"
                virtualName="" explicitFocusOrder="0" pos="16 24 150 24" posRelativeX="a452293a001780b9"
                posRelativeY="a452293a001780b9" buttonText="Enable XYZ" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIpExtXyz" id="ed3cbf6e9d145d08" memberName="textOscSendIpExtXyz"
              virtualName="" explicitFocusOrder="0" pos="109Rr 24 106 24" posRelativeX="a452293a001780b9"
              posRelativeY="a452293a001780b9" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExt" id="ee4b90142f7a53f4" memberName="labelOscSendIpExt"
         virtualName="" explicitFocusOrder="0" pos="219Rr 24 126 24" posRelativeX="a452293a001780b9"
         posRelativeY="a452293a001780b9" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <GROUPCOMPONENT name="groupCustom" id="5ccced30e0050e9" memberName="groupCustom"
                  virtualName="" explicitFocusOrder="0" pos="0 176 0M 176M" title="Custom OSC"/>
  <GENERICCOMPONENT name="targetList" id="78a64bf0c7700896" memberName="targetList"
                    virtualName="" explicitFocusOrder="0" pos="16 24 32M 40M" posRelativeX="5ccced30e0050e9"
                    posRelativeY="5ccced30e0050e9" posRelativeW="5ccced30e0050e9"
                    posRelativeH="5ccced30e0050e9" class="TableListBox" params=""/>
  <GROUPCOMPONENT name="groupGeneral" id="4e210261f2d82b98" memberName="groupGeneral"
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 64" title="General"/>
  <SLIDER name="sliderInterval" id="d02cfad6a67a7536" memberName="sliderInterval"
          virtualName="" explicitFocusOrder="0" pos="16Rr 24 270 24" posRelativeX="4e210261f2d82b98"
          posRelativeY="4e210261f2d82b98" min="10.0" max="10000.0" int="1.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelInterval" id="d3a3f463e2503e2f" memberName="labelInterval"
         virtualName="" explicitFocusOrder="0" pos="16 24 150 24" posRelativeX="4e210261f2d82b98"
         posRelativeY="4e210261f2d82b98" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="toggleEnableStandardAed" id="5b2f4aed17c34efa" memberName="toggleEnableStandardAed"
                virtualName="" explicitFocusOrder="0" pos="16 55 150 24" posRelativeX="a452293a001780b9"
                posRelativeY="a452293a001780b9" buttonText="Enable AED" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscSendIpExtAed" id="9e5f2789bd7772" memberName="textOscSendIpExtAed"
              virtualName="" explicitFocusOrder="0" pos="109Rr 55 106 24" posRelativeX="a452293a001780b9"
              posRelativeY="a452293a001780b9" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExt" id="22dbcbf07f7968d2" memberName="labelOscSendIpExt2"
         virtualName="" explicitFocusOrder="0" pos="219Rr 55 126 24" posRelativeX="a452293a001780b9"
         posRelativeY="a452293a001780b9" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="sliderPortExtXyz" id="591bcc850e858bff" memberName="sliderPortExtXyz"
          virtualName="" explicitFocusOrder="0" pos="16Rr 24 86 24" posRelativeX="a452293a001780b9"
          posRelativeY="a452293a001780b9" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderPortExtAed" id="218b7cbec68ee7e8" memberName="sliderPortExtAed"
          virtualName="" explicitFocusOrder="0" pos="16Rr 56 86 24" posRelativeX="a452293a001780b9"
          posRelativeY="a452293a001780b9" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

