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

#include "../../Common/FilterInfo.h"
#include "../../Common/Constants.h"
#include "SingleFilterSettingsComponent.h"

SingleFilterSettingsComponent::SingleFilterSettingsComponent (FilterInfo* _pFilterInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, Colour color_)
    : pFilterInfo(_pFilterInfo), color(color_)
{
    groupMain.reset (new juce::GroupComponent ("groupMain",
                                               juce::String()));
    addAndMakeVisible (groupMain.get());

    sliderGain.reset (new juce::Slider ("sliderGain"));
    addAndMakeVisible (sliderGain.get());
    sliderGain->setRange (Constants::FilterGainDbMin, Constants::FilterGainDbMax, 0.1);
    sliderGain->setSliderStyle(juce::Slider::LinearBar);
    sliderGain->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    sliderGain->addListener (this);
    sliderGain->setNumDecimalPlacesToDisplay(1);
    sliderGain->setTextValueSuffix(" dB");
    sliderGain->setDoubleClickReturnValue(true, 0.0);
    sliderGain->setTooltip("Gain [dB]");
    //sliderGain->setColour(juce::Slider::rotarySliderFillColourId, Colours::darkgreen);

    sliderQ.reset (new juce::Slider ("sliderQ"));
    addAndMakeVisible (sliderQ.get());
    sliderQ->setRange (0.001, 100, 0);
    sliderQ->setSliderStyle(juce::Slider::LinearBar);
    sliderQ->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    sliderQ->addListener (this);
    sliderQ->setDoubleClickReturnValue(true, 0.0);
    sliderQ->setTextValueSuffix(" (Q)");
    sliderQ->setTooltip("Q value");
    //sliderQ->setColour(juce::Slider::rotarySliderFillColourId, Colours::darkorange);
    sliderQ->setNumDecimalPlacesToDisplay(3);

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
    sliderFrequency->setTooltip("Cutoff frequency [Hz]");

    labelQ.reset (new juce::Label ("labelQ",
                                   TRANS("Q")));
    addAndMakeVisible (labelQ.get());
    labelQ->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelQ->setJustificationType (juce::Justification::centred);
    labelQ->setEditable (false, false, false);
    labelQ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelQ->setColour (juce::TextEditor::backgroundColourId, Colours::darkorange);

    labelGain.reset (new juce::Label ("labelGain",
                                      TRANS("Gain")));
    addAndMakeVisible (labelGain.get());
    labelGain->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelGain->setJustificationType (juce::Justification::centred);
    labelGain->setEditable (false, false, false);
    labelGain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGain->setColour (juce::TextEditor::backgroundColourId, Colours::darkgreen);
    
    if(color != Colours::transparentBlack)
    {
        groupMain->setColour(GroupComponent::outlineColourId, color);
    }
    
    sliderFrequency->setTextValueSuffix(" Hz");

    setSize (600, 400);


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
    
    setLookAndFeel(&ambiLookAndFeel);

    updateUi();

    addChangeListener(pChangeListener);
}

SingleFilterSettingsComponent::~SingleFilterSettingsComponent()
{
    setLookAndFeel(nullptr);
    removeAllChangeListeners();

    groupMain = nullptr;
    sliderGain = nullptr;
    sliderQ = nullptr;
    comboBoxType = nullptr;
    sliderFrequency = nullptr;
    labelQ = nullptr;
    labelGain = nullptr;
}

void SingleFilterSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void SingleFilterSettingsComponent::resized()
{
    int horizBorder = 4;
    int vertBorder = 0;
    int offset = 10;
    int height = int((getHeight() - offset - 4 - 8*vertBorder) / 4.0);
    groupMain->setBounds (0, 0, getWidth() - 0, getHeight());
    comboBoxType->setBounds(horizBorder, offset + vertBorder, getWidth() - 2 * horizBorder, height);
    sliderFrequency->setBounds(horizBorder, offset + height + 3*vertBorder, getWidth() - 2 * horizBorder, height);
    sliderGain->setBounds(horizBorder, offset + 2*height + 5*vertBorder, getWidth() - 2 * horizBorder, height);
    sliderQ->setBounds(horizBorder, offset + 3*height + 7*vertBorder, getWidth() - 2*horizBorder, getHeight() - (4 + offset+ 3*height + 7*vertBorder));
}

void SingleFilterSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderGain.get())
    {
        pFilterInfo->gainFactor = Decibels::decibelsToGain(float(sliderGain->getValue()));
    }
    else if (sliderThatWasMoved == sliderQ.get())
    {
        pFilterInfo->qValue = float(sliderQ->getValue());
    }
    else if (sliderThatWasMoved == sliderFrequency.get())
    {
        pFilterInfo->cutOffFrequencyHz = float(sliderFrequency->getValue());
    }

    sendChangeMessage();
}

void SingleFilterSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == comboBoxType.get())
    {
        pFilterInfo->filterType = FilterInfo::FilterType(comboBoxType->getSelectedId() - 1);
        pFilterInfo->cutOffFrequencyHz = pFilterInfo->defaultFrequency();
        pFilterInfo->qValue = pFilterInfo->defaultQ();
        pFilterInfo->gainFactor = pFilterInfo->defaultGainFactor();
        updateUi();
        sendChangeMessage();
    }
}

void SingleFilterSettingsComponent::updateUi()
{
    comboBoxType->setSelectedId(1 + pFilterInfo->filterType, dontSendNotification);
    sliderFrequency->setValue(pFilterInfo->cutOffFrequencyHz, dontSendNotification);
    sliderQ->setValue(pFilterInfo->qValue, dontSendNotification);
    sliderGain->setValue(Decibels::gainToDecibels(pFilterInfo->gainFactor), dontSendNotification);

    sliderFrequency->setEnabled(pFilterInfo->frequencyRequired());
    sliderQ->setEnabled(pFilterInfo->qRequired());
    sliderGain->setEnabled(pFilterInfo->gainRequired());
}

Colour SingleFilterSettingsComponent::getColor() const
{
    return color;
}
