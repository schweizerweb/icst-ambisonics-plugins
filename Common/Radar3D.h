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
#include "Radar2D.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Radar3D  : public Component,
                 public Button::Listener
{
public:
    //==============================================================================
    Radar3D (AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, ZoomSettings* pZoomSettings, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~Radar3D();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* flatArrowDown_png;
    static const int flatArrowDown_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	void setRadarMode(bool fullRadar);
	bool fullRadarFlag;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Radar2D> zyRadar;
    std::unique_ptr<Radar2D> xyRadar;
    std::unique_ptr<ImageButton> btnFull;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Radar3D)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

