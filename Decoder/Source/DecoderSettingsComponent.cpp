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
//[/Headers]

#include "DecoderSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DecoderSettingsComponent::DecoderSettingsComponent (DecoderSettings* pSettings)
    : pDecoderSettings(pSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (groupOsc = new GroupComponent ("groupOsc",
                                                      TRANS("OSC")));

    addAndMakeVisible (toggleReceiveOsc = new ToggleButton ("toggleReceiveOsc"));
    toggleReceiveOsc->setButtonText (TRANS("Enable OSC Receiver"));
    toggleReceiveOsc->addListener (this);

    addAndMakeVisible (textOscPort = new TextEditor ("textOscPort"));
    textOscPort->setMultiLine (false);
    textOscPort->setReturnKeyStartsNewLine (false);
    textOscPort->setReadOnly (false);
    textOscPort->setScrollbarsShown (true);
    textOscPort->setCaretVisible (true);
    textOscPort->setPopupMenuEnabled (true);
    textOscPort->setText (String());

    addAndMakeVisible (labelOscPort = new Label ("labelOscPort",
                                                 TRANS("OSC-Port:\n")));
    labelOscPort->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelOscPort->setJustificationType (Justification::centredLeft);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (TextEditor::textColourId, Colours::black);
    labelOscPort->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 200);


    //[Constructor] You can add your own custom stuff here..
	toggleReceiveOsc->setToggleState(pDecoderSettings->oscReceive, dontSendNotification);
	textOscPort->setText(String(pDecoderSettings->oscReceivePort));
    //[/Constructor]
}

DecoderSettingsComponent::~DecoderSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupOsc = nullptr;
    toggleReceiveOsc = nullptr;
    textOscPort = nullptr;
    labelOscPort = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DecoderSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DecoderSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupOsc->setBounds (8, 8, getWidth() - 8, 64);
    toggleReceiveOsc->setBounds (24, 32, 150, 24);
    textOscPort->setBounds (8 + (getWidth() - 8) - 20 - 142, 8 + 24, 142, 24);
    labelOscPort->setBounds (8 + (getWidth() - 8) - 170 - 80, 32, 80, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DecoderSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleReceiveOsc)
    {
        //[UserButtonCode_toggleReceiveOsc] -- add your button handler code here..
		pDecoderSettings->oscReceive = toggleReceiveOsc->getToggleState();
        //[/UserButtonCode_toggleReceiveOsc]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DecoderSettingsComponent::showAsDialog(DecoderSettings* pSettings)
{
	DecoderSettingsComponent *p = new DecoderSettingsComponent(pSettings);
	p->setSize(400, 200);

	DialogWindow::LaunchOptions options;
	options.content.setOwned(p);
	options.dialogTitle = "Decoder-Plugin settings";
	options.dialogBackgroundColour = Colours::white;
	options.escapeKeyTriggersCloseButton = false;
	options.useNativeTitleBar = false;
	options.resizable = true;
	options.launchAsync();
}

void DecoderSettingsComponent::textEditorTextChanged(TextEditor& editor)
{
	if (textOscPort->getText().containsOnly("0123456789"))
	{
		pDecoderSettings->oscReceivePort = textOscPort->getText().getIntValue();
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid port number");
		editor.undo();
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DecoderSettingsComponent"
                 componentName="" parentClasses="public Component, public TextEditorListener"
                 constructorParams="DecoderSettings* pSettings" variableInitialisers="pDecoderSettings(pSettings)&#10;"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="200">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupOsc" id="f4cf3a53a6ef0d87" memberName="groupOsc" virtualName=""
                  explicitFocusOrder="0" pos="8 8 8M 64" title="OSC"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="24 32 150 24" buttonText="Enable OSC Receiver"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="textOscPort" id="ef3c7d2594795ec7" memberName="textOscPort"
              virtualName="" explicitFocusOrder="0" pos="20Rr 24 142 24" posRelativeX="f4cf3a53a6ef0d87"
              posRelativeY="f4cf3a53a6ef0d87" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="170Rr 32 80 24" posRelativeX="f4cf3a53a6ef0d87"
         edTextCol="ff000000" edBkgCol="0" labelText="OSC-Port:&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
