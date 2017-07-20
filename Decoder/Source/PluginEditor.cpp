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

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AmbisonicsDecoderAudioProcessorEditor::AmbisonicsDecoderAudioProcessorEditor (AmbisonicsDecoderAudioProcessor& ownerProc)
    : AudioProcessorEditor(ownerProc), processor(ownerProc)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (comboBoxChannelConfig = new ComboBox ("channelConfig"));
    comboBoxChannelConfig->setEditableText (false);
    comboBoxChannelConfig->setJustificationType (Justification::centredLeft);
    comboBoxChannelConfig->setTextWhenNothingSelected (TRANS("-"));
    comboBoxChannelConfig->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxChannelConfig->addItem (TRANS("2"), 1);
    comboBoxChannelConfig->addItem (TRANS("4"), 2);
    comboBoxChannelConfig->addItem (TRANS("6"), 3);
    comboBoxChannelConfig->addItem (TRANS("8"), 4);
    comboBoxChannelConfig->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	startTimer(200);//starts timer with interval of 200mS
    //[/Constructor]
}

AmbisonicsDecoderAudioProcessorEditor::~AmbisonicsDecoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBoxChannelConfig = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AmbisonicsDecoderAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff6c9bb2));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmbisonicsDecoderAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBoxChannelConfig->setBounds (8, 9, 192, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AmbisonicsDecoderAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxChannelConfig)
    {
        //[UserComboBoxCode_comboBoxChannelConfig] -- add your combo box handling code here..
		int channelNb = atoi(comboBoxChannelConfig->getItemText(comboBoxChannelConfig->getSelectedItemIndex()).toRawUTF8());
		AudioChannelSet::ChannelType type = AudioChannelSet::unknown;
    	AudioChannelSet layout = processor.getChannelLayoutOfBus(false, 0);
		for (int i = 0; i < channelNb; i++)
			layout.addChannel(type);

		
    	int cnt = processor.getBusCount(false);
		int chanCnt = processor.getChannelCountOfBus(false, 0);
		bool ok = processor.setChannelLayoutOfBus(false, 0, layout);
		if(!ok)
		{
			return;
		}
		int chanCntNew = processor.getChannelCountOfBus(false, 0);

		bool canAdd = processor.canAddBus(false);
		if(canAdd)
		{
			processor.addBus(false);
			processor.enableAllBuses();
		}
		//[/UserComboBoxCode_comboBoxChannelConfig]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AmbisonicsDecoderAudioProcessorEditor::timerCallback()
{
	//if you want any display updates with a refresh timer add them here
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AmbisonicsDecoderAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="AmbisonicsDecoderAudioProcessor&amp; ownerProc"
                 variableInitialisers="AudioProcessorEditor(ownerProc), processor(ownerProc)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff6c9bb2"/>
  <COMBOBOX name="channelConfig" id="4b25adf5b07e9492" memberName="comboBoxChannelConfig"
            virtualName="" explicitFocusOrder="0" pos="8 9 192 24" editable="0"
            layout="33" items="2&#10;4&#10;6&#10;8" textWhenNonSelected="-"
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
