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
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "../../Common/PointSelection.h"
#include "../../Common/RadarComponent.h"
#include "EncoderSettingsDialog.h"
#include "../../Common/HelpDialogManager.h"
#include "../../Common/OSCLogDialogManager.h"
#include "TimelineDialog.h"
#include "../../Common/DebugLogWindow.h"
#include "../../Common/ColorBorderDrawableButton.h"

class AmbisonicEncoderAudioProcessorEditor  : public AudioProcessorEditor,
                                              public ChangeListener,
                                              public ActionListener,
                                              public juce::Button::Listener
{
public:
    AmbisonicEncoderAudioProcessorEditor (AmbisonicEncoderAudioProcessor& ownerProc);
    ~AmbisonicEncoderAudioProcessorEditor() override;

	void actionListenerCallback(const String& message) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
    bool keyPressed(const juce::KeyPress& key) override;
    void toggleDebugLogWindow();

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void showInfoCallout();
    
private:
    AmbisonicEncoderAudioProcessor& mainProcessor;
	AmbiSourceSet* pSources;
	PointSelection pointSelection;
	EncoderSettings* pEncoderSettings;
	EncoderSettingsDialog* settingsWindow;
    HelpDialogManager helpDialogManager;
    OSCLogDialogManager oscLogDialogManager;
    SharedResourcePointer<TooltipWindow> tooltipWindow;
    TimelineDialogManager timelineDialogManager;

    std::unique_ptr<DebugLogWindow> debugLogWindow;
    std::unique_ptr<RadarComponent> radarComponent;
    std::unique_ptr<DrawableButton> btnSettings;
    std::unique_ptr<juce::Label> labelMessage;
    std::unique_ptr<DrawableButton> btnHelp;
    std::unique_ptr<ColorBorderDrawableButton> btnAnimator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessorEditor)
};
