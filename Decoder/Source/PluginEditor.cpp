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

#include "SpeakerSettingsComponent.h"
#include "../../Common/SvgHelper.h"
#include "PluginEditor.h"

AmbisonicsDecoderAudioProcessorEditor::AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), mainProcessor(ownerProc)
{
	settingsWindow = nullptr;
	pSpeakerSet = ownerProc.getSpeakerSet();
	pMovingPoints = ownerProc.getMovingPoints();
	pAmbiSettings = ownerProc.getAmbiSettings();
	pDecoderSettings = ownerProc.getDecoderSettings();
	pFilterSpecification = ownerProc.getFilterSpecification();
	pOscHandler = new OSCHandlerDecoder(pMovingPoints);
    pRadarOptions = ownerProc.getRadarOptions();
	initializeOscHandler();

    radarComponent.reset (new RadarComponent (pSpeakerSet, pMovingPoints, &pointSelection, ownerProc.getRadarOptions()));
    addAndMakeVisible (radarComponent.get());
    radarComponent->setName ("radarComponent");

    btnSettings.reset (new DrawableButton ("btnSettings", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnSettings.get());
    btnSettings->setButtonText (TRANS("Settings"));
    btnSettings->addListener (this);
    SvgHelper::loadSVGIcon(btnSettings.get(), BinaryData::settings_icon_svg, BinaryData::settings_icon_svgSize, String("Settings"));

    btnHelp.reset (new DrawableButton ("btnHelp", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnHelp.get());
    btnHelp->setButtonText (TRANS("Help"));
    btnHelp->addListener (this);
    SvgHelper::loadSVGIcon(btnHelp.get(), BinaryData::help_icon_svg, BinaryData::help_icon_svgSize, String(String(JucePlugin_Name).upToFirstOccurrenceOf("_", false, false) + Constants::getUiVersionString(true)));

	setResizable(true, true);
    setSize (400, 700);

	setSize(pDecoderSettings->lastUIWidth, pDecoderSettings->lastUIHeight);
	updateRadarOptions();
}

AmbisonicsDecoderAudioProcessorEditor::~AmbisonicsDecoderAudioProcessorEditor()
{
    radarComponent = nullptr;
    btnSettings = nullptr;
    btnHelp = nullptr;

	if (settingsWindow != nullptr)
	{
		delete settingsWindow;
	}

	delete pOscHandler;
}

void AmbisonicsDecoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void AmbisonicsDecoderAudioProcessorEditor::resized()
{
    radarComponent->setBounds (0, 32, getWidth() - 0, getHeight() - 32);
    btnSettings->setBounds (2, 2, 28, 28);
    btnHelp->setBounds (34, 2, 28, 28);

    pDecoderSettings->lastUIWidth = getWidth();
	pDecoderSettings->lastUIHeight = getHeight();
}

void AmbisonicsDecoderAudioProcessorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnSettings.get())
    {
		if (settingsWindow)
			delete settingsWindow;
		settingsWindow = new SpeakerSettingsDialog(this, new SpeakerSettingsComponent(pSpeakerSet, mainProcessor.getSpeakerPresetHelper(), mainProcessor.getDecodingPresetHelper(), & pointSelection, pAmbiSettings, pDecoderSettings, mainProcessor.getTestSoundGenerator(), this, pFilterSpecification, mainProcessor.getZoomSettingsPointer(), mainProcessor.getChannelLayout()));
		settingsWindow->setVisible(true);
		settingsWindow->updatePosition(getScreenBounds());
    }
    else if (buttonThatWasClicked == btnHelp.get())
    {
        helpDialogManager.show(false, this);
    }
}

void AmbisonicsDecoderAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster*)
{
	initializeOscHandler();
	updateRadarOptions();
}

void AmbisonicsDecoderAudioProcessorEditor::initializeOscHandler()
{
	// update timeout
	pRadarOptions->displayTimeout = pDecoderSettings->oscReceiveTimeoutMs;

	pOscHandler->stop();

	if (pDecoderSettings->oscReceive)
	{
		if (!pOscHandler->start(pDecoderSettings->oscReceivePort))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC-Handler on port " + String(pDecoderSettings->oscReceivePort), "OK");
		}
	}
}

void AmbisonicsDecoderAudioProcessorEditor::updateRadarOptions()
{
	pRadarOptions->showEditablePoints = pDecoderSettings->editMode;
	pRadarOptions->showDisplayOnlyPoints = !pDecoderSettings->editMode;
	if(!pDecoderSettings->editMode)
		pointSelection.unselectPoint();
	radarComponent->setPointInfoVisible(pDecoderSettings->editMode);
}

void AmbisonicsDecoderAudioProcessorEditor::actionListenerCallback(const String& message)
{
	if (message == ACTION_CLOSE_SETTINGS)
	{
		delete settingsWindow;
		settingsWindow = nullptr;
	}
}
