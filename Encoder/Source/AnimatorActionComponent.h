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
#include "AnimatorAction.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AnimatorActionComponent  : public juce::Component,
                                 public juce::ComboBox::Listener,
                                 public juce::Button::Listener,
                                 public juce::Slider::Listener
{
public:
    //==============================================================================
    AnimatorActionComponent (AmbiSourceSet* pSourceSet, AnimatorAction* pAnimatorAction);
    ~AnimatorActionComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void refreshControls();
    
    AmbiSourceSet* pSourceSet;
    AnimatorAction* pAnimatorAction;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupMain;
    std::unique_ptr<juce::ComboBox> comboBoxGroup;
    std::unique_ptr<juce::Label> labelGroup;
    std::unique_ptr<juce::ToggleButton> toggleEnable;
    std::unique_ptr<juce::GroupComponent> groupRotation;
    std::unique_ptr<juce::Label> labelRotationX;
    std::unique_ptr<juce::Slider> sliderRotationX;
    std::unique_ptr<juce::Label> labelRotationY;
    std::unique_ptr<juce::Slider> sliderRotationY;
    std::unique_ptr<juce::Label> labelRotationZ;
    std::unique_ptr<juce::Slider> sliderRotationZ;
    std::unique_ptr<juce::GroupComponent> groupRotationOrigin;
    std::unique_ptr<juce::Label> labelRotationOriginX;
    std::unique_ptr<juce::Slider> sliderRotationOriginX;
    std::unique_ptr<juce::Label> labelRotationOriginY;
    std::unique_ptr<juce::Slider> sliderRotationOriginY;
    std::unique_ptr<juce::Label> labelRotationOriginZ;
    std::unique_ptr<juce::Slider> sliderRotationOriginZ;
    std::unique_ptr<juce::GroupComponent> groupStretch;
    std::unique_ptr<juce::Label> labelStretchSimple;
    std::unique_ptr<juce::Slider> sliderStretchSimple;
    std::unique_ptr<juce::TextButton> buttonReset;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatorActionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

