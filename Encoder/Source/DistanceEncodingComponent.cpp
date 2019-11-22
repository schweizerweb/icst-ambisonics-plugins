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

#include "DistanceEncodingComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DistanceEncodingComponent::DistanceEncodingComponent (DistanceEncodingParams* pParams)
    : pParams(pParams)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    distanceEncodingGraph.reset (new DistanceEncodingGraph (pParams));
    addAndMakeVisible (distanceEncodingGraph.get());
    distanceEncodingGraph->setName ("distanceEncodingGraph");

    sliderUnitCircleRadius.reset (new Slider ("sliderUnitCircleRadius"));
    addAndMakeVisible (sliderUnitCircleRadius.get());
    sliderUnitCircleRadius->setRange (0.01, 1, 0.01);
    sliderUnitCircleRadius->setSliderStyle (Slider::LinearHorizontal);
    sliderUnitCircleRadius->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderUnitCircleRadius->addListener (this);

    labelUnitCircleRadius.reset (new Label ("labelUnitCircleRadius",
                                            TRANS("Unit Circle Radius")));
    addAndMakeVisible (labelUnitCircleRadius.get());
    labelUnitCircleRadius->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelUnitCircleRadius->setJustificationType (Justification::centredLeft);
    labelUnitCircleRadius->setEditable (false, false, false);
    labelUnitCircleRadius->setColour (TextEditor::textColourId, Colours::black);
    labelUnitCircleRadius->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelUnitCircleRadius->setBounds (8, 8, 150, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	sliderUnitCircleRadius->setValue(pParams->unitCircleRadius, dontSendNotification);
    //[/Constructor]
}

DistanceEncodingComponent::~DistanceEncodingComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    distanceEncodingGraph = nullptr;
    sliderUnitCircleRadius = nullptr;
    labelUnitCircleRadius = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DistanceEncodingComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DistanceEncodingComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    distanceEncodingGraph->setBounds (0, 40, proportionOfWidth (1.0000f), getHeight() - 40);
    sliderUnitCircleRadius->setBounds (160, 8, getWidth() - 165, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DistanceEncodingComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderUnitCircleRadius.get())
    {
        //[UserSliderCode_sliderUnitCircleRadius] -- add your slider handling code here..
		pParams->unitCircleRadius = float(sliderUnitCircleRadius->getValue());
		distanceEncodingGraph->repaint();
        //[/UserSliderCode_sliderUnitCircleRadius]
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

<JUCER_COMPONENT documentType="Component" className="DistanceEncodingComponent"
                 componentName="" parentClasses="public Component" constructorParams="DistanceEncodingParams* pParams"
                 variableInitialisers="pParams(pParams)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="distanceEncodingGraph" id="eaba5f5be7082dad" memberName="distanceEncodingGraph"
                    virtualName="" explicitFocusOrder="0" pos="0 40 100% 40M" class="DistanceEncodingGraph"
                    params="pParams"/>
  <SLIDER name="sliderUnitCircleRadius" id="33a23e1d161c87b2" memberName="sliderUnitCircleRadius"
          virtualName="" explicitFocusOrder="0" pos="160 8 165M 24" min="0.01"
          max="1.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelUnitCircleRadius" id="1c135dae5e4bb342" memberName="labelUnitCircleRadius"
         virtualName="" explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Unit Circle Radius" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

