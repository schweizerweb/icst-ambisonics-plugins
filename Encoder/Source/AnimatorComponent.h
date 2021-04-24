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
#include "../../Common/AmbiSourceSet.h"

#define STEP_TIMER_ID       1
#define STEP_TIMER_INTERVAL 50
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AnimatorComponent  : public juce::Component,
                           public MultiTimer,
                           public juce::Button::Listener,
                           public juce::Slider::Listener,
                           public juce::ComboBox::Listener
{
public:
    //==============================================================================
    AnimatorComponent (AmbiSourceSet* pSourceSet);
    ~AnimatorComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback(int timerID) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void calculateStepsTo(OwnedArray<AmbiSource>* pPositions, bool isPolar, double timeSec);
    AmbiSourceSet* pSourceSet;
    OwnedArray<AmbiSource> set1;
    OwnedArray<AmbiSource> set2;
    HashMap<int, OwnedArray<Point3D<float>>*> steps;
    int currentStep;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> group1;
    std::unique_ptr<juce::TextButton> buttonSet1;
    std::unique_ptr<juce::Slider> sliderTime1;
    std::unique_ptr<juce::ToggleButton> togglePolar1;
    std::unique_ptr<juce::TextButton> buttonGo1;
    std::unique_ptr<juce::GroupComponent> group2;
    std::unique_ptr<juce::TextButton> buttonSet2;
    std::unique_ptr<juce::Slider> sliderTime2;
    std::unique_ptr<juce::ToggleButton> togglePolar2;
    std::unique_ptr<juce::TextButton> buttonGo2;
    std::unique_ptr<juce::GroupComponent> groupRotationOrigin;
    std::unique_ptr<juce::ComboBox> comboBoxRotateOrigin;
    std::unique_ptr<juce::Slider> sliderRotateOriginTime;
    std::unique_ptr<juce::Label> labelGroup;
    std::unique_ptr<juce::Label> labelTime;
    std::unique_ptr<juce::Slider> sliderRotateOriginRepetoitions;
    std::unique_ptr<juce::Label> labelRepetitions;
    std::unique_ptr<juce::TextButton> buttonRotationOriginGo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

