/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "AnimatorMovement.h"
struct PositionSet
{
    OwnedArray<Point3D<double>> sources;
    OwnedArray<Point3D<double>> groups;
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AnimatorMovementComponent  : public juce::Component,
                                   ActionBroadcaster,
                                   public juce::Button::Listener,
                                   public juce::Slider::Listener
{
public:
    //==============================================================================
    AnimatorMovementComponent (AmbiSourceSet* pSourceSet, AnimatorMovement* pAnimatorMovement, ActionListener* pActionListener);
    ~AnimatorMovementComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void controlDimming();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void refreshControls();

    AmbiSourceSet* pSourceSet;
    AnimatorMovement* pAnimatorMovement;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> group1;
    std::unique_ptr<juce::TextButton> buttonSet;
    std::unique_ptr<juce::Slider> sliderTime;
    std::unique_ptr<juce::ToggleButton> togglePolar;
    std::unique_ptr<juce::TextButton> buttonGo;
    std::unique_ptr<juce::Label> labelG;
    std::unique_ptr<juce::Slider> sliderGroup;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatorMovementComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

