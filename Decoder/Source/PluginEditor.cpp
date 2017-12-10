/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SpeakerSettingsComponent.h"
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AmbisonicsDecoderAudioProcessorEditor::AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), processor(ownerProc)
{
    //[Constructor_pre] You can add your own custom stuff here..
	pSpeakerArray = ownerProc.getSpeakerArray();
	pMovingPointsArray = ownerProc.getMovingPointsArray();
	pAmbiSettings = ownerProc.getAmbiSettings();
	oscHandler = new OSCHandler(pMovingPointsArray);
	if(!oscHandler->start(5011))
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, "OSC", "Error starting OSC-Handler!", "OK");
	}
    //[/Constructor_pre]

    addAndMakeVisible (component = new RadarComponent (pSpeakerArray, pMovingPointsArray, &pointSelection));
    component->setName ("new component");

    addAndMakeVisible (buttonConfigure = new TextButton ("buttonConfigure"));
    buttonConfigure->setButtonText (TRANS("configure"));
    buttonConfigure->addListener (this);


    //[UserPreSize]
	setResizable(true, true);
    //[/UserPreSize]

    setSize (400, 700);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

AmbisonicsDecoderAudioProcessorEditor::~AmbisonicsDecoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    buttonConfigure = nullptr;


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
    buttonConfigure->setBounds (8, 8, getWidth() - 12, 24);
    //[UserResized] Add your own custom resize handling here..
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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
                    params="pSpeakerArray, pMovingPointsArray, &amp;pointSelection"/>
  <TEXTBUTTON name="buttonConfigure" id="9d167617277afe11" memberName="buttonConfigure"
              virtualName="" explicitFocusOrder="0" pos="8 8 12M 24" buttonText="configure"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
