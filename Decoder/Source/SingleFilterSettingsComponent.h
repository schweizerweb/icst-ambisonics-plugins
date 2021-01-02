/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SingleFilterSettingsComponent  : public juce::Component,
                                       public ChangeBroadcaster,
                                       public juce::Slider::Listener,
                                       public juce::ComboBox::Listener
{
public:
    //==============================================================================
    SingleFilterSettingsComponent (FilterInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener);
    ~SingleFilterSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updateUi();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    FilterInfo* pFilterInfo;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupMain;
    std::unique_ptr<juce::Slider> sliderGain;
    std::unique_ptr<juce::Slider> sliderQ;
    std::unique_ptr<juce::ComboBox> comboBoxType;
    std::unique_ptr<juce::Slider> sliderFrequency;
    std::unique_ptr<juce::Label> labelQ;
    std::unique_ptr<juce::Label> labelGain;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleFilterSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

