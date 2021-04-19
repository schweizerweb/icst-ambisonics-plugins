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

#include "ScalingComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ScalingComponent::ScalingComponent (ChangeListener* pListener, AmbiSpeakerSet* pSpeakerSet, ZoomSettings* pZoomSettings)
    : pSpeakerSet(pSpeakerSet), pZoomSettings(pZoomSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pListener);
    //[/Constructor_pre]

    labelProportionalScale.reset (new juce::Label ("new label",
                                                   TRANS("Proportional distance scaling:")));
    addAndMakeVisible (labelProportionalScale.get());
    labelProportionalScale->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelProportionalScale->setJustificationType (juce::Justification::centredLeft);
    labelProportionalScale->setEditable (false, false, false);
    labelProportionalScale->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelProportionalScale->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelProportionalScale->setBounds (0, 0, 200, 24);

    labelRoomSize.reset (new juce::Label ("new label",
                                          TRANS("Room size:")));
    addAndMakeVisible (labelRoomSize.get());
    labelRoomSize->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelRoomSize->setJustificationType (juce::Justification::centredLeft);
    labelRoomSize->setEditable (false, false, false);
    labelRoomSize->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRoomSize->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelRoomSize->setBounds (0, 32, 80, 24);

    sliderRoomSizeX.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeX.get());
    sliderRoomSizeX->setRange (0, 1000, 0.1);
    sliderRoomSizeX->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeX->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeX->addListener (this);

    sliderRoomSizeX->setBounds (80, 32, 120, 24);

    sliderRoomSizeY.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeY.get());
    sliderRoomSizeY->setRange (0, 1000, 0.1);
    sliderRoomSizeY->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeY->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeY->addListener (this);

    sliderRoomSizeY->setBounds (228, 32, 120, 24);

    labelX.reset (new juce::Label ("new label",
                                   TRANS("x")));
    addAndMakeVisible (labelX.get());
    labelX->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelX->setJustificationType (juce::Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelX->setBounds (206, 32, 15, 24);

    buttonDec.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonDec.get());
    buttonDec->setButtonText (TRANS("-"));
    buttonDec->addListener (this);

    buttonDec->setBounds (226, 0, 56, 24);

    buttonInc.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonInc.get());
    buttonInc->setButtonText (TRANS("+"));
    buttonInc->addListener (this);

    buttonInc->setBounds (290, 0, 56, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (350, 60);


    //[Constructor] You can add your own custom stuff here..
    calculateAndDisplayRoomSize();
    sliderRoomSizeX->setNumDecimalPlacesToDisplay(2);
    sliderRoomSizeY->setNumDecimalPlacesToDisplay(2);
    //[/Constructor]
}

ScalingComponent::~ScalingComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelProportionalScale = nullptr;
    labelRoomSize = nullptr;
    sliderRoomSizeX = nullptr;
    sliderRoomSizeY = nullptr;
    labelX = nullptr;
    buttonDec = nullptr;
    buttonInc = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ScalingComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ScalingComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ScalingComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderRoomSizeX.get())
    {
        //[UserSliderCode_sliderRoomSizeX] -- add your slider handling code here..
        for(int i = 0; i < pSpeakerSet->size(); i++)
            pSpeakerSet->get(i)->getPoint()->setX(pSpeakerSet->get(i)->getPoint()->getX() / currentRoomSizeX * sliderRoomSizeX->getValue());
        
        currentRoomSizeX = sliderRoomSizeX->getValue();
        //[/UserSliderCode_sliderRoomSizeX]
    }
    else if (sliderThatWasMoved == sliderRoomSizeY.get())
    {
        //[UserSliderCode_sliderRoomSizeY] -- add your slider handling code here..
        for(int i = 0; i < pSpeakerSet->size(); i++)
        pSpeakerSet->get(i)->getPoint()->setY(pSpeakerSet->get(i)->getPoint()->getY() / currentRoomSizeY * sliderRoomSizeY->getValue());
        
        currentRoomSizeY = sliderRoomSizeY->getValue();
        //[/UserSliderCode_sliderRoomSizeY]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
    pZoomSettings->Reset(pSpeakerSet);
    //[/UsersliderValueChanged_Post]
}

void ScalingComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonDec.get())
    {
        //[UserButtonCode_buttonDec] -- add your button handler code here..
        for(int i = 0; i < pSpeakerSet->size(); i++)
            pSpeakerSet->get(i)->getPoint()->setDistance(jmax(DISTANCE_MIN_VALUE, pSpeakerSet->get(i)->getPoint()->getDistance() - 1));
        //[/UserButtonCode_buttonDec]
    }
    else if (buttonThatWasClicked == buttonInc.get())
    {
        //[UserButtonCode_buttonInc] -- add your button handler code here..
        for(int i = 0; i < pSpeakerSet->size(); i++)
            pSpeakerSet->get(i)->getPoint()->setDistance(pSpeakerSet->get(i)->getPoint()->getDistance() + 1);
        //[/UserButtonCode_buttonInc]
    }

    //[UserbuttonClicked_Post]
    sendChangeMessage();
    pZoomSettings->Reset(pSpeakerSet);
    calculateAndDisplayRoomSize();
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ScalingComponent::calculateAndDisplayRoomSize()
{
    double maxX = 0.0, maxY = 0.0;
    for(int i = 0; i < pSpeakerSet->size(); i++)
    {
        maxX = jmax(maxX, abs(pSpeakerSet->get(i)->getPoint()->getX()));
        maxY = jmax(maxY, abs(pSpeakerSet->get(i)->getPoint()->getY()));
    }
    
    sliderRoomSizeX->setValue(maxX, dontSendNotification);
    sliderRoomSizeY->setValue(maxY, dontSendNotification);
    currentRoomSizeX = maxX;
    currentRoomSizeY = maxY;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ScalingComponent" componentName=""
                 parentClasses="public juce::Component, public ChangeBroadcaster"
                 constructorParams="ChangeListener* pListener, AmbiSpeakerSet* pSpeakerSet, ZoomSettings* pZoomSettings"
                 variableInitialisers="pSpeakerSet(pSpeakerSet), pZoomSettings(pZoomSettings)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="350" initialHeight="60">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="8d4ea15fd03ab89d" memberName="labelProportionalScale"
         virtualName="" explicitFocusOrder="0" pos="0 0 200 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Proportional distance scaling:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="4c61197f8352080b" memberName="labelRoomSize"
         virtualName="" explicitFocusOrder="0" pos="0 32 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Room size:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="25d6a4f96c7831e1" memberName="sliderRoomSizeX"
          virtualName="" explicitFocusOrder="0" pos="80 32 120 24" min="0.0"
          max="1000.0" int="0.1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="ffd4bd0a82928115" memberName="sliderRoomSizeY"
          virtualName="" explicitFocusOrder="0" pos="228 32 120 24" min="0.0"
          max="1000.0" int="0.1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="1e24e82f0fe497f9" memberName="labelX" virtualName=""
         explicitFocusOrder="0" pos="206 32 15 24" edTextCol="ff000000"
         edBkgCol="0" labelText="x" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="370794d369b7d65e" memberName="buttonDec"
              virtualName="" explicitFocusOrder="0" pos="226 0 56 24" buttonText="-"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="ac7b38c1070661ed" memberName="buttonInc"
              virtualName="" explicitFocusOrder="0" pos="290 0 56 24" buttonText="+"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

