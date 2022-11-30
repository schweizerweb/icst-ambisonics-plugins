/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "OSCLogComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCLogComponent::OSCLogComponent (StatusMessageHandler* pStatusHandler)
    : pStatusHandler(pStatusHandler)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textLog.reset (new juce::TextEditor ("textLog"));
    addAndMakeVisible (textLog.get());
    textLog->setMultiLine (true);
    textLog->setReturnKeyStartsNewLine (true);
    textLog->setReadOnly (false);
    textLog->setScrollbarsShown (true);
    textLog->setCaretVisible (true);
    textLog->setPopupMenuEnabled (true);
    textLog->setText (juce::String());

    toggleOn.reset (new juce::ToggleButton ("toggleOn"));
    addAndMakeVisible (toggleOn.get());
    toggleOn->setButtonText (TRANS("On"));
    toggleOn->addListener (this);
    toggleOn->setToggleState (true, juce::dontSendNotification);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    textLog->setTextToShowWhenEmpty("no incoming OSC", Colours::lightcoral);
    pStatusHandler->registerDetailLog(this);
    //[/Constructor]
}

OSCLogComponent::~OSCLogComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pStatusHandler->unregisterDetailLog(this);
    //[/Destructor_pre]

    textLog = nullptr;
    toggleOn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCLogComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCLogComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textLog->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
    toggleOn->setBounds (getWidth() - 8 - 56, 16, 56, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCLogComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleOn.get())
    {
        //[UserButtonCode_toggleOn] -- add your button handler code here..
        if(toggleOn->getToggleState())
            pStatusHandler->registerDetailLog(this);
        else
            pStatusHandler->unregisterDetailLog(this);
        //[/UserButtonCode_toggleOn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OSCLogComponent::notify(StatusMessage msg)
{
    textLog->setColour(TextEditor::textColourId, msg.messageStyle == StatusMessage::Error ? Colours::red : Colours::limegreen);

    textLog->moveCaretToEnd();
    String timeString = msg.timestamp.toString(false, true, true, true);
    textLog->insertTextAtCaret(timeString + "\t " + msg.message + NewLine::getDefault());

    // truncate periodically
    if(textLog->getText().length() > 20000)
    {
        textLog->setText(textLog->getText().getLastCharacters(10000));
    }
}

void OSCLogComponent::notifyOverflow(int discardedCount)
{
    textLog->setColour(TextEditor::textColourId, Colours::red);
    textLog->moveCaretToEnd();
    textLog->insertTextAtCaret("Too many messages to display, " + String(discardedCount) + " elements discarded");
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCLogComponent" componentName=""
                 parentClasses="public juce::Component, public StatusMessageReceiver"
                 constructorParams="StatusMessageHandler* pStatusHandler" variableInitialisers="pStatusHandler(pStatusHandler)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="textLog" id="177ad788fed7d0e3" memberName="textLog" virtualName=""
              explicitFocusOrder="0" pos="8 8 16M 16M" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="toggleOn" id="70d90ef4c7334d0b" memberName="toggleOn" virtualName=""
                explicitFocusOrder="0" pos="8Rr 16 56 24" buttonText="On" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

