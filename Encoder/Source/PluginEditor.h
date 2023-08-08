/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "../../Common/PointSelection.h"
#include "../../Common/RadarComponent.h"
#include "EncoderSettingsDialog.h"
#include "../../Common/HelpDialogManager.h"
#include "../../Common/OSCLogDialogManager.h"
#include "AnimatorDialogManager.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AmbisonicEncoderAudioProcessorEditor  : public AudioProcessorEditor,
                                              public ChangeListener,
                                              public ActionListener,
                                              public juce::Button::Listener
{
public:
    //==============================================================================
    AmbisonicEncoderAudioProcessorEditor (AmbisonicEncoderAudioProcessor& ownerProc);
    ~AmbisonicEncoderAudioProcessorEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void actionListenerCallback(const String& message) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* settings_png;
    static const int settings_pngSize;
    static const char* help_png;
    static const int help_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AmbisonicEncoderAudioProcessor& processor;
	AmbiSourceSet* pSources;
	PointSelection pointSelection;
	EncoderSettings* pEncoderSettings;
	EncoderSettingsDialog* settingsWindow;
    HelpDialogManager helpDialogManager;
    OSCLogDialogManager oscLogDialogManager;
    AnimatorDialogManager animatorDialogManager;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<juce::Label> labelVersion;
    std::unique_ptr<juce::ImageButton> btnSettings;
    std::unique_ptr<juce::Label> labelMessage;
    std::unique_ptr<juce::ImageButton> btnHelp;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

