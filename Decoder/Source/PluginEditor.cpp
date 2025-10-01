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



//[Headers] You can add your own extra header files here...
#include "SpeakerSettingsComponent.h"
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AmbisonicsDecoderAudioProcessorEditor::AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), mainProcessor(ownerProc)
{
    //[Constructor_pre] You can add your own custom stuff here..
	settingsWindow = nullptr;
	pSpeakerSet = ownerProc.getSpeakerSet();
	pMovingPoints = ownerProc.getMovingPoints();
	pAmbiSettings = ownerProc.getAmbiSettings();
	pDecoderSettings = ownerProc.getDecoderSettings();
	pFilterSpecification = ownerProc.getFilterSpecification();
	pOscHandler = new OSCHandlerDecoder(pMovingPoints);
    pRadarOptions = ownerProc.getRadarOptions();
	initializeOscHandler();
	//[/Constructor_pre]

    radarComponent.reset (new RadarComponent (pSpeakerSet, pMovingPoints, &pointSelection, ownerProc.getRadarOptions()));
    addAndMakeVisible (radarComponent.get());
    radarComponent->setName ("radarComponent");

    labelVersion.reset (new juce::Label ("labelVersion",
                                         TRANS("Version")));
    addAndMakeVisible (labelVersion.get());
    labelVersion->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelVersion->setJustificationType (juce::Justification::centredRight);
    labelVersion->setEditable (false, false, false);
    labelVersion->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelVersion->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnSettings.reset (new juce::ImageButton ("btnSettings"));
    addAndMakeVisible (btnSettings.get());
    btnSettings->setButtonText (juce::String());
    btnSettings->addListener (this);

    btnSettings->setImages (false, true, true,
                            juce::ImageCache::getFromMemory (BinaryData::settings_png, BinaryData::settings_pngSize), 1.000f, juce::Colour (0x00000000),
                            juce::ImageCache::getFromMemory (BinaryData::settings_png, BinaryData::settings_pngSize), 0.400f, juce::Colour (0x6eee1010),
                            juce::ImageCache::getFromMemory (BinaryData::settings_png, BinaryData::settings_pngSize), 1.000f, juce::Colour (0xc0ee1010));
    btnSettings->setBounds (0, 0, 32, 32);

    btnHelp.reset (new juce::ImageButton ("btnHelp"));
    addAndMakeVisible (btnHelp.get());
    btnHelp->setButtonText (TRANS("new button"));
    btnHelp->addListener (this);

    btnHelp->setImages (false, true, true,
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0xc0ee1010));
    btnHelp->setBounds (32, 4, 24, 24);


    //[UserPreSize]
	setResizable(true, true);
    //[/UserPreSize]

    setSize (400, 700);


    //[Constructor] You can add your own custom stuff here..
	setSize(pDecoderSettings->lastUIWidth, pDecoderSettings->lastUIHeight);
	labelVersion->setText(String(JucePlugin_Name).upToFirstOccurrenceOf("_", false, false) + Constants::getUiVersionString(true), dontSendNotification);
	updateRadarOptions();
    //[/Constructor]
}

AmbisonicsDecoderAudioProcessorEditor::~AmbisonicsDecoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    radarComponent = nullptr;
    labelVersion = nullptr;
    btnSettings = nullptr;
    btnHelp = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	if (settingsWindow != nullptr)
	{
		delete settingsWindow;
	}

	delete pOscHandler;
    //[/Destructor]
}

//==============================================================================
void AmbisonicsDecoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmbisonicsDecoderAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    radarComponent->setBounds (0, 32, getWidth() - 0, getHeight() - 32);
    labelVersion->setBounds (getWidth() - 5 - 111, 4, 111, 24);
    //[UserResized] Add your own custom resize handling here..
	pDecoderSettings->lastUIWidth = getWidth();
	pDecoderSettings->lastUIHeight = getHeight();
    //[/UserResized]
}

void AmbisonicsDecoderAudioProcessorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnSettings.get())
    {
        //[UserButtonCode_btnSettings] -- add your button handler code here..
		if (settingsWindow)
			delete settingsWindow;
		settingsWindow = new SpeakerSettingsDialog(this, new SpeakerSettingsComponent(pSpeakerSet, mainProcessor.getSpeakerPresetHelper(), mainProcessor.getDecodingPresetHelper(), & pointSelection, pAmbiSettings, pDecoderSettings, mainProcessor.getTestSoundGenerator(), this, pFilterSpecification, mainProcessor.getZoomSettingsPointer(), mainProcessor.getChannelLayout()));
		settingsWindow->setVisible(true);
		settingsWindow->updatePosition(getScreenBounds());
        //[/UserButtonCode_btnSettings]
    }
    else if (buttonThatWasClicked == btnHelp.get())
    {
        //[UserButtonCode_btnHelp] -- add your button handler code here..
        helpDialogManager.show(false, this);
        //[/UserButtonCode_btnHelp]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

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

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AmbisonicsDecoderAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public ChangeListener, public ActionListener"
                 constructorParams="AmbisonicsDecoderAudioProcessor&amp; ownerProc"
                 variableInitialisers="AudioProcessorEditor(ownerProc), mainProcessor(ownerProc)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="radarComponent" id="cb26712c5c52dede" memberName="radarComponent"
                    virtualName="" explicitFocusOrder="0" pos="0 32 0M 32M" class="RadarComponent"
                    params="pSpeakerSet, pMovingPoints, &amp;pointSelection, ownerProc.getRadarOptions()"/>
  <LABEL name="labelVersion" id="79dc1bc82b90b8df" memberName="labelVersion"
         virtualName="" explicitFocusOrder="0" pos="5Rr 4 111 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Version" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <IMAGEBUTTON name="btnSettings" id="1c0127439cd8fab6" memberName="btnSettings"
               virtualName="" explicitFocusOrder="0" pos="0 0 32 32" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="settings_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="settings_png" opacityOver="0.4000000059604645"
               colourOver="6eee1010" resourceDown="settings_png" opacityDown="1.0"
               colourDown="c0ee1010"/>
  <IMAGEBUTTON name="btnHelp" id="b20bfc3a18a6b339" memberName="btnHelp" virtualName=""
               explicitFocusOrder="0" pos="32 4 24 24" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="help_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="help_png" opacityOver="0.4000000059604645" colourOver="6eee1010"
               resourceDown="help_png" opacityDown="1.0" colourDown="c0ee1010"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
