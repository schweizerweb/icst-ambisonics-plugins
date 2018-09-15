/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/PointSelection.h"
#include "../../Common/RadarComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AmbisonicEncoderAudioProcessorEditor  : public AudioProcessorEditor,
                                              public Button::Listener
{
public:
    //==============================================================================
    AmbisonicEncoderAudioProcessorEditor (AmbisonicEncoderAudioProcessor& ownerProc);
    ~AmbisonicEncoderAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* settings_png;
    static const int settings_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AmbisonicEncoderAudioProcessor& processor;
	OwnedArray<AmbiPoint>* pSourcesArray;
	PointSelection pointSelection;
	RadarOptions radarOptions;
	EncoderSettings* pEncoderSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<Label> labelVersion;
    std::unique_ptr<ImageButton> btnSettings;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
