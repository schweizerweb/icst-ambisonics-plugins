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
#include "../../Common/AmbiSourceSet.h"
#include "AnimatorActionComponent.h"
#include "EncoderSettings.h"

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
                           public juce::Slider::Listener
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



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    struct PositionSet
    {
        OwnedArray<Point3D<double>> sources;
        OwnedArray<Point3D<double>> groups;
    };

    void calculateStepsTo(PositionSet* pPositions, bool isPolar, double timeSec);
    void calculateStepsTo(Point3D<double> origin, Point3D<double> target, OwnedArray<Point3D<float>>* pStepArray, bool isPolar, int stepCount);
    void setPreset(PositionSet* pSet, int groupIndex);
    void performAction(AnimatorAction* pAction);
    void controlDimming();

    AmbiSourceSet* pSourceSet;
    PositionSet set1;
    PositionSet set2;
    PositionSet set3;
    PositionSet set4;
    OwnedArray<Point3D<float>> steps[JucePlugin_MaxNumInputChannels];
#if MULTI_ENCODER_MODE
    OwnedArray<Point3D<float>> groupSteps[MAXIMUM_NUMBER_OF_GROUPS];
#endif
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
    std::unique_ptr<juce::GroupComponent> group3;
    std::unique_ptr<juce::TextButton> buttonSet3;
    std::unique_ptr<juce::Slider> sliderTime3;
    std::unique_ptr<juce::ToggleButton> togglePolar3;
    std::unique_ptr<juce::TextButton> buttonGo3;
    std::unique_ptr<juce::GroupComponent> group4;
    std::unique_ptr<juce::TextButton> buttonSet4;
    std::unique_ptr<juce::Slider> sliderTime4;
    std::unique_ptr<juce::ToggleButton> togglePolar4;
    std::unique_ptr<juce::TextButton> buttonGo4;
    std::unique_ptr<AnimatorActionComponent> action1;
    std::unique_ptr<AnimatorActionComponent> action2;
    std::unique_ptr<juce::Label> labelG;
    std::unique_ptr<juce::Slider> sliderG1;
    std::unique_ptr<juce::Label> labelG2;
    std::unique_ptr<juce::Slider> sliderG2;
    std::unique_ptr<juce::Label> labelG3;
    std::unique_ptr<juce::Slider> sliderG3;
    std::unique_ptr<juce::Label> labelG4;
    std::unique_ptr<juce::Slider> sliderG4;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

