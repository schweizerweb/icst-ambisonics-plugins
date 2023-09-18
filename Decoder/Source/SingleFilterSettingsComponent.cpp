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
#include "../../Common/FilterInfo.h"
//[/Headers]

#include "SingleFilterSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#if JUCE_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif
//[/MiscUserDefs]

//==============================================================================
SingleFilterSettingsComponent::SingleFilterSettingsComponent (FilterInfo* _pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener)
    : pFilterInfo(_pFilterInfo)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    groupMain.reset (new juce::GroupComponent ("groupMain",
                                               juce::String()));
    addAndMakeVisible (groupMain.get());

    sliderGain.reset (new juce::Slider ("sliderGain"));
    addAndMakeVisible (sliderGain.get());
    sliderGain->setRange (0.001, 20, 0.001);
    sliderGain->setSliderStyle (juce::Slider::Rotary);
    sliderGain->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    sliderGain->addListener (this);

    sliderQ.reset (new juce::Slider ("sliderQ"));
    addAndMakeVisible (sliderQ.get());
    sliderQ->setRange (0.001, 100, 0);
    sliderQ->setSliderStyle (juce::Slider::Rotary);
    sliderQ->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    sliderQ->addListener (this);

    comboBoxType.reset (new juce::ComboBox ("comboBoxType"));
    addAndMakeVisible (comboBoxType.get());
    comboBoxType->setEditableText (false);
    comboBoxType->setJustificationType (juce::Justification::centredLeft);
    comboBoxType->setTextWhenNothingSelected (juce::String());
    comboBoxType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxType->addListener (this);

    sliderFrequency.reset (new juce::Slider ("sliderFrequency"));
    addAndMakeVisible (sliderFrequency.get());
    sliderFrequency->setRange (20, 15000, 1);
    sliderFrequency->setSliderStyle (juce::Slider::LinearBar);
    sliderFrequency->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderFrequency->addListener (this);

    labelQ.reset (new juce::Label ("labelQ",
                                   TRANS("Q")));
    addAndMakeVisible (labelQ.get());
    labelQ->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelQ->setJustificationType (juce::Justification::centred);
    labelQ->setEditable (false, false, false);
    labelQ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelQ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelGain.reset (new juce::Label ("labelGain",
                                      TRANS("Gain")));
    addAndMakeVisible (labelGain.get());
    labelGain->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelGain->setJustificationType (juce::Justification::centred);
    labelGain->setEditable (false, false, false);
    labelGain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    sliderFrequency->setTextValueSuffix(" [Hz]");
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    comboBoxType->addItem("Off", 1 + FilterInfo::FilterType::None);
    comboBoxType->addItem("Low-Pass", 1 + FilterInfo::FilterType::LowPass);
    comboBoxType->addItem("Band-Pass", 1 + FilterInfo::FilterType::BandPass);
    comboBoxType->addItem("High-Pass", 1 + FilterInfo::FilterType::HighPass);
    comboBoxType->addItem("Low-Pass (O1)", 1 + FilterInfo::FilterType::FirstOrderLowPass);
    comboBoxType->addItem("High-Pass (O1)", 1 + FilterInfo::FilterType::FirstOrderHighPass);
    comboBoxType->addItem("Notch", 1 + FilterInfo::FilterType::Notch);
    comboBoxType->addItem("Low Shelf", 1 + FilterInfo::FilterType::LowShelf);
    comboBoxType->addItem("High Shelf", 1 + FilterInfo::FilterType::HighShelf);
    comboBoxType->addItem("Peak", 1 + FilterInfo::FilterType::Peak);


    sliderFrequency->setSkewFactorFromMidPoint(500);
    sliderFrequency->setRange(20, jmin(int(pFilterSpecification->sampleRate / 2.0), 22000));
    sliderFrequency->setNumDecimalPlacesToDisplay(0);
    sliderQ->setNumDecimalPlacesToDisplay(3);
    sliderQ->setSkewFactorFromMidPoint(1.0);
    sliderGain->setNumDecimalPlacesToDisplay(3);
    sliderGain->setSkewFactorFromMidPoint(1.0);

    updateUi();

    addChangeListener(pChangeListener);
    //[/Constructor]
}

