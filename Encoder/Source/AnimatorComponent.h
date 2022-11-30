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
#include "AnimatorMovementComponent.h"
#include "EncoderSettings.h"
#include "AnimatorDataset.h"

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
                           public ActionListener
{
public:
    //==============================================================================
    AnimatorComponent (AmbiSourceSet* pSourceSet, AnimatorDataset* pAnimatorDataset);
    ~AnimatorComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback(int timerID) override;
    void actionListenerCallback(const juce::String &message) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.


    void calculateStepsTo(Point3D<double> origin, Point3D<double> target, OwnedArray<Point3D<float>>* pStepArray, bool isPolar, int stepCount);
    void setPreset(PositionSet* pSet, int groupIndex);
    void performAction(AnimatorAction* pAction);
    void refreshControls();
    
    AmbiSourceSet* pSourceSet;
    AnimatorDataset* pAnimatorDataset;

#if MULTI_ENCODER_MODE
    OwnedArray<Point3D<float>> groupSteps[MAXIMUM_NUMBER_OF_GROUPS];
    int currentStep[MAXIMUM_NUMBER_OF_GROUPS];
#endif
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<AnimatorActionComponent> action1;
    std::unique_ptr<AnimatorActionComponent> action2;
    std::unique_ptr<AnimatorMovementComponent> preset1;
    std::unique_ptr<AnimatorMovementComponent> preset2;
    std::unique_ptr<AnimatorMovementComponent> preset3;
    std::unique_ptr<AnimatorMovementComponent> preset4;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

