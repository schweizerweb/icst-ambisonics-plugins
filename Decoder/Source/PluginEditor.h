/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.4

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
#include "SpeakerSettingsDialog.h"
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
                                               public ChangeListener,
                                               public ActionListener,
                                               public Button::Listener
{
public:
    //==============================================================================
    AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc);
    ~AmbisonicsDecoderAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void initializeOscHandler();
	void updateRadarOptions();
	void actionListenerCallback(const String& message) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* settings_png;
    static const int settings_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbisonicsDecoderAudioProcessor& processor;
	AmbiDataSet* pSpeakerSet;
	AmbiDataSet* pMovingPoints;
	AmbiSettings* pAmbiSettings;
	DecoderSettings *pDecoderSettings;
	OwnedArray<PresetInfo> presets;
	OSCHandler* pOscHandler;
	PointSelection pointSelection;
	RadarOptions radarOptions;
	SpeakerSettingsDialog* settingsWindow;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<Label> labelVersion;
    std::unique_ptr<ImageButton> btnSettings;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

