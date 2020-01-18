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

#include "RadarSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
RadarSettingsComponent::RadarSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings)
    : pSettings(pSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    //[/Constructor_pre]

    sliderPointScaler.reset (new Slider ("sliderPointScaler"));
    addAndMakeVisible (sliderPointScaler.get());
    sliderPointScaler->setRange (0.2, 2, 0.01);
    sliderPointScaler->setSliderStyle (Slider::LinearHorizontal);
    sliderPointScaler->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    sliderPointScaler->addListener (this);

    labelPointScaler.reset (new Label ("labelPointScaler",
                                       TRANS("Point Scaler:")));
    addAndMakeVisible (labelPointScaler.get());
    labelPointScaler->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPointScaler->setJustificationType (Justification::centredLeft);
    labelPointScaler->setEditable (false, false, false);
    labelPointScaler->setColour (TextEditor::textColourId, Colours::black);
    labelPointScaler->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelPointScaler->setBounds (8, 8, 109, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sliderPointScaler->setValue(pSettings->pointScaler);
    sliderPointScaler->setSkewFactorFromMidPoint(1.0);
    //[/Constructor]
}

RadarSettingsComponent::~RadarSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    sliderPointScaler = nullptr;
    labelPointScaler = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RadarSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RadarSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    sliderPointScaler->setBounds (getWidth() - 14 - (getWidth() - 150), 8, getWidth() - 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RadarSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderPointScaler.get())
    {
        //[UserSliderCode_sliderPointScaler] -- add your slider handling code here..
        pSettings->pointScaler = sliderPointScaler->getValue();
        sendChangeMessage();
        //[/UserSliderCode_sliderPointScaler]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RadarSettingsComponent" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings"
                 variableInitialisers="pSettings(pSettings)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <SLIDER name="sliderPointScaler" id="e67c3f12c44da8fb" memberName="sliderPointScaler"
          virtualName="" explicitFocusOrder="0" pos="14Rr 8 150M 24" posRelativeX="6c4c7585300838d8"
          posRelativeY="6c4c7585300838d8" posRelativeW="6c4c7585300838d8"
          min="0.2" max="2.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelPointScaler" id="36bfe3e1aa4822af" memberName="labelPointScaler"
         virtualName="" explicitFocusOrder="0" pos="8 8 109 24" posRelativeX="6c4c7585300838d8"
         posRelativeY="6c4c7585300838d8" edTextCol="ff000000" edBkgCol="0"
         labelText="Point Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

