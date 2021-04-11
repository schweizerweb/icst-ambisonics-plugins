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
    customOscTableModel.reset(new CustomOscTableListModel(pSettings, this, this));
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

    labelOscSendIpExtXyz.reset (new juce::Label ("labelOscSendIpExtXyz",
                                                 TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIpExtXyz.get());
    labelOscSendIpExtXyz->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIpExtXyz->setJustificationType (juce::Justification::centredRight);
    labelOscSendIpExtXyz->setEditable (false, false, false);
    labelOscSendIpExtXyz->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendIpExtXyz->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

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

    labelOscSendIpExtAed.reset (new juce::Label ("labelOscSendIpExtAed",
                                                 TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIpExtAed.get());
    labelOscSendIpExtAed->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelOscSendIpExtAed->setJustificationType (juce::Justification::centredRight);
    labelOscSendIpExtAed->setEditable (false, false, false);
    labelOscSendIpExtAed->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendIpExtAed->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderPortExtXyz.reset (new juce::Slider ("sliderPortExtXyz"));
    addAndMakeVisible (sliderPortExtXyz.get());
    sliderPortExtXyz->setRange (0, 65535, 1);
    sliderPortExtXyz->setSliderStyle (juce::Slider::IncDecButtons);
    sliderPortExtXyz->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderPortExtXyz->addListener (this);

    sliderPortExtAed.reset (new juce::Slider ("sliderPortExtAed"));
    addAndMakeVisible (sliderPortExtAed.get());
    sliderPortExtAed->setRange (0, 65535, 1);
    sliderPortExtAed->setSliderStyle (juce::Slider::IncDecButtons);
    sliderPortExtAed->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderPortExtAed->addListener (this);

    btnAdd.reset (new juce::TextButton ("btnAdd"));
    addAndMakeVisible (btnAdd.get());
    btnAdd->setButtonText (TRANS("add"));
    btnAdd->addListener (this);

    btnDelete.reset (new juce::TextButton ("btnDelete"));
    addAndMakeVisible (btnDelete.get());
    btnDelete->setButtonText (TRANS("delete"));
    btnDelete->addListener (this);

    btnInfo.reset (new juce::ImageButton ("btnInfo"));
    addAndMakeVisible (btnInfo.get());
    btnInfo->setButtonText (TRANS("new button"));
    btnInfo->addListener (this);

    btnInfo->setImages (false, true, true,
                        juce::ImageCache::getFromMemory (help_png, help_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::ImageCache::getFromMemory (help_png, help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                        juce::ImageCache::getFromMemory (help_png, help_pngSize), 1.000f, juce::Colour (0xc0ee1010));

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sliderInterval->setSkewFactorFromMidPoint(100.0);
    sliderInterval->setValue(pSettings->oscSendExtIntervalMs, dontSendNotification);

    toggleEnableStandardXyz->setToggleState(pSettings->oscSendExtXyzFlag, dontSendNotification);
    textOscSendIpExtXyz->setText(pSettings->oscSendExtXyzHost, false);
    sliderPortExtXyz->setValue(pSettings->oscSendExtXyzPort, dontSendNotification);

    toggleEnableStandardAed->setToggleState(pSettings->oscSendExtAedFlag, dontSendNotification);
    textOscSendIpExtAed->setText(pSettings->oscSendExtAedHost, false);
    sliderPortExtAed->setValue(pSettings->oscSendExtAedPort, dontSendNotification);

    customOscTableModel->initTable(targetList.get());

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
    labelOscSendIpExtXyz = nullptr;
    groupCustom = nullptr;
    targetList = nullptr;
    groupGeneral = nullptr;
    sliderInterval = nullptr;
    labelInterval = nullptr;
    toggleEnableStandardAed = nullptr;
    textOscSendIpExtAed = nullptr;
    labelOscSendIpExtAed = nullptr;
    sliderPortExtXyz = nullptr;
    sliderPortExtAed = nullptr;
    btnAdd = nullptr;
    btnDelete = nullptr;
    btnInfo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    customOscTableModel = nullptr;
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
    textOscSendIpExtXyz->setBounds (0 + (getWidth() - 0) - 124 - 106, 72 + 24, 106, 24);
    labelOscSendIpExtXyz->setBounds (0 + (getWidth() - 0) - 234 - 126, 72 + 24, 126, 24);
    groupCustom->setBounds (0, 176, getWidth() - 0, getHeight() - 176);
    targetList->setBounds (0 + 16, 176 + 24, (getWidth() - 0) - 32, (getHeight() - 176) - 65);
    groupGeneral->setBounds (0, 0, getWidth() - 0, 64);
    sliderInterval->setBounds (0 + (getWidth() - 0) - 16 - 270, 0 + 24, 270, 24);
    labelInterval->setBounds (0 + 16, 0 + 24, 150, 24);
    toggleEnableStandardAed->setBounds (0 + 16, 72 + 55, 150, 24);
    textOscSendIpExtAed->setBounds (0 + (getWidth() - 0) - 124 - 106, 72 + 55, 106, 24);
    labelOscSendIpExtAed->setBounds (0 + (getWidth() - 0) - 234 - 126, 72 + 55, 126, 24);
    sliderPortExtXyz->setBounds (0 + (getWidth() - 0) - 16 - 100, 72 + 24, 100, 24);
    sliderPortExtAed->setBounds (0 + (getWidth() - 0) - 16 - 100, 72 + 56, 100, 24);
    btnAdd->setBounds (0 + (getWidth() - 0) - 16 - 70, 176 + (getHeight() - 176) - 10 - 24, 70, 24);
    btnDelete->setBounds (0 + (getWidth() - 0) - 92 - 70, 176 + (getHeight() - 176) - 10 - 24, 70, 24);
    btnInfo->setBounds (0 + 16, 176 + (getHeight() - 176) - 10 - 24, 23, 24);
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
    else if (buttonThatWasClicked == btnAdd.get())
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
        pSettings->customOscTargets.add(new CustomOscTarget());
        targetList->updateContent();
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
        int row = targetList->getSelectedRow();
        if (row >= 0 && row < pSettings->customOscTargets.size())
            pSettings->customOscTargets.remove(row);
        targetList->updateContent();
        //[/UserButtonCode_btnDelete]
    }
    else if (buttonThatWasClicked == btnInfo.get())
    {
        //[UserButtonCode_btnInfo] -- add your button handler code here..
        auto label = std::make_unique<Label>("", "Allowed objects:\n{x}, {y}, {z} = Real X/Y/Z-Coordinates\n{a}, {e}, {d} = Real A/E/D-Coordinates\n{sx}, {sy}, {sz} = Scaled X/Y/Z-Coordinates (0..1)\n{sa}, {se}, {sd} = Scaled A/E/D-Coordinates (0..1)\n{i} = index\n{n} = name\n{g} = gain\n{c} = color");
        label->setSize(350, 145);
        label->setJustificationType(Justification::left);
        CallOutBox::launchAsynchronously(std::move(label), btnInfo->getBounds(), this);
        //[/UserButtonCode_btnInfo]
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
    labelOscSendIpExtXyz->setEnabled(toggleEnableStandardXyz->getToggleState());
    labelOscSendIpExtAed->setEnabled(toggleEnableStandardAed->getToggleState());

    textOscSendIpExtXyz->setEnabled(toggleEnableStandardXyz->getToggleState());
    textOscSendIpExtAed->setEnabled(toggleEnableStandardAed->getToggleState());

    sliderPortExtXyz->setEnabled(toggleEnableStandardXyz->getToggleState());
    sliderPortExtAed->setEnabled(toggleEnableStandardAed->getToggleState());

    btnDelete->setEnabled(targetList->getSelectedRows().size() > 0);
}

void OSCTargetsComponent::actionListenerCallback(const String& message)
{
    controlDimming();

    if (message == ACTION_MESSAGE_DATA_CHANGED)
        sendChangeMessage();
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
                 parentClasses="public juce::Component, public ChangeBroadcaster, public TextEditor::Listener, public ActionListener"
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
              virtualName="" explicitFocusOrder="0" pos="124Rr 24 106 24" posRelativeX="a452293a001780b9"
              posRelativeY="a452293a001780b9" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExtXyz" id="ee4b90142f7a53f4" memberName="labelOscSendIpExtXyz"
         virtualName="" explicitFocusOrder="0" pos="234Rr 24 126 24" posRelativeX="a452293a001780b9"
         posRelativeY="a452293a001780b9" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <GROUPCOMPONENT name="groupCustom" id="5ccced30e0050e9" memberName="groupCustom"
                  virtualName="" explicitFocusOrder="0" pos="0 176 0M 176M" title="Custom OSC"/>
  <GENERICCOMPONENT name="targetList" id="78a64bf0c7700896" memberName="targetList"
                    virtualName="" explicitFocusOrder="0" pos="16 24 32M 65M" posRelativeX="5ccced30e0050e9"
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
              virtualName="" explicitFocusOrder="0" pos="124Rr 55 106 24" posRelativeX="a452293a001780b9"
              posRelativeY="a452293a001780b9" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscSendIpExtAed" id="22dbcbf07f7968d2" memberName="labelOscSendIpExtAed"
         virtualName="" explicitFocusOrder="0" pos="234Rr 55 126 24" posRelativeX="a452293a001780b9"
         posRelativeY="a452293a001780b9" edTextCol="ff000000" edBkgCol="0"
         labelText="Target Host/Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="sliderPortExtXyz" id="591bcc850e858bff" memberName="sliderPortExtXyz"
          virtualName="" explicitFocusOrder="0" pos="16Rr 24 100 24" posRelativeX="a452293a001780b9"
          posRelativeY="a452293a001780b9" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="sliderPortExtAed" id="218b7cbec68ee7e8" memberName="sliderPortExtAed"
          virtualName="" explicitFocusOrder="0" pos="16Rr 56 100 24" posRelativeX="a452293a001780b9"
          posRelativeY="a452293a001780b9" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="btnAdd" id="239ce64ab2ee06e6" memberName="btnAdd" virtualName=""
              explicitFocusOrder="0" pos="16Rr 10Rr 70 24" posRelativeX="5ccced30e0050e9"
              posRelativeY="5ccced30e0050e9" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnDelete" id="4ddb1b3ba4cb8ca0" memberName="btnDelete"
              virtualName="" explicitFocusOrder="0" pos="92Rr 10Rr 70 24" posRelativeX="5ccced30e0050e9"
              posRelativeY="5ccced30e0050e9" buttonText="delete" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="btnInfo" id="9b16466da29090a6" memberName="btnInfo" virtualName=""
               explicitFocusOrder="0" pos="16 10Rr 23 24" posRelativeX="5ccced30e0050e9"
               posRelativeY="5ccced30e0050e9" buttonText="new button" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal="help_png"
               opacityNormal="1.0" colourNormal="0" resourceOver="help_png"
               opacityOver="0.4000000059604645" colourOver="6eee1010" resourceDown="help_png"
               opacityDown="1.0" colourDown="c0ee1010"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: help_png, 1009, "../../Resources/help.png"
static const unsigned char resource_OSCTargetsComponent_help_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,64,0,0,0,64,8,6,0,0,0,170,105,113,222,0,0,3,184,73,68,65,84,120,94,229,155,193,
117,163,64,12,134,201,190,220,73,7,166,131,56,21,12,91,129,237,10,22,42,88,119,96,114,200,221,91,129,73,5,11,215,92,118,232,128,237,0,119,224,169,192,59,135,255,192,35,75,132,61,154,17,137,245,158,30,
151,60,143,244,161,145,52,163,112,119,62,159,163,91,150,123,171,209,219,219,155,47,0,11,171,169,213,37,52,129,142,73,7,109,161,218,234,145,27,64,211,52,209,203,203,11,0,248,113,122,109,53,131,211,151,
72,2,77,35,8,64,148,86,43,102,24,236,0,148,213,2,198,179,9,32,238,161,26,107,52,92,0,190,49,57,254,7,198,165,145,71,1,92,141,245,148,52,128,216,234,111,194,113,175,32,176,126,44,1,96,133,100,181,142,100,
101,13,59,86,33,1,28,144,144,30,162,121,200,3,236,57,248,6,16,99,239,101,209,60,37,131,125,177,15,0,177,195,94,151,200,13,49,39,0,56,143,154,62,127,89,246,32,176,0,168,28,156,151,132,80,113,52,66,7,198,
176,215,208,214,234,201,106,55,232,236,20,158,9,128,167,142,224,83,216,159,95,11,96,229,154,240,224,232,30,106,168,22,189,247,124,237,181,213,133,131,29,25,34,161,238,3,80,74,145,91,32,70,255,237,34,37,
222,230,179,85,211,131,186,67,182,222,81,221,28,34,36,199,239,180,14,118,196,151,70,64,233,82,231,65,254,149,72,164,41,158,26,13,141,33,64,60,33,164,47,141,134,7,64,216,76,77,130,202,177,195,43,224,252,
112,63,46,25,74,87,14,96,215,116,140,106,42,128,194,193,249,214,234,243,72,2,236,136,172,189,189,32,186,78,215,189,24,26,128,114,204,250,251,17,48,134,114,112,106,104,99,59,84,87,86,5,133,11,145,81,0,
69,228,32,132,97,53,145,200,18,100,125,82,28,18,116,241,81,4,44,24,106,190,113,0,4,8,147,164,117,232,13,22,35,0,130,28,109,53,211,2,198,229,8,61,2,128,229,148,167,200,22,149,71,22,110,37,250,61,128,5,
147,113,251,15,202,89,140,68,232,32,44,32,151,74,169,197,16,64,202,124,18,91,253,39,50,244,132,61,222,77,14,99,55,73,135,157,224,50,200,73,140,118,254,56,49,252,51,6,27,95,17,1,0,32,47,165,123,169,157,
14,96,184,5,18,89,0,56,53,210,114,96,138,214,100,86,0,224,188,33,175,224,17,250,188,0,228,165,35,222,190,178,218,250,234,83,238,103,114,155,107,70,222,122,129,210,233,73,228,35,160,26,153,243,253,176,
218,250,118,94,62,2,222,59,248,136,237,144,70,144,175,12,160,28,212,253,29,113,26,253,114,91,160,26,148,55,56,47,3,160,19,0,80,227,249,83,96,220,214,17,0,4,242,128,48,128,54,48,128,22,207,71,161,38,172,
149,6,112,26,244,228,226,0,180,192,76,95,9,78,155,245,16,192,17,84,66,79,112,51,137,240,111,154,230,56,0,128,186,124,27,82,142,245,1,213,141,0,168,198,0,28,3,229,130,10,153,255,14,154,4,132,175,225,39,
49,24,241,107,192,6,176,251,224,55,129,32,20,84,43,220,128,146,68,211,179,13,0,191,241,59,28,165,229,47,49,243,11,240,246,105,0,77,136,112,148,191,119,0,0,98,4,205,45,43,98,218,227,171,235,204,46,61,14,
27,64,240,17,134,49,81,159,3,93,185,209,247,1,53,97,20,215,228,232,17,255,51,148,122,106,122,106,151,27,161,28,117,58,21,152,28,113,100,253,156,227,70,104,141,115,194,167,145,254,53,58,7,0,99,53,253,68,
16,90,216,107,56,0,12,33,232,153,3,208,132,243,4,0,26,194,247,25,159,26,75,216,103,124,15,70,114,236,175,211,76,0,156,96,79,30,114,50,84,7,61,197,209,39,203,90,98,46,96,172,110,68,114,3,246,58,214,55,
210,131,145,6,123,15,32,252,59,142,245,154,43,191,26,37,0,56,130,64,72,110,25,203,102,139,223,75,8,199,195,70,0,113,172,253,101,245,9,48,178,222,87,159,221,132,129,133,198,223,103,128,249,132,223,99,57,
46,43,165,162,190,220,252,215,227,223,162,27,151,127,227,222,244,125,176,15,252,137,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* OSCTargetsComponent::help_png = (const char*) resource_OSCTargetsComponent_help_png;
const int OSCTargetsComponent::help_pngSize = 1009;


//[EndFile] You can add extra defines here...
//[/EndFile]
