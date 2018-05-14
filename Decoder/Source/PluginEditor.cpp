/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SpeakerSettingsComponent.h"
#include "DecoderSettingsComponent.h"
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AmbisonicsDecoderAudioProcessorEditor::AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), processor(ownerProc)
{
    //[Constructor_pre] You can add your own custom stuff here..
	radarOptions.nameFieldEditable = false;
	radarOptions.maxNumberEditablePoints = JucePlugin_MaxNumOutputChannels;
	radarOptions.editablePointsAsSquare = true;
	pSpeakerArray = ownerProc.getSpeakerArray();
	pMovingPointsArray = ownerProc.getMovingPointsArray();
	pAmbiSettings = ownerProc.getAmbiSettings();
	pDecoderSettings = ownerProc.getDecoderSettings();
	oscHandler = new OSCHandler(pMovingPointsArray);
	initializeOscHandler();
    //[/Constructor_pre]

    addAndMakeVisible (component = new RadarComponent (pSpeakerArray, pMovingPointsArray, &pointSelection, &radarOptions));
    component->setName ("new component");

    addAndMakeVisible (buttonConfigure = new TextButton ("buttonConfigure"));
    buttonConfigure->setButtonText (TRANS("decoder settings"));
    buttonConfigure->addListener (this);

    buttonConfigure->setBounds (8, 8, 128, 24);

    addAndMakeVisible (buttonConfigurePlugin = new TextButton ("buttonConfigurePlugin"));
    buttonConfigurePlugin->setButtonText (TRANS("plugin settings"));
    buttonConfigurePlugin->addListener (this);

    buttonConfigurePlugin->setBounds (152, 8, 128, 24);

    addAndMakeVisible (labelVersion = new Label ("labelVersion",
                                                 TRANS("Version")));
    labelVersion->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelVersion->setJustificationType (Justification::centredRight);
    labelVersion->setEditable (false, false, false);
    labelVersion->setColour (TextEditor::textColourId, Colours::black);
    labelVersion->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelVersion->setBounds (288, 8, 62, 24);


    //[UserPreSize]
	setResizable(true, true);
    //[/UserPreSize]

    setSize (400, 700);


    //[Constructor] You can add your own custom stuff here..
	setSize(pDecoderSettings->lastUIWidth, pDecoderSettings->lastUIHeight);
	labelVersion->setText("V. " + String(ProjectInfo::versionString), dontSendNotification);
	//[/Constructor]
}

AmbisonicsDecoderAudioProcessorEditor::~AmbisonicsDecoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    buttonConfigure = nullptr;
    buttonConfigurePlugin = nullptr;
    labelVersion = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AmbisonicsDecoderAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmbisonicsDecoderAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    component->setBounds (0, 40, getWidth() - 0, getHeight() - 40);
    //[UserResized] Add your own custom resize handling here..
	pDecoderSettings->lastUIWidth = getWidth();
	pDecoderSettings->lastUIHeight = getHeight();
    //[/UserResized]
}

void AmbisonicsDecoderAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonConfigure)
    {
        //[UserButtonCode_buttonConfigure] -- add your button handler code here..
		SpeakerSettingsComponent::showAsDialog(pSpeakerArray, &presets, &pointSelection, pAmbiSettings, processor.getTestSoundGenerator());
        //[/UserButtonCode_buttonConfigure]
    }
    else if (buttonThatWasClicked == buttonConfigurePlugin)
    {
        //[UserButtonCode_buttonConfigurePlugin] -- add your button handler code here..
		DecoderSettingsComponent::showAsDialog(pDecoderSettings);
		initializeOscHandler();
        //[/UserButtonCode_buttonConfigurePlugin]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AmbisonicsDecoderAudioProcessorEditor::initializeOscHandler()
{
	// update timeout
	radarOptions.displayTimeout = pDecoderSettings->oscReceiveTimeoutMs;

	oscHandler->stop();

	if (pDecoderSettings->oscReceive)
	{
		if (!oscHandler->start(pDecoderSettings->oscReceivePort))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC-Handler on port " + String(pDecoderSettings->oscReceivePort), "OK");
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

<JUCER_COMPONENT documentType="Component" className="AmbisonicsDecoderAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor"
                 constructorParams="AmbisonicsDecoderAudioProcessor&amp; ownerProc"
                 variableInitialisers="AudioProcessorEditor(ownerProc), processor(ownerProc)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="new component" id="cb26712c5c52dede" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="0 40 0M 40M" class="RadarComponent"
                    params="pSpeakerArray, pMovingPointsArray, &amp;pointSelection, &amp;radarOptions"/>
  <TEXTBUTTON name="buttonConfigure" id="9d167617277afe11" memberName="buttonConfigure"
              virtualName="" explicitFocusOrder="0" pos="8 8 128 24" buttonText="decoder settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonConfigurePlugin" id="265eef40909bd6dd" memberName="buttonConfigurePlugin"
              virtualName="" explicitFocusOrder="0" pos="152 8 128 24" buttonText="plugin settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelVersion" id="79dc1bc82b90b8df" memberName="labelVersion"
         virtualName="" explicitFocusOrder="0" pos="288 8 62 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Version" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="34"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
