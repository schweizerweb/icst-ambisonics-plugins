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
#include "../../Common/RadarComponent.h"
#include "EncoderSettingsComponent.h"
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AmbisonicEncoderAudioProcessorEditor::AmbisonicEncoderAudioProcessorEditor (AmbisonicEncoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), processor(ownerProc)
{
    //[Constructor_pre] You can add your own custom stuff here..
	pSourcesArray = ownerProc.getSourcesArray();
	pEncoderSettings = ownerProc.getEncoderSettings();
	radarOptions.nameFieldEditable = true;
	radarOptions.maxNumberEditablePoints = JucePlugin_MaxNumInputChannels;
	radarOptions.audioParams = ownerProc.getAudioParams();
    //[/Constructor_pre]

    addAndMakeVisible (buttonConfigure = new TextButton ("buttonConfigure"));
    buttonConfigure->setButtonText (TRANS("settings"));
    buttonConfigure->addListener (this);

    buttonConfigure->setBounds (0, 8, 280, 24);

    addAndMakeVisible (component = new RadarComponent (pSourcesArray, nullptr, &pointSelection, &radarOptions));
    component->setName ("new component");

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
	labelVersion->setText("V. " + String(ProjectInfo::versionString), dontSendNotification);
    //[/Constructor]
}

AmbisonicEncoderAudioProcessorEditor::~AmbisonicEncoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    buttonConfigure = nullptr;
    component = nullptr;
    labelVersion = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AmbisonicEncoderAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmbisonicEncoderAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    component->setBounds (0, 40, getWidth() - 0, getHeight() - 46);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AmbisonicEncoderAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonConfigure)
    {
        //[UserButtonCode_buttonConfigure] -- add your button handler code here..
		EncoderSettingsComponent::showAsDialog(pEncoderSettings);
		processor.initializeOsc();
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

<JUCER_COMPONENT documentType="Component" className="AmbisonicEncoderAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor"
                 constructorParams="AmbisonicEncoderAudioProcessor&amp; ownerProc"
                 variableInitialisers="AudioProcessorEditor(ownerProc), processor(ownerProc)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="700">
  <BACKGROUND backgroundColour="ff505050"/>
  <TEXTBUTTON name="buttonConfigure" id="3fc456e2bfcc8706" memberName="buttonConfigure"
              virtualName="" explicitFocusOrder="0" pos="0 8 280 24" buttonText="settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="5bf6bd31c23a4886" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="0 40 0M 46M" class="RadarComponent"
                    params="pSourcesArray, nullptr, &amp;pointSelection, &amp;radarOptions"/>
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
