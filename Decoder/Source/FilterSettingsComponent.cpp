/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



//[Headers] You can add your own extra header files here...
#include "../../Common/FilterBankInfo.h"
#include "../../Common/FFTAnalyzer.h"
//[/Headers]

#include "FilterSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FilterSettingsComponent::FilterSettingsComponent (FilterBankInfo* _pFilterBankInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* _pPresetHelper, int _channelIndex)
    : pPresetHelper(_pPresetHelper), pFilterBankInfo(_pFilterBankInfo), channelIndex(_channelIndex)
{
    //[Constructor_pre] You can add your own custom stuff here..
    std::vector<FilterBankInfo*> filterInfo;
    filterInfo.push_back(pFilterBankInfo);
    //[/Constructor_pre]

    filterGraph.reset (new IIRFilterGraph (filterInfo, pFilterSpecification));
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
    labelPresets->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
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

    toggleBypass.reset (new juce::ToggleButton ("toggleBypass"));
    addAndMakeVisible (toggleBypass.get());
    toggleBypass->setButtonText (TRANS("Bypass filter"));
    toggleBypass->addListener (this);

    toggleFFT.reset (new juce::ToggleButton ("toggleFFT"));
    addAndMakeVisible (toggleFFT.get());
    toggleFFT->setButtonText (TRANS("FFT"));
    toggleFFT->addListener (this);

    sliderFFTScaler.reset (new juce::Slider ("sliderFFTScaler"));
    addAndMakeVisible (sliderFFTScaler.get());
    sliderFFTScaler->setRange (0, 50, 1);
    sliderFFTScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderFFTScaler->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 30, 20);
    sliderFFTScaler->addListener (this);

    labelFFTScaler.reset (new juce::Label ("labelFFTScaler",
                                           TRANS("Scaler [dB]:")));
    addAndMakeVisible (labelFFTScaler.get());
    labelFFTScaler->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
    labelFFTScaler->setJustificationType (juce::Justification::centredLeft);
    labelFFTScaler->setEditable (false, false, false);
    labelFFTScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelFFTScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (700, 550);


    //[Constructor] You can add your own custom stuff here..
    labelFFTScaler->setVisible(false);
    sliderFFTScaler->setVisible(false);

	addChangeListener(pChangeListener);

    updatePresetComboBox();
    if (pPresetHelper != nullptr)
    {
        pPresetHelper->addActionListener(this);
    }

    sliderFFTScaler->setValue(INITIAL_FFT_SCALER);
    toggleBypass->setToggleState(pFilterBankInfo->getFilterBypass(), dontSendNotification);
    //[/Constructor]
}

FilterSettingsComponent::~FilterSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    if (pPresetHelper != nullptr)
    {
        pPresetHelper->removeActionListener(this);
    }

    stopTimer();
    FFTAnalyzer::getInstance()->disable();
    //[/Destructor_pre]

    filterGraph = nullptr;
    comboBoxFilterPreset = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    filter0 = nullptr;
    filter1 = nullptr;
    filter2 = nullptr;
    filter3 = nullptr;
    toggleBypass = nullptr;
    toggleFFT = nullptr;
    sliderFFTScaler = nullptr;
    labelFFTScaler = nullptr;


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
    toggleBypass->setBounds (((getWidth() - 170) + 0 - 85) + 0 - 58 - 128, 42, 128, 24);
    toggleFFT->setBounds (((getWidth() - 170) + 0 - 85) + 0 - 58, 42, 58, 24);
    sliderFFTScaler->setBounds (getWidth() - 170, 42, 170, 24);
    labelFFTScaler->setBounds ((getWidth() - 170) + 0 - 85, 42, 85, 24);
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
    else if (buttonThatWasClicked == toggleBypass.get())
    {
        //[UserButtonCode_toggleBypass] -- add your button handler code here..
        pFilterBankInfo->setFilterBypass(toggleBypass->getToggleState());
        sendChangeMessage();
        //[/UserButtonCode_toggleBypass]
    }
    else if (buttonThatWasClicked == toggleFFT.get())
    {
        //[UserButtonCode_toggleFFT] -- add your button handler code here..
        if(toggleFFT->getToggleState())
        {
            filterGraph->setFFTParams(true, sliderFFTScaler->getValue());
            FFTAnalyzer::getInstance()->setActive(channelIndex);
            startTimer(50);
        }
        else
        {
            stopTimer();
            FFTAnalyzer::getInstance()->disable();
            filterGraph->setFFTParams(false);
        }

        labelFFTScaler->setVisible(toggleFFT->getToggleState());
        sliderFFTScaler->setVisible(toggleFFT->getToggleState());
        //[/UserButtonCode_toggleFFT]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void FilterSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderFFTScaler.get())
    {
        //[UserSliderCode_sliderFFTScaler] -- add your slider handling code here..
        filterGraph->setFFTParams(true, sliderFFTScaler->getValue());
        //[/UserSliderCode_sliderFFTScaler]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
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

void FilterSettingsComponent::timerCallback()
{
    float scope[SCOPE_SIZE];
    int fftSize;
    if (FFTAnalyzer::getInstance()->scopeRequest(&scope[0], &fftSize))
    {
        filterGraph->setFFTResult(&scope[0], SCOPE_SIZE, fftSize);
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
                 componentName="" parentClasses="public Component, public ChangeBroadcaster, public ActionListener, public ChangeListener, public Timer"
                 constructorParams="FilterBankInfo* _pFilterBankInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* _pPresetHelper, int _channelIndex"
                 variableInitialisers="pPresetHelper(_pPresetHelper), pFilterBankInfo(_pFilterBankInfo), channelIndex(_channelIndex)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="700" initialHeight="550">
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
  <TOGGLEBUTTON name="toggleFFT" id="7049cb4ab444a015" memberName="toggleFFT"
                virtualName="" explicitFocusOrder="0" pos="0r 42 58 24" posRelativeX="8939080a2743d889"
                buttonText="FFT" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="toggleBypass" id="7049cb4ab444a482" memberName="toggleBypass"
                virtualName="" explicitFocusOrder="0" pos="0r 42 128 24" posRelativeX="7049cb4ab444a015"
                buttonText="Bypass filter" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="sliderFFTScaler" id="8c8b26e83a78b29" memberName="sliderFFTScaler"
          virtualName="" explicitFocusOrder="0" pos="170R 42 170 24" min="0.0"
          max="50.0" int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelFFTScaler" id="8939080a2743d889" memberName="labelFFTScaler"
         virtualName="" explicitFocusOrder="0" pos="0r 42 85 24" posRelativeX="8c8b26e83a78b29"
         edTextCol="ff000000" edBkgCol="0" labelText="Scaler [dB]:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
