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
    AnimatorActionComponent (AmbiSourceSet* pSourceSet, String title);
    ~AnimatorActionComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    AnimatorAction action;
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

