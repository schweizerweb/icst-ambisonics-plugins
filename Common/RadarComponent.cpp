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

    addAndMakeVisible (radar = new Radar3D (pAmbiPointArray, &zoomSettings, &selectedPointIndex));
    radar->setName ("radar");

    addAndMakeVisible (pointInfo = new PointInfoControl (pAmbiPointArray, &selectedPointIndex));
    pointInfo->setName ("pointInfo");


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

    radar = nullptr;
    pointInfo = nullptr;


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

    radar->setBounds (0, 0 + 140, proportionOfWidth (1.0000f), getHeight() - 140);
    pointInfo->setBounds (0, 0, proportionOfWidth (1.0000f), 140);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
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
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="radar" id="bb1556089d26688f" memberName="radar" virtualName=""
                    explicitFocusOrder="0" pos="0 0R 100% 140M" posRelativeY="328b0557e3704175"
                    class="Radar3D" params="pAmbiPointArray, &amp;zoomSettings, &amp;selectedPointIndex"/>
  <GENERICCOMPONENT name="pointInfo" id="328b0557e3704175" memberName="pointInfo"
                    virtualName="" explicitFocusOrder="0" pos="0 0 100% 140" class="PointInfoControl"
                    params="pAmbiPointArray, &amp;selectedPointIndex"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
