/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "RadarSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#if JUCE_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif
//[/MiscUserDefs]

//==============================================================================
RadarSettingsComponent::RadarSettingsComponent (ChangeListener* pChangeListener, ZoomSettings* _pZoomSettings)
    : pZoomSettings(_pZoomSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    pZoomSettings->addChangeListener(this);
    //[/Constructor_pre]

    groupZoom.reset (new juce::GroupComponent ("groupZoom",
                                               TRANS("Zoom")));
    addAndMakeVisible (groupZoom.get());

    sliderCenterPointX.reset (new juce::Slider ("sliderCenterPointX"));
    addAndMakeVisible (sliderCenterPointX.get());
    sliderCenterPointX->setRange (-1, 1, 0.01);
    sliderCenterPointX->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterPointX->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderCenterPointX->addListener (this);

    labelCenterPointX.reset (new juce::Label ("labelCenterPointX",
                                              TRANS("Center X")));
    addAndMakeVisible (labelCenterPointX.get());
    labelCenterPointX->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCenterPointX->setJustificationType (juce::Justification::centredLeft);
    labelCenterPointX->setEditable (false, false, false);
    labelCenterPointX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterPointX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderCenterPointY.reset (new juce::Slider ("sliderCenterPointY"));
    addAndMakeVisible (sliderCenterPointY.get());
    sliderCenterPointY->setRange (-1, 1, 0.01);
    sliderCenterPointY->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterPointY->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderCenterPointY->addListener (this);

    labelCenterPointY.reset (new juce::Label ("labelCenterPointY",
                                              TRANS("Center Y")));
    addAndMakeVisible (labelCenterPointY.get());
    labelCenterPointY->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCenterPointY->setJustificationType (juce::Justification::centredLeft);
    labelCenterPointY->setEditable (false, false, false);
    labelCenterPointY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterPointY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderCenterPointZ.reset (new juce::Slider ("sliderCenterPointZ"));
    addAndMakeVisible (sliderCenterPointZ.get());
    sliderCenterPointZ->setRange (-1, 1, 0.01);
    sliderCenterPointZ->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterPointZ->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderCenterPointZ->addListener (this);

    labelCenterPointZ.reset (new juce::Label ("labelCenterPointZ",
                                              TRANS("Center Z")));
    addAndMakeVisible (labelCenterPointZ.get());
    labelCenterPointZ->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCenterPointZ->setJustificationType (juce::Justification::centredLeft);
    labelCenterPointZ->setEditable (false, false, false);
    labelCenterPointZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterPointZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    groupDisplay.reset (new juce::GroupComponent ("groupDisplay",
                                                  TRANS("Display")));
    addAndMakeVisible (groupDisplay.get());

    sliderPointScaler.reset (new juce::Slider ("sliderPointScaler"));
    addAndMakeVisible (sliderPointScaler.get());
    sliderPointScaler->setRange (0.2, 2, 0.01);
    sliderPointScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderPointScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderPointScaler->addListener (this);

    labelPointScaler.reset (new juce::Label ("labelPointScaler",
                                             TRANS("Point Scaler:")));
    addAndMakeVisible (labelPointScaler.get());
    labelPointScaler->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPointScaler->setJustificationType (juce::Justification::centredLeft);
    labelPointScaler->setEditable (false, false, false);
    labelPointScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPointScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRadius.reset (new juce::Slider ("sliderRadius"));
    addAndMakeVisible (sliderRadius.get());
    sliderRadius->setRange (0.01, 100000, 0.01);
    sliderRadius->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderRadius->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderRadius->addListener (this);

    labelRadius.reset (new juce::Label ("labelRadius",
                                        TRANS("Radius")));
    addAndMakeVisible (labelRadius.get());
    labelRadius->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelRadius->setJustificationType (juce::Justification::centredLeft);
    labelRadius->setEditable (false, false, false);
    labelRadius->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRadius->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnReset.reset (new juce::TextButton ("btnReset"));
    addAndMakeVisible (btnReset.get());
    btnReset->setButtonText (TRANS("Reset"));
    btnReset->addListener (this);

    sliderGroupPointScaler.reset (new juce::Slider ("sliderGroupPointScaler"));
    addAndMakeVisible (sliderGroupPointScaler.get());
    sliderGroupPointScaler->setRange (0.2, 2, 0.01);
    sliderGroupPointScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderGroupPointScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderGroupPointScaler->addListener (this);

    labelGroupPointScaler.reset (new juce::Label ("labelGroupPointScaler",
                                                  TRANS("Group Point Scaler:")));
    addAndMakeVisible (labelGroupPointScaler.get());
    labelGroupPointScaler->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelGroupPointScaler->setJustificationType (juce::Justification::centredLeft);
    labelGroupPointScaler->setEditable (false, false, false);
    labelGroupPointScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGroupPointScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sliderPointScaler->setSkewFactorFromMidPoint(1.0);
    sliderGroupPointScaler->setSkewFactorFromMidPoint(1.0);
    sliderRadius->setSkewFactorFromMidPoint(100.0);
    displaySettings();
    //[/Constructor]
}

