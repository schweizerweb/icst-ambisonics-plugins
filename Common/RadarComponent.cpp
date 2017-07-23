/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "RadarComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
RadarComponent::RadarComponent (Array<AmbiPoint>* pAmbiPointArray)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (labelCoordinatesXYZ = new Label ("coordinatesXYZ",
                                                        TRANS("X: - ; Y: - ; Z: -")));
    labelCoordinatesXYZ->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCoordinatesXYZ->setJustificationType (Justification::centredLeft);
    labelCoordinatesXYZ->setEditable (false, false, false);
    labelCoordinatesXYZ->setColour (TextEditor::textColourId, Colours::black);
    labelCoordinatesXYZ->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (labelCoordinatesAED = new Label ("coordinatesAED",
                                                        TRANS("A: - ; E: - ; D: -")));
    labelCoordinatesAED->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCoordinatesAED->setJustificationType (Justification::centredLeft);
    labelCoordinatesAED->setEditable (false, false, false);
    labelCoordinatesAED->setColour (TextEditor::textColourId, Colours::black);
    labelCoordinatesAED->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (radar = new Radar (pAmbiPointArray));
    radar->setName ("radar");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

RadarComponent::~RadarComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelCoordinatesXYZ = nullptr;
    labelCoordinatesAED = nullptr;
    radar = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RadarComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RadarComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    labelCoordinatesXYZ->setBounds (0, -8, 150, 24);
    labelCoordinatesAED->setBounds (0, 8, 150, 24);
    radar->setBounds (0, 8 + 24, proportionOfWidth (1.0000f), getHeight() - 32);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RadarComponent::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...
	labelCoordinatesXYZ->setText(e.getPosition().toString(), sendNotificationAsync);
	if(radar->contains(e.getPosition()))
	{
		labelCoordinatesAED->setText(radar->getMouseXYRelative().toString(), sendNotificationAsync);
	}
    //[/UserCode_mouseMove]
}

void RadarComponent::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    //[/UserCode_mouseExit]
}

void RadarComponent::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    //[/UserCode_mouseDown]
}

void RadarComponent::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    //[/UserCode_mouseDrag]
}

void RadarComponent::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RadarComponent" componentName=""
                 parentClasses="public Component" constructorParams="Array&lt;AmbiPoint&gt;* pAmbiPointArray"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="coordinatesXYZ" id="eb2d4e80735ade2e" memberName="labelCoordinatesXYZ"
         virtualName="" explicitFocusOrder="0" pos="0 -8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="X: - ; Y: - ; Z: -" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="coordinatesAED" id="dba7ee2a2153b8cf" memberName="labelCoordinatesAED"
         virtualName="" explicitFocusOrder="0" pos="0 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="A: - ; E: - ; D: -" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="radar" id="bb1556089d26688f" memberName="radar" virtualName=""
                    explicitFocusOrder="0" pos="0 0R 100% 32M" posRelativeY="dba7ee2a2153b8cf"
                    class="Radar" params="pAmbiPointArray"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
