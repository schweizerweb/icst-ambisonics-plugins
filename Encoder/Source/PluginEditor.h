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
#include "../../Common/PointSelection.h"
#include "../../Common/RadarComponent.h"
#include "EncoderSettingsDialog.h"
#include "../../Common/HelpDialogManager.h"
#include "../../Common/OSCLogDialogManager.h"
#include "AnimatorDialogManager.h"
#include "TimelineDialog.h"
#include "../../Common/DebugLogWindow.h"
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
    bool keyPressed(const juce::KeyPress& key) override;
    void toggleDebugLogWindow();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AmbisonicEncoderAudioProcessor& mainProcessor;
	AmbiSourceSet* pSources;
	PointSelection pointSelection;
	EncoderSettings* pEncoderSettings;
	EncoderSettingsDialog* settingsWindow;
    HelpDialogManager helpDialogManager;
    OSCLogDialogManager oscLogDialogManager;
    AnimatorDialogManager animatorDialogManager;
    SharedResourcePointer<TooltipWindow> tooltipWindow;
    TimelineDialogManager timelineDialogManager;
    std::unique_ptr<DebugLogWindow> debugLogWindow;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<juce::Label> labelVersion;
    std::unique_ptr<juce::ImageButton> btnSettings;
    std::unique_ptr<juce::Label> labelMessage;
    std::unique_ptr<juce::ImageButton> btnHelp;
    std::unique_ptr<juce::ImageButton> btnAnimator;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
