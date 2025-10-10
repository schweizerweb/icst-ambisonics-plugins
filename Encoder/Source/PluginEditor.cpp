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
#include "../../Common/RadarComponent.h"
#include "EncoderSettingsComponent.h"
#include "TimelineComponent.h"
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AmbisonicEncoderAudioProcessorEditor::AmbisonicEncoderAudioProcessorEditor (AmbisonicEncoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), mainProcessor(ownerProc)
{
    //[Constructor_pre] You can add your own custom stuff here..
	settingsWindow = nullptr;
    pSources = mainProcessor.getSources();
	pEncoderSettings = mainProcessor.getEncoderSettings();
    mainProcessor.getAnimatorEngine()->addChangeListener(this);
    //[/Constructor_pre]

    radarComponent.reset (new RadarComponent (pSources, nullptr, &pointSelection, mainProcessor.getRadarOptions()));
    addAndMakeVisible (radarComponent.get());
    radarComponent->setName ("radarComponent");

    btnSettings.reset (new juce::ImageButton ("btnSettings"));
    addAndMakeVisible (btnSettings.get());
    btnSettings->setButtonText (TRANS("new button"));
    btnSettings->addListener (this);

    btnSettings->setImages (false, true, true,
                            juce::ImageCache::getFromMemory (BinaryData::settings_png, BinaryData::settings_pngSize), 1.000f, juce::Colour (0x00000000),
                            juce::ImageCache::getFromMemory (BinaryData::settings_png, BinaryData::settings_pngSize), 0.400f, juce::Colour (0x6eee1010),
                            juce::ImageCache::getFromMemory (BinaryData::settings_png, BinaryData::settings_pngSize), 1.000f, juce::Colour (0xc0ee1010));

    labelMessage.reset (new juce::Label ("labelMessage",
                                         juce::String()));
    addAndMakeVisible (labelMessage.get());
    labelMessage->setFont (juce::Font (juce::FontOptions(11.70f, juce::Font::plain)));
    labelMessage->setJustificationType (juce::Justification::centred);
    labelMessage->setEditable (false, false, false);
    labelMessage->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelMessage->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnHelp.reset (new juce::ImageButton ("btnHelp"));
    addAndMakeVisible (btnHelp.get());
    btnHelp->setButtonText (TRANS("new button"));
    btnHelp->addListener (this);
    btnHelp->setTooltip(String(JucePlugin_Name).upToFirstOccurrenceOf("_", false, false) + Constants::getUiVersionString(true));
    btnHelp->setImages (false, true, true,
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0xc0ee1010));
    
    btnAnimator.reset (new ColorBorderButton(pEncoderSettings->animatorSettings.enable));
    addAndMakeVisible (btnAnimator.get());
    btnAnimator->setButtonText (TRANS("new button"));
    btnAnimator->addListener (this);
    
    btnAnimator->setImages (false, true, true,
        juce::ImageCache::getFromMemory (BinaryData::animator_icon_png, BinaryData::animator_icon_pngSize), 1.000f, juce::Colour (0x00000000),
        juce::ImageCache::getFromMemory (BinaryData::animator_icon_png, BinaryData::animator_icon_pngSize), 0.400f, juce::Colour (0x6eee1010),
        juce::ImageCache::getFromMemory (BinaryData::animator_icon_png, BinaryData::animator_icon_pngSize), 1.000f, juce::Colour (0xc0ee1010));
    
    //[UserPreSize]
	setResizable(true, true);
    //[/UserPreSize]

    setSize (400, 700);


    //[Constructor] You can add your own custom stuff here..
	ownerProc.getStatusMessageHandler()->registerLabel(labelMessage.get());
    //[/Constructor]
}

AmbisonicEncoderAudioProcessorEditor::~AmbisonicEncoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	mainProcessor.getStatusMessageHandler()->unregisterLabel();
    mainProcessor.getAnimatorEngine()->removeChangeListener(this);
    //[/Destructor_pre]

    radarComponent = nullptr;
    btnSettings = nullptr;
    labelMessage = nullptr;
    btnHelp = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	if (settingsWindow != nullptr)
	{
		delete settingsWindow;
	}
    
    debugLogWindow = nullptr;
    //[/Destructor]
}

//==============================================================================
void AmbisonicEncoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmbisonicEncoderAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    radarComponent->setBounds (0, 32, getWidth() - 0, getHeight() - 32);
    btnSettings->setBounds (0, 0, 32, 32);
    btnHelp->setBounds (32, 4, 24, 24);
    btnAnimator->setBounds(getWidth() - 28, 4, 24, 24);
    labelMessage->setBounds (64, 4, getWidth() - 32, 24);
    
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AmbisonicEncoderAudioProcessorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnSettings.get())
    {
        //[UserButtonCode_btnSettings] -- add your button handler code here..
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
        //[/UserButtonCode_btnSettings]
    }
    else if (buttonThatWasClicked == btnHelp.get())
    {
        //[UserButtonCode_btnHelp] -- add your button handler code here..

        if(ModifierKeys::currentModifiers.isCommandDown() && ModifierKeys::currentModifiers.isCtrlDown() && ModifierKeys::currentModifiers.isAltDown() && ModifierKeys::currentModifiers.isShiftDown())
        {
            animatorDialogManager.show(pSources, mainProcessor.getAnimatorDataset(), this);
            return;
        }

        helpDialogManager.show(true, this);
        //[/UserButtonCode_btnHelp]
    }

    else if (buttonThatWasClicked == btnAnimator.get())
    {
        //[UserButtonCode_btnHelp] -- add your button handler code here..

        // CallOutBox zeigt ein neues TimelineWidgetMS
        timelineDialogManager.show(this, &mainProcessor, &pointSelection, mainProcessor.getAnimatorEngine());
        //[/UserButtonCode_btnHelp]
    }
    
    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


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

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AmbisonicEncoderAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public ChangeListener, public ActionListener"
                 constructorParams="AmbisonicEncoderAudioProcessor&amp; ownerProc"
                 variableInitialisers="AudioProcessorEditor(ownerProc), mainProcessor(ownerProc)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="radarComponent" id="5bf6bd31c23a4886" memberName="radarComponent"
                    virtualName="" explicitFocusOrder="0" pos="0 32 0M 32M" class="RadarComponent"
                    params="pSources, nullptr, &amp;pointSelection, mainProcessor.getRadarOptions()"/>
  <LABEL name="labelVersion" id="79dc1bc82b90b8df" memberName="labelVersion"
         virtualName="" explicitFocusOrder="0" pos="5Rr 4 111 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Version" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <IMAGEBUTTON name="btnSettings" id="e4880b2ef7a6a35e" memberName="btnSettings"
               virtualName="" explicitFocusOrder="0" pos="0 0 32 32" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="settings_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="settings_png" opacityOver="0.4000000059604645"
               colourOver="6eee1010" resourceDown="settings_png" opacityDown="1.0"
               colourDown="c0ee1010"/>
  <LABEL name="labelMessage" id="24478721f956d20c" memberName="labelMessage"
         virtualName="" explicitFocusOrder="0" pos="64 4 188M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.7"
         kerning="0.0" bold="0" italic="0" justification="36"/>
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
