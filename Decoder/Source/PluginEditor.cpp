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
	pSpeakerArray = ownerProc.getSpeakerArray();
	pMovingPointsArray = ownerProc.getMovingPointsArray();
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

    addAndMakeVisible (component = new RadarComponent (pSpeakerArray, pMovingPointsArray));
    component->setName ("new component");

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Presets:")));
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (buttonLoad = new TextButton ("buttonLoad"));
    buttonLoad->setButtonText (TRANS("load"));
    buttonLoad->addListener (this);

    addAndMakeVisible (buttonSave = new TextButton ("buttonSave"));
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);


    //[UserPreSize]
	setResizable(true, true);
    //[/UserPreSize]

    setSize (400, 700);


    //[Constructor] You can add your own custom stuff here..
	startTimer(200);//starts timer with interval of 200mS
	updateComboBox();
    //[/Constructor]
}

AmbisonicsDecoderAudioProcessorEditor::~AmbisonicsDecoderAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBoxChannelConfig = nullptr;
    component = nullptr;
    label = nullptr;
    buttonLoad = nullptr;
    buttonSave = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AmbisonicsDecoderAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmbisonicsDecoderAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBoxChannelConfig->setBounds (88, 8, 192, 24);
    component->setBounds (0, 40, getWidth() - 0, getHeight() - 40);
    label->setBounds (8, 8, 112, 24);
    buttonLoad->setBounds (288, 8, 40, 24);
    buttonSave->setBounds (336, 8, 40, 24);
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
		String presetName = comboBoxChannelConfig->getText();
    	PresetInfo* namedPreset = nullptr;
    	for(PresetInfo* info : presets)
		{
			if(info->getName() == presetName)
			{
				namedPreset = info;
				break;
			}
		}

		if(namedPreset != nullptr)
		{
			loadPreset(namedPreset);
		}
		else
		{
			int nbChannels = comboBoxChannelConfig->getText().getIntValue();

			pSpeakerArray->clear();
			if (nbChannels == 2)
			{
				pSpeakerArray->add(AmbiPoint(Point3D<double>(0.7, -0.7, 0.0), "L", 0));
				pSpeakerArray->add(AmbiPoint(Point3D<double>(0.7, 0.7, 0.0), "R", 0));
			}
			else
			{
				Point<float> projectedPoint(1.0, 0.0);
				projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI / nbChannels));
				for (int i = 0; i < nbChannels; i++)
				{
					pSpeakerArray->add(AmbiPoint(Point3D<double>(projectedPoint.getX(), projectedPoint.getY(), 0.0), String(i), 0));
					projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI * 2 / nbChannels));
				}
			}
		}
        //[/UserComboBoxCode_comboBoxChannelConfig]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void AmbisonicsDecoderAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonLoad)
    {
        //[UserButtonCode_buttonLoad] -- add your button handler code here..
		ScopedPointer<FileChooser> fileChooser = new FileChooser("Load Preset", File(), "*.xml");
		if(fileChooser->browseForFileToOpen())
		{
			PresetInfo* preset = new PresetInfo();
			if (preset->LoadFromFile(fileChooser->getResult()))
			{
				loadPreset(preset);
				presets.add(preset);
				updateComboBox(preset->getName());
			}
		}
        //[/UserButtonCode_buttonLoad]
    }
    else if (buttonThatWasClicked == buttonSave)
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
		ScopedPointer<FileChooser> fileChooser = new FileChooser("Save Preset", File(), "*.xml");
		if (fileChooser->browseForFileToSave(true))
		{
			PresetInfo* preset = new PresetInfo();
			preset->setName(fileChooser->getResult().getFileNameWithoutExtension());
			for (AmbiPoint pt : *pSpeakerArray)
				preset->getPoints()->add(new AmbiPoint(pt));

			preset->SaveToFile(fileChooser->getResult());

			presets.add(preset);
			updateComboBox(preset->getName());
		}
        //[/UserButtonCode_buttonSave]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AmbisonicsDecoderAudioProcessorEditor::timerCallback()
{
	//if you want any display updates with a refresh timer add them here
}
void AmbisonicsDecoderAudioProcessorEditor::loadPreset(PresetInfo* preset) const
{
	pSpeakerArray->clear();
	for(AmbiPoint* pt : *preset->getPoints())
	{
		pSpeakerArray->add(AmbiPoint(*pt));
	}
}

void AmbisonicsDecoderAudioProcessorEditor::updateComboBox(String elementToSelect)
{
	comboBoxChannelConfig->clear();
	int i = 1;
	for (PresetInfo* preset : presets)
	{
		comboBoxChannelConfig->addItem(preset->getName(), i++);
	}
	comboBoxChannelConfig->addItem("2", i++);
	comboBoxChannelConfig->addItem("4", i++);
	comboBoxChannelConfig->addItem("6", i++);
	comboBoxChannelConfig->addItem("8", i++);
	comboBoxChannelConfig->addItem("16", i++);
	comboBoxChannelConfig->addItem("32", i++);

	for(i = 0; i < comboBoxChannelConfig->getNumItems(); i++)
	{
		if(comboBoxChannelConfig->getItemText(i) == elementToSelect)
		{
			comboBoxChannelConfig->setSelectedItemIndex(i);
			break;
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
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="AmbisonicsDecoderAudioProcessor&amp; ownerProc"
                 variableInitialisers="AudioProcessorEditor(ownerProc), processor(ownerProc)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="700">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="channelConfig" id="4b25adf5b07e9492" memberName="comboBoxChannelConfig"
            virtualName="" explicitFocusOrder="0" pos="88 8 192 24" editable="0"
            layout="33" items="2&#10;4&#10;6&#10;8" textWhenNonSelected="-"
            textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="new component" id="cb26712c5c52dede" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="0 40 0M 40M" class="RadarComponent"
                    params="pSpeakerArray, pMovingPointsArray"/>
  <LABEL name="new label" id="107b43efebb2a5c8" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Presets:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonLoad" id="5a786eb91323df32" memberName="buttonLoad"
              virtualName="" explicitFocusOrder="0" pos="288 8 40 24" buttonText="load"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="336 8 40 24" buttonText="save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
