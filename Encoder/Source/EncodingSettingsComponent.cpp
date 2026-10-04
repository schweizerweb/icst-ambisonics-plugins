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

#include "EncoderPresetHelper.h"
#include "EncodingSettingsComponent.h"

EncodingSettingsComponent::EncodingSettingsComponent (EncoderSettingsComponentArgs args)
    : m_args(args)
{
    addChangeListener(m_args.pChangeListener);

    groupDistanceEncoding.reset (new juce::GroupComponent ("groupDistanceEncoding",
                                                           TRANS("Distance Encoding")));
    addAndMakeVisible (groupDistanceEncoding.get());

    toggleDistanceEncoding.reset (new juce::ToggleButton ("toggleDistanceEncoding"));
    addAndMakeVisible (toggleDistanceEncoding.get());
    toggleDistanceEncoding->setButtonText (TRANS("Enable"));
    toggleDistanceEncoding->addListener (this);

    groupDoppler.reset (new juce::GroupComponent ("groupDoppler",
                                                  TRANS("Doppler effect")));
    addAndMakeVisible (groupDoppler.get());

    toggleDoppler.reset (new juce::ToggleButton ("toggleDoppler"));
    addAndMakeVisible (toggleDoppler.get());
    toggleDoppler->setButtonText (TRANS("Enable"));
    toggleDoppler->addListener (this);

    groupBlauert.reset (new juce::GroupComponent ("groupBlauert",
                                                  TRANS("Blauert filter")));
    addAndMakeVisible (groupBlauert.get());

    toggleEnableBlauert.reset (new juce::ToggleButton ("toggleEnableBlauert"));
    addAndMakeVisible (toggleEnableBlauert.get());
    toggleEnableBlauert->setButtonText (TRANS("Enable"));
    toggleEnableBlauert->addListener (this);

    labelIntensity.reset (new juce::Label ("labelIntensity",
                                           TRANS("Intensity Factor")));
    addAndMakeVisible (labelIntensity.get());
    labelIntensity->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelIntensity->setJustificationType (juce::Justification::centredLeft);
    labelIntensity->setEditable (false, false, false);
    labelIntensity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelIntensity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    
    sliderBlauertIntensity.reset(new Slider("sliderBlauertIntensity"));
    addAndMakeVisible (sliderBlauertIntensity.get());
    sliderBlauertIntensity->addListener(this);
    sliderBlauertIntensity->setRange(0.1, 5.0);
    sliderBlauertIntensity->setNumDecimalPlacesToDisplay(3);

    comboBlauertMode.reset(new ComboBox("comboBlauertMode"));
    addAndMakeVisible (comboBlauertMode.get());
    comboBlauertMode->addItem(TRANS("Standard"), 1);
    comboBlauertMode->addItem(TRANS("Height Only"), 2);
    comboBlauertMode->addListener(this);

    distanceEncodingComponent.reset (new DistanceEncodingComponent (&m_args.pSettings->distanceEncodingParams, m_args.pDistanceEncodingPresetHelper, m_args.pZoomSettings));
    addAndMakeVisible (distanceEncodingComponent.get());
    distanceEncodingComponent->setName ("distanceEncodingComponent");

    setSize (600, 400);

    updateEncodingUiElements();
}

EncodingSettingsComponent::~EncodingSettingsComponent()
{
    groupDistanceEncoding = nullptr;
    toggleDistanceEncoding = nullptr;
    groupDoppler = nullptr;
    toggleDoppler = nullptr;
    groupBlauert = nullptr;
    toggleEnableBlauert = nullptr;
    labelIntensity = nullptr;
    sliderBlauertIntensity = nullptr;
    comboBlauertMode = nullptr;
    distanceEncodingComponent = nullptr;
}

void EncodingSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void EncodingSettingsComponent::resized()
{
    const int groupWidth = getWidth() - 14;
    const int contentX = 8 + 14;

    groupDoppler->setBounds (8, 8, groupWidth, 56);
    toggleDoppler->setBounds (contentX, 8 + 24, 150, 24);

    groupBlauert->setBounds (8, 72, groupWidth, 56);
    toggleEnableBlauert->setBounds (contentX, 72 + 24, 76, 24);
    comboBlauertMode->setBounds (contentX + 76 + 10, 72 + 24, 110, 24);
    labelIntensity->setBounds (contentX + 76 + 10 + 110 + 14, 72 + 24, 116, 24);
    sliderBlauertIntensity->setBounds (contentX + 76 + 10 + 110 + 14 + 116 + 4, 72 + 24, 196, 24);

    groupDistanceEncoding->setBounds (8, 136, groupWidth, getHeight() - 142);
    toggleDistanceEncoding->setBounds (contentX, 136 + 24, 199, 24);
    distanceEncodingComponent->setBounds (contentX, 136 + 56, groupWidth - 28, (getHeight() - 142) - 70);
}

void EncodingSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == toggleDistanceEncoding.get())
    {
        m_args.pSettings->distanceEncodingFlag = toggleDistanceEncoding->getToggleState();
        sendChangeMessage();
        controlDimming();
    }
    else if (buttonThatWasClicked == toggleDoppler.get())
    {
        m_args.pSettings->dopplerEncodingFlag = toggleDoppler->getToggleState();
        sendChangeMessage();
    }
    else if (buttonThatWasClicked == toggleEnableBlauert.get())
    {
        m_args.pSettings->bypassBlauertFlag = !toggleEnableBlauert->getToggleState();
        sendChangeMessage();
        controlDimming();
    }
}

void EncodingSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderBlauertIntensity.get())
    {
        m_args.pSettings->blauertIntensity = sliderBlauertIntensity->getValue();
        sendChangeMessage();
    }
}

void EncodingSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == comboBlauertMode.get())
    {
        m_args.pSettings->blauertHeightOnlyMode = (comboBlauertMode->getSelectedId() == 2);
        sendChangeMessage();
    }
}

void EncodingSettingsComponent::updateEncodingUiElements()
{
    toggleDistanceEncoding->setToggleState(m_args.pSettings->distanceEncodingFlag, dontSendNotification);

    toggleDoppler->setToggleState(m_args.pSettings->dopplerEncodingFlag, dontSendNotification);
    toggleEnableBlauert->setToggleState(!m_args.pSettings->bypassBlauertFlag, dontSendNotification);
    
    sliderBlauertIntensity->setValue(m_args.pSettings->blauertIntensity, dontSendNotification);
    comboBlauertMode->setSelectedId(m_args.pSettings->blauertHeightOnlyMode ? 2 : 1, dontSendNotification);
    controlDimming();
}

void EncodingSettingsComponent::controlDimming()
{
    distanceEncodingComponent->setEnabled(toggleDistanceEncoding->getToggleState());
    sliderBlauertIntensity->setEnabled(toggleEnableBlauert->getToggleState());
    comboBlauertMode->setEnabled(toggleEnableBlauert->getToggleState());
}
