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

#include "../../Common/RadarComponent.h"
#include "EncoderSettingsComponent.h"
#include "TimelineComponent.h"
#include "../../Common/SvgHelper.h"
#include "PluginEditor.h"

AmbisonicEncoderAudioProcessorEditor::AmbisonicEncoderAudioProcessorEditor (AmbisonicEncoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), mainProcessor(ownerProc)
{
	settingsWindow = nullptr;
    pSources = mainProcessor.getSources();
	pEncoderSettings = mainProcessor.getEncoderSettings();
    mainProcessor.getAnimatorEngine()->addChangeListener(this);

    radarComponent.reset (new RadarComponent (pSources, nullptr, &pointSelection, mainProcessor.getRadarOptions()));
    addAndMakeVisible (radarComponent.get());
    radarComponent->setName ("radarComponent");

    btnSettings.reset (new DrawableButton ("btnSettings", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnSettings.get());
    btnSettings->setButtonText (TRANS("Settings"));
    btnSettings->addListener (this);
    SvgHelper::loadSVGIcon(btnSettings.get(), BinaryData::settings_icon_svg, BinaryData::settings_icon_svgSize, String("Settings"));

    labelMessage.reset (new juce::Label ("labelMessage",
                                         juce::String()));
    addAndMakeVisible (labelMessage.get());
    labelMessage->setFont (juce::Font (juce::FontOptions(11.70f, juce::Font::plain)));
    labelMessage->setJustificationType (juce::Justification::centred);
    labelMessage->setEditable (false, false, false);
    labelMessage->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelMessage->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnHelp.reset (new DrawableButton ("btnHelp", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnHelp.get());
    btnHelp->setButtonText (TRANS("Help"));
    btnHelp->addListener (this);
    SvgHelper::loadSVGIcon(btnHelp.get(), BinaryData::help_icon_svg, BinaryData::help_icon_svgSize, String(String(JucePlugin_Name).upToFirstOccurrenceOf("_", false, false) + Constants::getUiVersionString(true)));

    
    btnAnimator.reset (new ColorBorderDrawableButton("Animator", pEncoderSettings->animatorSettings.enable));
    addAndMakeVisible (btnAnimator.get());
    btnAnimator->setButtonText (TRANS("new button"));
    btnAnimator->addListener (this);
    SvgHelper::loadSVGIcon(btnAnimator.get(), BinaryData::animator_icon_svg, BinaryData::animator_icon_svgSize, String("Animator"));
    
	setResizable(true, true);
    setSize (400, 700);

	ownerProc.getStatusMessageHandler()->registerLabel(labelMessage.get());
}

AmbisonicEncoderAudioProcessorEditor::~AmbisonicEncoderAudioProcessorEditor()
{
	mainProcessor.getStatusMessageHandler()->unregisterLabel();
    mainProcessor.getAnimatorEngine()->removeChangeListener(this);

    radarComponent = nullptr;
    btnSettings = nullptr;
    labelMessage = nullptr;
    btnHelp = nullptr;

	if (settingsWindow != nullptr)
	{
		delete settingsWindow;
	}
    
    debugLogWindow = nullptr;
}

void AmbisonicEncoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void AmbisonicEncoderAudioProcessorEditor::resized()
{
    radarComponent->setBounds (0, 32, getWidth() - 0, getHeight() - 32);
    btnSettings->setBounds (2, 2, 28, 28);
    btnHelp->setBounds (34, 2, 28, 28);
    btnAnimator->setBounds(getWidth() - 30, 2, 28, 28);
    labelMessage->setBounds (64, 4, getWidth() - 32, 24);
}

void AmbisonicEncoderAudioProcessorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnSettings.get())
    {
		if (settingsWindow)
			delete settingsWindow;
        EncoderSettingsComponentArgs args {
            this,
            pEncoderSettings,
            pSources,
            &pointSelection,
            mainProcessor.getAudioParams(),
            mainProcessor.getZoomSettingsPointer(),
            mainProcessor.getStatusMessageHandler(),
            mainProcessor.getPresetHelper(),
            mainProcessor.getDistanceEncodingPresetHelper(),
            mainProcessor.getCustomOscRxPresetHelper(),
            mainProcessor.getCustomOscTxPresetHelper(),
            &oscLogDialogManager,
            mainProcessor.getDawParameter(),
            mainProcessor.getOscHandler(),
            mainProcessor.getChannelLayout()
        };
		settingsWindow = new EncoderSettingsDialog(this, new EncoderSettingsComponent(args));
		settingsWindow->setVisible(true);
        settingsWindow->updatePosition(getScreenBounds());
    }
    else if (buttonThatWasClicked == btnHelp.get())
    {
        if(ModifierKeys::currentModifiers.isCommandDown() && ModifierKeys::currentModifiers.isCtrlDown() && ModifierKeys::currentModifiers.isAltDown() && ModifierKeys::currentModifiers.isShiftDown())
        {
            animatorDialogManager.show(pSources, mainProcessor.getAnimatorDataset(), this);
            return;
        }

        helpDialogManager.show(true, this);
    }

    else if (buttonThatWasClicked == btnAnimator.get())
    {
        timelineDialogManager.show(this, &mainProcessor, &pointSelection, mainProcessor.getAnimatorEngine());
    }
}

void AmbisonicEncoderAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == mainProcessor.getAnimatorEngine())
    {
        repaint();
    }
    else
    {
        mainProcessor.initializeOscSender();
    }
}

void AmbisonicEncoderAudioProcessorEditor::actionListenerCallback(const String& message)
{
	if (message == ACTION_CLOSE_SETTINGS)
	{
		delete settingsWindow;
		settingsWindow = nullptr;
	}
}

bool AmbisonicEncoderAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress('d', juce::ModifierKeys::ctrlModifier | juce::ModifierKeys::shiftModifier, 0))
    {
        toggleDebugLogWindow();
        return true;
    }
    return false;
}

void AmbisonicEncoderAudioProcessorEditor::toggleDebugLogWindow()
{
    if (debugLogWindow == nullptr)
    {
        debugLogWindow = std::make_unique<DebugLogWindow>(mainProcessor.debugLogHandler);
        debugLogWindow->setVisible(true);
    }
    else
    {
        if (debugLogWindow->isVisible())
        {
            debugLogWindow->setVisible(false);
        }
        else
        {
            debugLogWindow->setVisible(true);
            debugLogWindow->toFront(true);
        }
    }
}
