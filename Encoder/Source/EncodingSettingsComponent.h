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
#include "../JuceLibraryCode/JuceHeader.h"
#include "SourceDefinitionComponent.h"
#include "../../Common/PresetManagerDialog.h"
#include "../../Common/ZoomSettings.h"
#include "DistanceEncodingPresetHelper.h"
#include "ComponentArguments.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EncodingSettingsComponent  : public Component,
                                   public ChangeBroadcaster,
                                   public juce::Button::Listener
{
public:
    //==============================================================================
    EncodingSettingsComponent (EncoderSettingsComponentArgs args);
    ~EncodingSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void controlDimming() const;
    void initializePresets();
    void updateEncodingUiElements();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PresetManagerDialog presetManagerDialog;
    EncoderSettingsComponentArgs m_args;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ToggleButton> toggleDistanceEncoding;
    std::unique_ptr<juce::ToggleButton> toggleDoppler;
    std::unique_ptr<juce::TextButton> btnEditDistanceEncoding;
    std::unique_ptr<juce::TextButton> btnManageDistanceEncodingPresets;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncodingSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

