/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.2

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

    labelRoomSize.reset (new juce::Label ("new label",
                                          TRANS("Room size")));
    addAndMakeVisible (labelRoomSize.get());
    labelRoomSize->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelRoomSize->setJustificationType (juce::Justification::centredLeft);
    labelRoomSize->setEditable (false, false, false);
    labelRoomSize->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRoomSize->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelRoomSize->setBounds (0, 0, 80, 24);

    sliderRoomSizeX.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeX.get());
    sliderRoomSizeX->setRange (0.1, 1000, 0.1);
    sliderRoomSizeX->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeX->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeX->addListener (this);

    sliderRoomSizeX->setBounds (24, 24, 120, 24);

    sliderRoomSizeY.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeY.get());
    sliderRoomSizeY->setRange (0.1, 1000, 0.1);
    sliderRoomSizeY->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeY->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeY->addListener (this);

    sliderRoomSizeY->setBounds (24, 48, 120, 24);

    labelX.reset (new juce::Label ("new label",
                                   TRANS("X:")));
    addAndMakeVisible (labelX.get());
    labelX->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelX->setJustificationType (juce::Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelX->setBounds (0, 24, 24, 24);

    labelY.reset (new juce::Label ("new label",
                                   TRANS("Y:")));
    addAndMakeVisible (labelY.get());
    labelY->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelY->setJustificationType (juce::Justification::centredLeft);
    labelY->setEditable (false, false, false);
    labelY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelY->setBounds (0, 48, 24, 24);

    sliderRoomSizeZ.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeZ.get());
    sliderRoomSizeZ->setRange (0.1, 1000, 0.1);
    sliderRoomSizeZ->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeZ->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeZ->addListener (this);

    sliderRoomSizeZ->setBounds (24, 72, 120, 24);

    labelZ.reset (new juce::Label ("new label",
                                   TRANS("Z:")));
    addAndMakeVisible (labelZ.get());
    labelZ->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelZ->setJustificationType (juce::Justification::centredLeft);
    labelZ->setEditable (false, false, false);
    labelZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelZ->setBounds (0, 72, 24, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (150, 100);


    //[Constructor] You can add your own custom stuff here..
    calculateAndDisplayRoomSize();
    sliderRoomSizeX->setNumDecimalPlacesToDisplay(2);
    sliderRoomSizeY->setNumDecimalPlacesToDisplay(2);
    sliderRoomSizeZ->setNumDecimalPlacesToDisplay(2);
    //[/Constructor]
}

ScalingComponent::~ScalingComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelRoomSize = nullptr;
    sliderRoomSizeX = nullptr;
    sliderRoomSizeY = nullptr;
    labelX = nullptr;
    labelY = nullptr;
    sliderRoomSizeZ = nullptr;
    labelZ = nullptr;


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
        if(currentRoomSizeX > 0.0)
        {
            for(int i = 0; i < pSpeakerSet->size(); i++)
                pSpeakerSet->get(i)->getPoint()->setX(pSpeakerSet->get(i)->getPoint()->getX() / currentRoomSizeX * sliderRoomSizeX->getValue());

            currentRoomSizeX = sliderRoomSizeX->getValue();
        }
        //[/UserSliderCode_sliderRoomSizeX]
    }
    else if (sliderThatWasMoved == sliderRoomSizeY.get())
    {
        //[UserSliderCode_sliderRoomSizeY] -- add your slider handling code here..
        if(currentRoomSizeY > 0.0)
        {
            for(int i = 0; i < pSpeakerSet->size(); i++)
                pSpeakerSet->get(i)->getPoint()->setY(pSpeakerSet->get(i)->getPoint()->getY() / currentRoomSizeY * sliderRoomSizeY->getValue());

            currentRoomSizeY = sliderRoomSizeY->getValue();
        }
        //[/UserSliderCode_sliderRoomSizeY]
    }
    else if (sliderThatWasMoved == sliderRoomSizeZ.get())
    {
        //[UserSliderCode_sliderRoomSizeZ] -- add your slider handling code here..
        if(currentRoomSizeZ > 0.0)
        {
            for(int i = 0; i < pSpeakerSet->size(); i++)
                pSpeakerSet->get(i)->getPoint()->setZ(pSpeakerSet->get(i)->getPoint()->getZ() / currentRoomSizeZ * sliderRoomSizeZ->getValue());

            currentRoomSizeZ = sliderRoomSizeZ->getValue();
        }
        //[/UserSliderCode_sliderRoomSizeZ]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
    pZoomSettings->Reset(pSpeakerSet);
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ScalingComponent::calculateAndDisplayRoomSize()
{
    double maxX = 0.0, maxY = 0.0, maxZ = 0.0;
    for(int i = 0; i < pSpeakerSet->size(); i++)
    {
        maxX = jmax(maxX, abs(pSpeakerSet->get(i)->getPoint()->getX()));
        maxY = jmax(maxY, abs(pSpeakerSet->get(i)->getPoint()->getY()));
        maxZ = jmax(maxZ, abs(pSpeakerSet->get(i)->getPoint()->getZ()));
    }

    currentRoomSizeX = maxX * 2.0;
    currentRoomSizeY = maxY * 2.0;
    currentRoomSizeZ = maxZ * 2.0;

    sliderRoomSizeX->setValue(currentRoomSizeX, dontSendNotification);
    sliderRoomSizeY->setValue(currentRoomSizeY, dontSendNotification);
    sliderRoomSizeZ->setValue(currentRoomSizeZ, dontSendNotification);

    sliderRoomSizeX->setEnabled(currentRoomSizeX > 0.0);
    sliderRoomSizeY->setEnabled(currentRoomSizeY > 0.0);
    sliderRoomSizeZ->setEnabled(currentRoomSizeZ > 0.0);
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
                 fixedSize="0" initialWidth="150" initialHeight="100">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="4c61197f8352080b" memberName="labelRoomSize"
         virtualName="" explicitFocusOrder="0" pos="0 0 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Room size" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="25d6a4f96c7831e1" memberName="sliderRoomSizeX"
          virtualName="" explicitFocusOrder="0" pos="24 24 120 24" min="0.1"
          max="1000.0" int="0.1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="ffd4bd0a82928115" memberName="sliderRoomSizeY"
          virtualName="" explicitFocusOrder="0" pos="24 48 120 24" min="0.1"
          max="1000.0" int="0.1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="1e24e82f0fe497f9" memberName="labelX" virtualName=""
         explicitFocusOrder="0" pos="0 24 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="X:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="ea9405f7cb5208df" memberName="labelY" virtualName=""
         explicitFocusOrder="0" pos="0 48 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Y:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="b90acff5eb8097b1" memberName="sliderRoomSizeZ"
          virtualName="" explicitFocusOrder="0" pos="24 72 120 24" min="0.1"
          max="1000.0" int="0.1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="1dc54b9ae36a1875" memberName="labelZ" virtualName=""
         explicitFocusOrder="0" pos="0 72 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Z:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

