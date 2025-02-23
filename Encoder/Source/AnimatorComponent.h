/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PresetManagerDialog.h"
#include "AnimatorActionComponent.h"
#include "AnimatorMovementComponent.h"
#include "EncoderSettings.h"
#include "AnimatorDataset.h"
#include "AnimatorPresetHelper.h"

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
                           public ActionListener,
                           public TextButton::Listener
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
    void setData();
    
    AmbiSourceSet* pSourceSet;
    AnimatorDataset* pAnimatorDataset;
    std::unique_ptr<AnimatorPresetHelper> animatorPresetHelper;
    PresetManagerDialog presetManagerDialog;

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
    std::unique_ptr<TextButton> btnPresets;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatorComponent)

        // Inherited via Listener
        void buttonClicked(Button*) override;
};

//[EndFile] You can add extra defines here...
//[/EndFile]

