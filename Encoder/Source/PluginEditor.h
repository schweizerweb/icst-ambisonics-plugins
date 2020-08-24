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
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "../../Common/PointSelection.h"
#include "../../Common/RadarComponent.h"
#include "EncoderSettingsDialog.h"
#include "../../Common/HelpDialogManager.h"
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
                                              public Button::Listener
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

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

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
	RadarOptions radarOptions;
	EncoderSettings* pEncoderSettings;
	EncoderSettingsDialog* settingsWindow;
    HelpDialogManager helpDialogManager;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<Label> labelVersion;
    std::unique_ptr<ImageButton> btnSettings;
    std::unique_ptr<Label> labelMessage;
    std::unique_ptr<ImageButton> btnHelp;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

