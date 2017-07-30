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

#include "Radar3D.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Radar3D::Radar3D (Array<AmbiPoint>* pSpeakerArray, ZoomSettings* pZoomSettings, PointSelection* pPointSelection)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (xyRadar = new Radar2D (Radar2D::XY, pSpeakerArray, pZoomSettings, pPointSelection));
    xyRadar->setName ("xyRadar");

    addAndMakeVisible (zyRadar = new Radar2D (Radar2D::ZY, pSpeakerArray, pZoomSettings, pPointSelection));
    zyRadar->setName ("zyRadar");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Radar3D::~Radar3D()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    xyRadar = nullptr;
    zyRadar = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Radar3D::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Radar3D::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    xyRadar->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (0.6661f));
    zyRadar->setBounds (0, 0 + proportionOfHeight (0.6661f), proportionOfWidth (1.0000f), roundFloatToInt (proportionOfHeight (0.6661f) * 0.4989f));
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

<JUCER_COMPONENT documentType="Component" className="Radar3D" componentName=""
                 parentClasses="public Component" constructorParams="Array&lt;AmbiPoint&gt;* pSpeakerArray, ZoomSettings* pZoomSettings, PointSelection* pPointSelection"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="xyRadar" id="952154a5b6ffaa65" memberName="xyRadar" virtualName=""
                    explicitFocusOrder="0" pos="0 0 100% 66.615%" class="Radar2D"
                    params="Radar2D::XY, pSpeakerArray, pZoomSettings, pPointSelection"/>
  <GENERICCOMPONENT name="zyRadar" id="9b35aa2c2da622df" memberName="zyRadar" virtualName=""
                    explicitFocusOrder="0" pos="0 0R 100% 49.885%" posRelativeY="952154a5b6ffaa65"
                    posRelativeH="952154a5b6ffaa65" class="Radar2D" params="Radar2D::ZY, pSpeakerArray, pZoomSettings, pPointSelection"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
