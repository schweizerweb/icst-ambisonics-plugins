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

