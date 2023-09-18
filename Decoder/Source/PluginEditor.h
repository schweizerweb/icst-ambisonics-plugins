/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "DecoderSettings.h"
#include "SpeakerSettingsDialog.h"
#include "../../Common/RadarComponent.h"
#include "../../Common/RadarOptions.h"
#include "OSCHandlerDecoder.h"
#include "../../Common/HelpDialogManager.h"
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
                                               public juce::Button::Listener
{
public:
    //==============================================================================
    AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc);
    ~AmbisonicsDecoderAudioProcessorEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void initializeOscHandler();
	void updateRadarOptions();
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
	AmbisonicsDecoderAudioProcessor& mainProcessor;
	AmbiSpeakerSet* pSpeakerSet;
	AmbiSourceSet* pMovingPoints;
	AmbiSettings* pAmbiSettings;
	DecoderSettings *pDecoderSettings;
	OSCHandler* pOscHandler;
	PointSelection pointSelection;
	SpeakerSettingsDialog* settingsWindow;
	dsp::ProcessSpec* pFilterSpecification;
    HelpDialogManager helpDialogManager;
    RadarOptions* pRadarOptions;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<juce::Label> labelVersion;
    std::unique_ptr<juce::ImageButton> btnSettings;
    std::unique_ptr<juce::ImageButton> btnHelp;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
