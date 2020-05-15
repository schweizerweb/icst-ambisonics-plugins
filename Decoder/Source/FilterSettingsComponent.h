/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
                                 public ComboBox::Listener,
                                 public Slider::Listener,
                                 public Button::Listener
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

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	FilterInfo* pFilterInfo;
    FilterPresetHelper* pPresetHelper;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> comboBoxType;
    std::unique_ptr<Slider> sliderFrequency;
    std::unique_ptr<Label> labelFrequency;
    std::unique_ptr<Label> labelFilterType;
    std::unique_ptr<Label> labelQ;
    std::unique_ptr<Slider> sliderQ;
    std::unique_ptr<IIRFilterGraph> filterGraph;
    std::unique_ptr<Label> labelGain;
    std::unique_ptr<Slider> sliderGain;
    std::unique_ptr<ComboBox> comboBoxFilterPreset;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<TextButton> buttonSave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

