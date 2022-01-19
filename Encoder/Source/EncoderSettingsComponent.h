/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "OSCSettingsComponent.h"
#include "OSCRxSettingsComponent.h"
#include "RadarSettingsComponent.h"
#include "EncodingSettingsComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EncoderSettingsComponent  : public Component,
                                  public ActionBroadcaster
{
public:
    //==============================================================================
    EncoderSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, ZoomSettings* pZoomSettings, StatusMessageHandler* pStatusMessageHandler, EncoderPresetHelper* pPresetHelper, DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper, OSCLogDialogManager* pOscLogManager);
    ~EncoderSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> tabbedComponent;
    std::unique_ptr<juce::Label> labelDevelopmentVersion;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncoderSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

