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
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Common/FilterInfo.h"
#include "IIRFilterGraph.h"
#include "FilterPresetHelper.h"
#include "../../Common/PresetManagerDialog.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FilterSettingsComponent  : public Component,
                                 public ChangeBroadcaster,
                                 public ActionListener,
                                 public juce::ComboBox::Listener,
                                 public juce::Slider::Listener,
                                 public juce::Button::Listener
{
public:
    //==============================================================================
    FilterSettingsComponent (FilterInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* pPresetHelper);
    ~FilterSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updatePresetComboBox();
    void updateUi();
    void actionListenerCallback(const String &message) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	FilterInfo* pFilterInfo;
    FilterPresetHelper* pPresetHelper;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> comboBoxType;
    std::unique_ptr<juce::Slider> sliderFrequency;
    std::unique_ptr<juce::Label> labelFrequency;
    std::unique_ptr<juce::Label> labelFilterType;
    std::unique_ptr<juce::Label> labelQ;
    std::unique_ptr<juce::Slider> sliderQ;
    std::unique_ptr<IIRFilterGraph> filterGraph;
    std::unique_ptr<juce::Label> labelGain;
    std::unique_ptr<juce::Slider> sliderGain;
    std::unique_ptr<juce::ComboBox> comboBoxFilterPreset;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

