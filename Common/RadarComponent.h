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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "AmbiDataSet.h"
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
class RadarComponent  : public Component,
                        public Timer
{
public:
    //==============================================================================
    RadarComponent (AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~RadarComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setPointInfoVisible(bool visible);
    void timerCallback() override;
    ZoomSettings* getZoomSettingsPointer() {return &zoomSettings;}
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ZoomSettings zoomSettings;
	bool showPointInfo;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Radar3D> radar;
    std::unique_ptr<PointInfoControl> pointInfo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadarComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