RadarSettingsComponent::~RadarSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pZoomSettings->removeChangeListener(this);
    //[/Destructor_pre]

    groupZoom = nullptr;
    sliderCenterPointX = nullptr;
    labelCenterPointX = nullptr;
    sliderCenterPointY = nullptr;
    labelCenterPointY = nullptr;
    sliderCenterPointZ = nullptr;
    labelCenterPointZ = nullptr;
    groupDisplay = nullptr;
    sliderPointScaler = nullptr;
    labelPointScaler = nullptr;
    sliderRadius = nullptr;
    labelRadius = nullptr;
    btnReset = nullptr;
    sliderGroupPointScaler = nullptr;
    labelGroupPointScaler = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RadarSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RadarSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupZoom->setBounds (8, 105, getWidth() - 16, 192);
    sliderCenterPointX->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 105 + 24, (getWidth() - 16) - 152, 24);
    labelCenterPointX->setBounds (8 + 16, 105 + 24, 128, 24);
    sliderCenterPointY->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 105 + 56, (getWidth() - 16) - 152, 24);
    labelCenterPointY->setBounds (8 + 16, 105 + 56, 128, 24);
    sliderCenterPointZ->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 105 + 88, (getWidth() - 16) - 152, 24);
    labelCenterPointZ->setBounds (8 + 16, 105 + 88, 128, 24);
    groupDisplay->setBounds (8, 8, getWidth() - 16, 91);
    sliderPointScaler->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 8 + 24, (getWidth() - 16) - 152, 24);
    labelPointScaler->setBounds (8 + 8, 8 + 24, 128, 24);
    sliderRadius->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 105 + 120, (getWidth() - 16) - 152, 24);
    labelRadius->setBounds (8 + 16, 105 + 120, 128, 24);
    btnReset->setBounds (8 + juce::roundToInt ((getWidth() - 16) * 0.5000f) - (150 / 2), 105 + 152, 150, 24);
    sliderGroupPointScaler->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 8 + 56, (getWidth() - 16) - 152, 24);
    labelGroupPointScaler->setBounds (8 + 8, 8 + 56, 128, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RadarSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderCenterPointX.get())
    {
        //[UserSliderCode_sliderCenterPointX] -- add your slider handling code here..
        pZoomSettings->setCurrentCenterPointXY((float)sliderCenterPointX->getValue(), (float)pZoomSettings->getCurrentCenterPoint().getY());
        //[/UserSliderCode_sliderCenterPointX]
    }
    else if (sliderThatWasMoved == sliderCenterPointY.get())
    {
        //[UserSliderCode_sliderCenterPointY] -- add your slider handling code here..
        pZoomSettings->setCurrentCenterPointXY((float)pZoomSettings->getCurrentCenterPoint().getX(), (float)sliderCenterPointY->getValue());
        //[/UserSliderCode_sliderCenterPointY]
    }
    else if (sliderThatWasMoved == sliderCenterPointZ.get())
    {
        //[UserSliderCode_sliderCenterPointZ] -- add your slider handling code here..
        pZoomSettings->setCurrentCenterPointXZ(pZoomSettings->getCurrentCenterPoint().getX(), (float)sliderCenterPointZ->getValue());
        //[/UserSliderCode_sliderCenterPointZ]
    }
    else if (sliderThatWasMoved == sliderPointScaler.get())
    {
        //[UserSliderCode_sliderPointScaler] -- add your slider handling code here..
        pZoomSettings->setPointScaler(sliderPointScaler->getValue());
        //[/UserSliderCode_sliderPointScaler]
    }
    else if (sliderThatWasMoved == sliderRadius.get())
    {
        //[UserSliderCode_sliderRadius] -- add your slider handling code here..
        pZoomSettings->setCurrentRadius((float)sliderRadius->getValue());
        //[/UserSliderCode_sliderRadius]
    }
    else if (sliderThatWasMoved == sliderGroupPointScaler.get())
    {
        //[UserSliderCode_sliderGroupPointScaler] -- add your slider handling code here..
        pZoomSettings->setGroupPointScaler(sliderGroupPointScaler->getValue());
        //[/UserSliderCode_sliderGroupPointScaler]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void RadarSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnReset.get())
    {
        //[UserButtonCode_btnReset] -- add your button handler code here..
        pZoomSettings->Reset();
        //[/UserButtonCode_btnReset]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void RadarSettingsComponent::displaySettings()
{
    sliderPointScaler->setValue(pZoomSettings->getPointScaler(), dontSendNotification);
    sliderGroupPointScaler->setValue(pZoomSettings->getGroupPointScaler(), dontSendNotification);
    sliderCenterPointX->setValue(pZoomSettings->getCurrentCenterPoint().getX(), dontSendNotification);
    sliderCenterPointY->setValue(pZoomSettings->getCurrentCenterPoint().getY(), dontSendNotification);
    sliderCenterPointZ->setValue(pZoomSettings->getCurrentCenterPoint().getZ(), dontSendNotification);
    sliderRadius->setValue(pZoomSettings->getCurrentRadius(), dontSendNotification);
}

void RadarSettingsComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if(source != this)
    {
        displaySettings();
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RadarSettingsComponent" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster, public ChangeListener"
                 constructorParams="ChangeListener* pChangeListener, ZoomSettings* _pZoomSettings"
                 variableInitialisers="pZoomSettings(_pZoomSettings)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupZoom" id="b29e8cd8e89263e2" memberName="groupZoom"
                  virtualName="" explicitFocusOrder="0" pos="8 105 16M 192" title="Zoom"/>
  <SLIDER name="sliderCenterPointX" id="14065109d43ec7e4" memberName="sliderCenterPointX"
          virtualName="" explicitFocusOrder="0" pos="8Rr 24 152M 24" posRelativeX="b29e8cd8e89263e2"
          posRelativeY="b29e8cd8e89263e2" posRelativeW="b29e8cd8e89263e2"
          min="-1.0" max="1.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelCenterPointX" id="b22334fa11147017" memberName="labelCenterPointX"
         virtualName="" explicitFocusOrder="0" pos="16 24 128 24" posRelativeX="b29e8cd8e89263e2"
         posRelativeY="b29e8cd8e89263e2" edTextCol="ff000000" edBkgCol="0"
         labelText="Center X" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderCenterPointY" id="ae7e9f9c205b3edd" memberName="sliderCenterPointY"
          virtualName="" explicitFocusOrder="0" pos="8Rr 56 152M 24" posRelativeX="b29e8cd8e89263e2"
          posRelativeY="b29e8cd8e89263e2" posRelativeW="b29e8cd8e89263e2"
          min="-1.0" max="1.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelCenterPointY" id="ae60b62e333b1dac" memberName="labelCenterPointY"
         virtualName="" explicitFocusOrder="0" pos="16 56 128 24" posRelativeX="b29e8cd8e89263e2"
         posRelativeY="b29e8cd8e89263e2" edTextCol="ff000000" edBkgCol="0"
         labelText="Center Y" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderCenterPointZ" id="1f5f6600c961f524" memberName="sliderCenterPointZ"
          virtualName="" explicitFocusOrder="0" pos="8Rr 88 152M 24" posRelativeX="b29e8cd8e89263e2"
          posRelativeY="b29e8cd8e89263e2" posRelativeW="b29e8cd8e89263e2"
          min="-1.0" max="1.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelCenterPointZ" id="789a034c8595f9eb" memberName="labelCenterPointZ"
         virtualName="" explicitFocusOrder="0" pos="16 88 128 24" posRelativeX="b29e8cd8e89263e2"
         posRelativeY="b29e8cd8e89263e2" edTextCol="ff000000" edBkgCol="0"
         labelText="Center Z" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="groupDisplay" id="fcc56a017f053e10" memberName="groupDisplay"
                  virtualName="" explicitFocusOrder="0" pos="8 8 16M 91" title="Display"/>
  <SLIDER name="sliderPointScaler" id="e67c3f12c44da8fb" memberName="sliderPointScaler"
          virtualName="" explicitFocusOrder="0" pos="8Rr 24 152M 24" posRelativeX="fcc56a017f053e10"
          posRelativeY="fcc56a017f053e10" posRelativeW="fcc56a017f053e10"
          min="0.2" max="2.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelPointScaler" id="36bfe3e1aa4822af" memberName="labelPointScaler"
         virtualName="" explicitFocusOrder="0" pos="8 24 128 24" posRelativeX="fcc56a017f053e10"
         posRelativeY="fcc56a017f053e10" edTextCol="ff000000" edBkgCol="0"
         labelText="Point Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderRadius" id="607378851a94aeae" memberName="sliderRadius"
          virtualName="" explicitFocusOrder="0" pos="8Rr 120 152M 24" posRelativeX="b29e8cd8e89263e2"
          posRelativeY="b29e8cd8e89263e2" posRelativeW="b29e8cd8e89263e2"
          min="0.01" max="100000.0" int="0.01" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelRadius" id="2c8c3f9ef419042b" memberName="labelRadius"
         virtualName="" explicitFocusOrder="0" pos="16 120 128 24" posRelativeX="b29e8cd8e89263e2"
         posRelativeY="b29e8cd8e89263e2" edTextCol="ff000000" edBkgCol="0"
         labelText="Radius" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnReset" id="55739ef20107b96d" memberName="btnReset" virtualName=""
              explicitFocusOrder="0" pos="50%c 152 150 24" posRelativeX="b29e8cd8e89263e2"
              posRelativeY="b29e8cd8e89263e2" buttonText="Reset" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="sliderGroupPointScaler" id="a187fb7f5856b4e3" memberName="sliderGroupPointScaler"
          virtualName="" explicitFocusOrder="0" pos="8Rr 56 152M 24" posRelativeX="fcc56a017f053e10"
          posRelativeY="fcc56a017f053e10" posRelativeW="fcc56a017f053e10"
          min="0.2" max="2.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelGroupPointScaler" id="dd3efe0142b2c446" memberName="labelGroupPointScaler"
         virtualName="" explicitFocusOrder="0" pos="8 56 128 24" posRelativeX="fcc56a017f053e10"
         posRelativeY="fcc56a017f053e10" edTextCol="ff000000" edBkgCol="0"
         labelText="Group Point Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

