/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "../../Common/RadarComponent.h"
#include "PresetInfo.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AmbisonicsDecoderAudioProcessorEditor  : public AudioProcessorEditor,
                                               public Timer,
                                               public ComboBoxListener,
                                               public ButtonListener
{
public:
    //==============================================================================
    AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc);
    ~AmbisonicsDecoderAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void timerCallback() override;
	void loadPreset(PresetInfo* preset) const;
	void updateComboBox(String elementToSelect = String::empty);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbisonicsDecoderAudioProcessor& processor;
	Array<AmbiPoint>* pSpeakerArray;
	Array<AmbiPoint>* pMovingPointsArray;
	OwnedArray<PresetInfo> presets;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> comboBoxChannelConfig;
    ScopedPointer<RadarComponent> component;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> buttonLoad;
    ScopedPointer<TextButton> buttonSave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
