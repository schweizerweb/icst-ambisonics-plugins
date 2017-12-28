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
#include "../../Common/RadarComponent.h"
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
    //[/Constructor_pre]

    addAndMakeVisible (buttonConfigure = new TextButton ("buttonConfigure"));
    buttonConfigure->setButtonText (TRANS("settings"));
    buttonConfigure->addListener (this);

    addAndMakeVisible (component = new RadarComponent (pSourcesArray, nullptr, &pointSelection, &radarOptions));
    component->setName ("new component");


    //[UserPreSize]
	setResizable(true, true);
    //[/UserPreSize]

    setSize (400, 700);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

AmbisonicEncoderAudioProcessorEditor::~AmbisonicEncoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    buttonConfigure = nullptr;
    component = nullptr;


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

    buttonConfigure->setBounds (0, 8, getWidth() - 0, 24);
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
              virtualName="" explicitFocusOrder="0" pos="0 8 0M 24" buttonText="settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="5bf6bd31c23a4886" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="0 40 0M 46M" class="RadarComponent"
                    params="pSourcesArray, nullptr, &amp;pointSelection, &amp;radarOptions"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
