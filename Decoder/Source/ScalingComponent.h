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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "../../Common/AmbiSpeakerSet.h"
#include "../../Common/ZoomSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ScalingComponent  : public juce::Component,
                          public ChangeBroadcaster,
                          public juce::Slider::Listener,
                          public juce::Button::Listener
{
public:
    //==============================================================================
    ScalingComponent (ChangeListener* pListener, AmbiSpeakerSet* pSpeakerSet, ZoomSettings* pZoomSettings);
    ~ScalingComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void calculateAndDisplayRoomSize();
    AmbiSpeakerSet* pSpeakerSet;
    ZoomSettings* pZoomSettings;
    double currentRoomSizeX;
    double currentRoomSizeY;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> labelProportionalScale;
    std::unique_ptr<juce::Label> labelRoomSize;
    std::unique_ptr<juce::Slider> sliderRoomSizeX;
    std::unique_ptr<juce::Slider> sliderRoomSizeY;
    std::unique_ptr<juce::Label> labelX;
    std::unique_ptr<juce::TextButton> buttonDec;
    std::unique_ptr<juce::TextButton> buttonInc;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScalingComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