SingleFilterSettingsComponent::~SingleFilterSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    removeAllChangeListeners();
    //[/Destructor_pre]

    groupMain = nullptr;
    sliderGain = nullptr;
    sliderQ = nullptr;
    comboBoxType = nullptr;
    sliderFrequency = nullptr;
    labelQ = nullptr;
    labelGain = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SingleFilterSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SingleFilterSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupMain->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
    sliderGain->setBounds (getWidth() - proportionOfWidth (0.5000f), 88, proportionOfWidth (0.5000f), getHeight() - 96);
    sliderQ->setBounds (0, 88, proportionOfWidth (0.5000f), getHeight() - 96);
    comboBoxType->setBounds (16, 24, getWidth() - 32, 24);
    sliderFrequency->setBounds (16, 56, getWidth() - 32, 24);
    labelQ->setBounds (0 + juce::roundToInt (proportionOfWidth (0.5000f) * 0.5017f) - (24 / 2), 88 + juce::roundToInt ((getHeight() - 96) * 0.5021f) - (24 / 2), 24, 24);
    labelGain->setBounds ((getWidth() - proportionOfWidth (0.5000f)) + juce::roundToInt (proportionOfWidth (0.5000f) * 0.4983f) - (48 / 2), 88 + juce::roundToInt ((getHeight() - 96) * 0.5021f) - (24 / 2), 48, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SingleFilterSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderGain.get())
    {
        //[UserSliderCode_sliderGain] -- add your slider handling code here..
        pFilterInfo->gainFactor = float(sliderGain->getValue());
        //[/UserSliderCode_sliderGain]
    }
    else if (sliderThatWasMoved == sliderQ.get())
    {
        //[UserSliderCode_sliderQ] -- add your slider handling code here..
        pFilterInfo->qValue = float(sliderQ->getValue());
        //[/UserSliderCode_sliderQ]
    }
    else if (sliderThatWasMoved == sliderFrequency.get())
    {
        //[UserSliderCode_sliderFrequency] -- add your slider handling code here..
        pFilterInfo->cutOffFrequencyHz = float(sliderFrequency->getValue());
        //[/UserSliderCode_sliderFrequency]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
    //[/UsersliderValueChanged_Post]
}

void SingleFilterSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxType.get())
    {
        //[UserComboBoxCode_comboBoxType] -- add your combo box handling code here..
        pFilterInfo->filterType = FilterInfo::FilterType(comboBoxType->getSelectedId() - 1);
        pFilterInfo->cutOffFrequencyHz = pFilterInfo->defaultFrequency();
        pFilterInfo->qValue = pFilterInfo->defaultQ();
        pFilterInfo->gainFactor = pFilterInfo->defaultGainFactor();
        updateUi();
        sendChangeMessage();
        //[/UserComboBoxCode_comboBoxType]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SingleFilterSettingsComponent::updateUi()
{
    comboBoxType->setSelectedId(1 + pFilterInfo->filterType, dontSendNotification);
    sliderFrequency->setValue(pFilterInfo->cutOffFrequencyHz);
    sliderQ->setValue(pFilterInfo->qValue);
    sliderGain->setValue(pFilterInfo->gainFactor);

    sliderFrequency->setEnabled(pFilterInfo->frequencyRequired());
    sliderQ->setEnabled(pFilterInfo->qRequired());
    sliderGain->setEnabled(pFilterInfo->gainRequired());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SingleFilterSettingsComponent"
                 componentName="" parentClasses="public juce::Component, public ChangeBroadcaster"
                 constructorParams="FilterInfo* _pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener"
                 variableInitialisers="pFilterInfo(_pFilterInfo)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupMain" id="8ef5d6310f4a0551" memberName="groupMain"
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 0M" title=""/>
  <SLIDER name="sliderGain" id="6dc6c3b4f1230227" memberName="sliderGain"
          virtualName="" explicitFocusOrder="0" pos="0Rr 88 50% 96M" min="0.001"
          max="20.0" int="0.001" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sliderQ" id="d75dd45147568a5" memberName="sliderQ" virtualName=""
          explicitFocusOrder="0" pos="0 88 50% 96M" min="0.001" max="100.0"
          int="0.0" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <COMBOBOX name="comboBoxType" id="8896d80d8503e534" memberName="comboBoxType"
            virtualName="" explicitFocusOrder="0" pos="16 24 32M 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sliderFrequency" id="f23e9b966efc9df5" memberName="sliderFrequency"
          virtualName="" explicitFocusOrder="0" pos="16 56 32M 24" min="20.0"
          max="15000.0" int="1.0" style="LinearBar" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelQ" id="c59d0be17b74b579" memberName="labelQ" virtualName=""
         explicitFocusOrder="0" pos="50.173%c 50.182%c 24 24" posRelativeX="d75dd45147568a5"
         posRelativeY="d75dd45147568a5" edTextCol="ff000000" edBkgCol="0"
         labelText="Q" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="labelGain" id="5e0ece6555401094" memberName="labelGain"
         virtualName="" explicitFocusOrder="0" pos="49.827%c 50.182%c 48 24"
         posRelativeX="6dc6c3b4f1230227" posRelativeY="6dc6c3b4f1230227"
         edTextCol="ff000000" edBkgCol="0" labelText="Gain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
