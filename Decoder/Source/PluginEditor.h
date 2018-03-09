/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "PresetInfo.h"
#include "DecoderSettings.h"
#include "../../Common/RadarComponent.h"
#include "../../Common/RadarOptions.h"
#include "../../Common/OSCHandler.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AmbisonicsDecoderAudioProcessorEditor  : public AudioProcessorEditor,
                                               public Button::Listener
{
public:
    //==============================================================================
    AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc);
    ~AmbisonicsDecoderAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void initializeOscHandler() const;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbisonicsDecoderAudioProcessor& processor;
	OwnedArray<AmbiPoint>* pSpeakerArray;
	OwnedArray<AmbiPoint>* pMovingPointsArray;
	AmbiSettings* pAmbiSettings;
	DecoderSettings *pDecoderSettings;
	OwnedArray<PresetInfo> presets;
	ScopedPointer<OSCHandler> oscHandler;
	PointSelection pointSelection;
	RadarOptions radarOptions;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<RadarComponent> component;
    ScopedPointer<TextButton> buttonConfigure;
    ScopedPointer<TextButton> buttonConfigurePlugin;
    ScopedPointer<Label> labelVersion;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
