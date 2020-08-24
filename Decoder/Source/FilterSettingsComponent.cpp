/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "FilterSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FilterSettingsComponent::FilterSettingsComponent (FilterInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* pPresetHelper)
    : pPresetHelper(pPresetHelper)
{
    //[Constructor_pre] You can add your own custom stuff here..
	this->pFilterInfo = pFilterInfo;

    //[/Constructor_pre]

    comboBoxType.reset (new ComboBox ("comboBoxType"));
    addAndMakeVisible (comboBoxType.get());
    comboBoxType->setEditableText (false);
    comboBoxType->setJustificationType (Justification::centredLeft);
    comboBoxType->setTextWhenNothingSelected (String());
    comboBoxType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxType->addListener (this);

    sliderFrequency.reset (new Slider ("sliderFrequency"));
    addAndMakeVisible (sliderFrequency.get());
    sliderFrequency->setRange (20, 15000, 1);
    sliderFrequency->setSliderStyle (Slider::LinearBar);
    sliderFrequency->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderFrequency->addListener (this);

    labelFrequency.reset (new Label ("labelFrequency",
                                     TRANS("Frequency [Hz]")));
    addAndMakeVisible (labelFrequency.get());
    labelFrequency->setTooltip (TRANS("Frequency [Hz] (Cut-off / Center)"));
    labelFrequency->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelFrequency->setJustificationType (Justification::centredLeft);
    labelFrequency->setEditable (false, false, false);
    labelFrequency->setColour (TextEditor::textColourId, Colours::black);
    labelFrequency->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelFrequency->setBounds (8, 40, 152, 24);

    labelFilterType.reset (new Label ("labelFilterType",
                                      TRANS("Filter Type")));
    addAndMakeVisible (labelFilterType.get());
    labelFilterType->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelFilterType->setJustificationType (Justification::centredLeft);
    labelFilterType->setEditable (false, false, false);
    labelFilterType->setColour (TextEditor::textColourId, Colours::black);
    labelFilterType->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelFilterType->setBounds (8, 8, 150, 24);

    labelQ.reset (new Label ("labelQ",
                             TRANS("Q-Value")));
    addAndMakeVisible (labelQ.get());
    labelQ->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelQ->setJustificationType (Justification::centredLeft);
    labelQ->setEditable (false, false, false);
    labelQ->setColour (TextEditor::textColourId, Colours::black);
    labelQ->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ->setBounds (8, 72, 150, 24);

    sliderQ.reset (new Slider ("sliderQ"));
    addAndMakeVisible (sliderQ.get());
    sliderQ->setRange (0.001, 100, 0);
    sliderQ->setSliderStyle (Slider::LinearBar);
    sliderQ->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderQ->addListener (this);

    filterGraph.reset (new IIRFilterGraph (pFilterInfo, pFilterSpecification));
    addAndMakeVisible (filterGraph.get());
    filterGraph->setName ("filterGraph");

    labelGain.reset (new Label ("labelGain",
                                TRANS("Gain Factor")));
    addAndMakeVisible (labelGain.get());
    labelGain->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelGain->setJustificationType (Justification::centredLeft);
    labelGain->setEditable (false, false, false);
    labelGain->setColour (TextEditor::textColourId, Colours::black);
    labelGain->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelGain->setBounds (8, 104, 150, 24);

    sliderGain.reset (new Slider ("sliderGain"));
    addAndMakeVisible (sliderGain.get());
    sliderGain->setRange (0.1, 10, 0.001);
    sliderGain->setSliderStyle (Slider::LinearBar);
    sliderGain->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderGain->addListener (this);

    comboBoxFilterPreset.reset (new ComboBox ("comboBoxFilterPreset"));
    addAndMakeVisible (comboBoxFilterPreset.get());
    comboBoxFilterPreset->setEditableText (false);
    comboBoxFilterPreset->setJustificationType (Justification::centredLeft);
    comboBoxFilterPreset->setTextWhenNothingSelected (TRANS("-"));
    comboBoxFilterPreset->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxFilterPreset->addListener (this);

    labelPresets.reset (new Label ("labelPresets",
                                   TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (TextEditor::textColourId, Colours::black);
    labelPresets->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    buttonSave.reset (new TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	addChangeListener(pChangeListener);
	comboBoxType->addItem("Off", 1+FilterInfo::FilterType::None);
	comboBoxType->addItem("Low-Pass", 1+FilterInfo::FilterType::LowPass);
	comboBoxType->addItem("Band-Pass", 1+FilterInfo::FilterType::BandPass);
	comboBoxType->addItem("High-Pass", 1+FilterInfo::FilterType::HighPass);
	comboBoxType->addItem("Low-Pass (1st order)", 1+FilterInfo::FilterType::FirstOrderLowPass);
	comboBoxType->addItem("High-Pass (1st order)", 1+FilterInfo::FilterType::FirstOrderHighPass);
	comboBoxType->addItem("Notch", 1+FilterInfo::FilterType::Notch);
    comboBoxType->addItem("Low Shelf", 1+FilterInfo::FilterType::LowShelf);
    comboBoxType->addItem("High Shelf", 1+FilterInfo::FilterType::HighShelf);
    comboBoxType->addItem("Peak", 1+FilterInfo::FilterType::Peak);


	sliderFrequency->setSkewFactorFromMidPoint(500);
	sliderFrequency->setRange(20, jmin(int(pFilterSpecification->sampleRate / 2.0), 22000));
	sliderFrequency->setNumDecimalPlacesToDisplay(0);
    sliderQ->setNumDecimalPlacesToDisplay(3);
	sliderQ->setSkewFactorFromMidPoint(1.0);
    sliderGain->setNumDecimalPlacesToDisplay(3);
    sliderGain->setSkewFactorFromMidPoint(1.0);

    updateUi();
    updatePresetComboBox();
    pPresetHelper->addActionListener(this);
    //[/Constructor]
}

FilterSettingsComponent::~FilterSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    comboBoxType = nullptr;
    sliderFrequency = nullptr;
    labelFrequency = nullptr;
    labelFilterType = nullptr;
    labelQ = nullptr;
    sliderQ = nullptr;
    filterGraph = nullptr;
    labelGain = nullptr;
    sliderGain = nullptr;
    comboBoxFilterPreset = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBoxType->setBounds (136, 8, getWidth() - 144, 24);
    sliderFrequency->setBounds (136, 40, getWidth() - 144, 24);
    sliderQ->setBounds (136, 72, getWidth() - 144, 24);
    filterGraph->setBounds (8, 136, getWidth() - 16, getHeight() - 175);
    sliderGain->setBounds (136, 104, getWidth() - 144, 24);
    comboBoxFilterPreset->setBounds (80, getHeight() - 32, getWidth() - 179, 24);
    labelPresets->setBounds (8, getHeight() - 32, 64, 24);
    buttonSave->setBounds (getWidth() - 11 - 80, getHeight() - 32, 80, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxType.get())
    {
        //[UserComboBoxCode_comboBoxType] -- add your combo box handling code here..
		pFilterInfo->filterType = FilterInfo::FilterType(comboBoxType->getSelectedId()-1);
        pFilterInfo->cutOffFrequencyHz = pFilterInfo->defaultFrequency();
        pFilterInfo->qValue = pFilterInfo->defaultQ();
        pFilterInfo->gainFactor = pFilterInfo->defaultGainFactor();
        updateUi();
        sendChangeMessage();
        //[/UserComboBoxCode_comboBoxType]
    }
    else if (comboBoxThatHasChanged == comboBoxFilterPreset.get())
    {
        //[UserComboBoxCode_comboBoxFilterPreset] -- add your combo box handling code here..
        String presetName = comboBoxFilterPreset->getText();
        pPresetHelper->selectPresetName(presetName);
        comboBoxFilterPreset->setSelectedItemIndex(-1);
        //[/UserComboBoxCode_comboBoxFilterPreset]
    }

    //[UsercomboBoxChanged_Post]
	filterGraph->repaint();
    //[/UsercomboBoxChanged_Post]
}

void FilterSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderFrequency.get())
    {
        //[UserSliderCode_sliderFrequency] -- add your slider handling code here..
		pFilterInfo->cutOffFrequencyHz = float(sliderFrequency->getValue());
        //[/UserSliderCode_sliderFrequency]
    }
    else if (sliderThatWasMoved == sliderQ.get())
    {
        //[UserSliderCode_sliderQ] -- add your slider handling code here..
		pFilterInfo->qValue = float(sliderQ->getValue());
        //[/UserSliderCode_sliderQ]
    }
    else if (sliderThatWasMoved == sliderGain.get())
    {
        //[UserSliderCode_sliderGain] -- add your slider handling code here..
        pFilterInfo->gainFactor = float(sliderGain->getValue());
        //[/UserSliderCode_sliderGain]
    }

    //[UsersliderValueChanged_Post]
	filterGraph->repaint();
    //[/UsersliderValueChanged_Post]
}

void FilterSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
                return;

        pPresetHelper->writeToXmlFile(*newFile, pFilterInfo);
        comboBoxFilterPreset->setText("", dontSendNotification);
        delete newFile;
        //[/UserButtonCode_buttonSave]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void FilterSettingsComponent::updatePresetComboBox()
{
    comboBoxFilterPreset->clear();
    int i = 1;
    for (File file : pPresetHelper->presetFiles)
    {
        comboBoxFilterPreset->addItem(file.getFileNameWithoutExtension(), i++);
    }
}

void FilterSettingsComponent::updateUi()
{
    comboBoxType->setSelectedId(1+pFilterInfo->filterType, dontSendNotification);
    sliderFrequency->setValue(pFilterInfo->cutOffFrequencyHz);
    sliderQ->setValue(pFilterInfo->qValue);
    sliderGain->setValue(pFilterInfo->gainFactor);
    
    sliderFrequency->setEnabled(pFilterInfo->frequencyRequired());
    sliderQ->setEnabled(pFilterInfo->qRequired());
    sliderGain->setEnabled(pFilterInfo->gainRequired());
}

void FilterSettingsComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        updatePresetComboBox();
    }
    else if(message.startsWith(ACTION_MESSAGE_SELECT_PRESET))
    {
        File presetFile(message.substring(String(ACTION_MESSAGE_SELECT_PRESET).length()));
        pPresetHelper->loadFromXmlFile(presetFile, pFilterInfo);
        pPresetHelper->notifyPresetChanged();
        updateUi();
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterSettingsComponent"
                 componentName="" parentClasses="public Component, public ChangeBroadcaster, public ActionListener"
                 constructorParams="FilterInfo* pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* pPresetHelper"
                 variableInitialisers="pPresetHelper(pPresetHelper)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <COMBOBOX name="comboBoxType" id="8896d80d8503e534" memberName="comboBoxType"
            virtualName="" explicitFocusOrder="0" pos="136 8 144M 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sliderFrequency" id="f23e9b966efc9df5" memberName="sliderFrequency"
          virtualName="" explicitFocusOrder="0" pos="136 40 144M 24" min="20.0"
          max="15000.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelFrequency" id="2b4b513621db1c33" memberName="labelFrequency"
         virtualName="" explicitFocusOrder="0" pos="8 40 152 24" tooltip="Frequency [Hz] (Cut-off / Center)"
         edTextCol="ff000000" edBkgCol="0" labelText="Frequency [Hz]"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="labelFilterType" id="45be7377944657a2" memberName="labelFilterType"
         virtualName="" explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Filter Type" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelQ" id="c59d0be17b74b579" memberName="labelQ" virtualName=""
         explicitFocusOrder="0" pos="8 72 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Q-Value" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderQ" id="d75dd45147568a5" memberName="sliderQ" virtualName=""
          explicitFocusOrder="0" pos="136 72 144M 24" min="0.001" max="100.0"
          int="0.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="filterGraph" id="d097d04040748d3c" memberName="filterGraph"
                    virtualName="" explicitFocusOrder="0" pos="8 136 16M 175M" class="IIRFilterGraph"
                    params="pFilterInfo, pFilterSpecification"/>
  <LABEL name="labelGain" id="5e0ece6555401094" memberName="labelGain"
         virtualName="" explicitFocusOrder="0" pos="8 104 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Gain Factor" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderGain" id="6dc6c3b4f1230227" memberName="sliderGain"
          virtualName="" explicitFocusOrder="0" pos="136 104 144M 24" min="0.1"
          max="10.0" int="0.001" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="comboBoxFilterPreset" id="4b25adf5b07e9492" memberName="comboBoxFilterPreset"
            virtualName="" explicitFocusOrder="0" pos="80 32R 179M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="8 32R 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="11Rr 32R 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

