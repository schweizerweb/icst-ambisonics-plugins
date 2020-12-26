/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../../Common/FilterBankInfo.h"
//[/Headers]

#include "FilterSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FilterSettingsComponent::FilterSettingsComponent (FilterBankInfo* pFilterBankInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* pPresetHelper)
    : pPresetHelper(pPresetHelper), pFilterBankInfo(pFilterBankInfo)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    filterGraph.reset (new IIRFilterGraph (pFilterBankInfo, pFilterSpecification));
    addAndMakeVisible (filterGraph.get());
    filterGraph->setName ("filterGraph");

    comboBoxFilterPreset.reset (new juce::ComboBox ("comboBoxFilterPreset"));
    addAndMakeVisible (comboBoxFilterPreset.get());
    comboBoxFilterPreset->setEditableText (false);
    comboBoxFilterPreset->setJustificationType (juce::Justification::centredLeft);
    comboBoxFilterPreset->setTextWhenNothingSelected (TRANS("-"));
    comboBoxFilterPreset->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxFilterPreset->addListener (this);

    labelPresets.reset (new juce::Label ("labelPresets",
                                         TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (juce::Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPresets->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPresets->setBounds (0, 8, 64, 24);

    buttonSave.reset (new juce::TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    filter0.reset (new SingleFilterSettingsComponent (pFilterBankInfo->get(0), pFilterSpecification, this));
    addAndMakeVisible (filter0.get());
    filter0->setName ("filter0");

    filter1.reset (new SingleFilterSettingsComponent (pFilterBankInfo->get(1), pFilterSpecification, this));
    addAndMakeVisible (filter1.get());
    filter1->setName ("filter1");

    filter2.reset (new SingleFilterSettingsComponent (pFilterBankInfo->get(2), pFilterSpecification, this));
    addAndMakeVisible (filter2.get());
    filter2->setName ("filter2");

    filter3.reset (new SingleFilterSettingsComponent (pFilterBankInfo->get(3), pFilterSpecification, this));
    addAndMakeVisible (filter3.get());
    filter3->setName ("filter3");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 550);


    //[Constructor] You can add your own custom stuff here..
	addChangeListener(pChangeListener);

    updatePresetComboBox();
    pPresetHelper->addActionListener(this);
    //[/Constructor]
}

FilterSettingsComponent::~FilterSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    filterGraph = nullptr;
    comboBoxFilterPreset = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    filter0 = nullptr;
    filter1 = nullptr;
    filter2 = nullptr;
    filter3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    filterGraph->setBounds (0, 40, getWidth() - 0, getHeight() - 240);
    comboBoxFilterPreset->setBounds (72, 8, getWidth() - 167, 24);
    buttonSave->setBounds (getWidth() - 7 - 80, 8, 80, 24);
    filter0->setBounds (0, getHeight() - 200, proportionOfWidth (0.2491f), 200);
    filter1->setBounds (proportionOfWidth (0.2491f), getHeight() - 200, proportionOfWidth (0.2491f), 200);
    filter2->setBounds (proportionOfWidth (0.4983f), getHeight() - 200, proportionOfWidth (0.2491f), 200);
    filter3->setBounds (proportionOfWidth (0.7509f), getHeight() - 200, proportionOfWidth (0.2491f), 200);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxFilterPreset.get())
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

void FilterSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
                return;

        pPresetHelper->writeToXmlFile(*newFile, pFilterBankInfo);
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

void FilterSettingsComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        updatePresetComboBox();
    }
    else if(message.startsWith(ACTION_MESSAGE_SELECT_PRESET))
    {
        File presetFile(message.substring(String(ACTION_MESSAGE_SELECT_PRESET).length()));
        pPresetHelper->loadFromXmlFile(presetFile, pFilterBankInfo);
        pPresetHelper->notifyPresetChanged();
        filter0->updateUi();
        filter1->updateUi();
        filter2->updateUi();
        filter3->updateUi();
    }
}

void FilterSettingsComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    filterGraph->repaint();
    sendChangeMessage();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterSettingsComponent"
                 componentName="" parentClasses="public Component, public ChangeBroadcaster, public ActionListener, public ChangeListener"
                 constructorParams="FilterBankInfo* pFilterBankInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* pPresetHelper"
                 variableInitialisers="pPresetHelper(pPresetHelper), pFilterBankInfo(pFilterBankInfo)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="550">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="filterGraph" id="d097d04040748d3c" memberName="filterGraph"
                    virtualName="" explicitFocusOrder="0" pos="0 40 0M 240M" class="IIRFilterGraph"
                    params="pFilterBankInfo, pFilterSpecification"/>
  <COMBOBOX name="comboBoxFilterPreset" id="4b25adf5b07e9492" memberName="comboBoxFilterPreset"
            virtualName="" explicitFocusOrder="0" pos="72 8 167M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="0 8 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="7Rr 8 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="filter0" id="49c5ab20d38acf14" memberName="filter0" virtualName=""
                    explicitFocusOrder="0" pos="0 200R 24.913% 200" class="SingleFilterSettingsComponent"
                    params="pFilterBankInfo-&gt;get(0), pFilterSpecification, this"/>
  <GENERICCOMPONENT name="filter1" id="1bbdd0b2e36d35d6" memberName="filter1" virtualName=""
                    explicitFocusOrder="0" pos="24.913% 200R 24.913% 200" class="SingleFilterSettingsComponent"
                    params="pFilterBankInfo-&gt;get(1), pFilterSpecification, this"/>
  <GENERICCOMPONENT name="filter2" id="fd705b145567f812" memberName="filter2" virtualName=""
                    explicitFocusOrder="0" pos="49.827% 200R 24.913% 200" class="SingleFilterSettingsComponent"
                    params="pFilterBankInfo-&gt;get(2), pFilterSpecification, this"/>
  <GENERICCOMPONENT name="filter3" id="d7de5a2154c80bcf" memberName="filter3" virtualName=""
                    explicitFocusOrder="0" pos="75.087% 200R 24.913% 200" class="SingleFilterSettingsComponent"
                    params="pFilterBankInfo-&gt;get(3), pFilterSpecification, this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

