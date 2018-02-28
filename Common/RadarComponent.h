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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "AmbiPoint.h"
#include "Radar3D.h"
#include "ZoomSettings.h"
#include "PointInfoControl.h"
#include "PointSelection.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class RadarComponent  : public Component
{
public:
    //==============================================================================
    RadarComponent (Array<AmbiPoint>* pSpeakerArray, Array<AmbiPoint>* pMovingPointsArray, PointSelection* pPointSelection);
    ~RadarComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ZoomSettings zoomSettings;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Radar3D> radar;
    ScopedPointer<PointInfoControl> pointInfo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadarComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
