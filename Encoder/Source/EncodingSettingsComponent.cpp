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

    toggleDoppler.reset (new juce::ToggleButton ("toggleDoppler"));
    addAndMakeVisible (toggleDoppler.get());
    toggleDoppler->setButtonText (TRANS("Enable Doppler"));
    toggleDoppler->addListener (this);

    toggleDoppler->setBounds (6, 8, 199, 24);

    toggleBypassBlauert.reset (new juce::ToggleButton ("toggleBypassBlauert"));
    addAndMakeVisible (toggleBypassBlauert.get());
    toggleBypassBlauert->setButtonText (TRANS("Bypass"));
    toggleBypassBlauert->addListener (this);

    labelIntensity.reset (new juce::Label ("labelIntensity",
                                           TRANS("Blauert Intensity")));
    addAndMakeVisible (labelIntensity.get());
    labelIntensity->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelIntensity->setJustificationType (juce::Justification::centredLeft);
    labelIntensity->setEditable (false, false, false);
    labelIntensity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelIntensity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    
    sliderBlauertIntensity.reset(new Slider("sliderBlauertIntensity"));
    addAndMakeVisible (sliderBlauertIntensity.get());
    sliderBlauertIntensity->addListener(this);
    sliderBlauertIntensity->setRange(0.0, 5.0);
    sliderBlauertIntensity->setNumDecimalPlacesToDisplay(3);

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
    toggleDoppler = nullptr;
    toggleBypassBlauert = nullptr;
    labelIntensity = nullptr;
    sliderBlauertIntensity = nullptr;
    distanceEncodingComponent = nullptr;
}

void EncodingSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void EncodingSettingsComponent::resized()
{
    groupDistanceEncoding->setBounds (8, 40, getWidth() - 14, getHeight() - 46);
    toggleDistanceEncoding->setBounds (8 + 14, 40 + 24, 199, 24);
    distanceEncodingComponent->setBounds (8 + 14, 40 + 56, (getWidth() - 14) - 28, (getHeight() - 46) - 70);
    
    toggleBypassBlauert->setBounds (getWidth()-80, 8, 76, 24);
    sliderBlauertIntensity->setBounds(getWidth()-280, 8, 196, 24);
    labelIntensity->setBounds(getWidth()-400, 8, 116, 24);
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
    else if (buttonThatWasClicked == toggleBypassBlauert.get())
    {
        m_args.pSettings->bypassBlauertFlag = toggleBypassBlauert->getToggleState();
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

void EncodingSettingsComponent::updateEncodingUiElements()
{
    toggleDistanceEncoding->setToggleState(m_args.pSettings->distanceEncodingFlag, dontSendNotification);

    toggleDoppler->setToggleState(m_args.pSettings->dopplerEncodingFlag, dontSendNotification);
    toggleBypassBlauert->setToggleState(m_args.pSettings->bypassBlauertFlag, dontSendNotification);
    
    sliderBlauertIntensity->setValue(m_args.pSettings->blauertIntensity, dontSendNotification);
    controlDimming();
}

void EncodingSettingsComponent::controlDimming()
{
    distanceEncodingComponent->setEnabled(toggleDistanceEncoding->getToggleState());
    sliderBlauertIntensity->setEnabled(!toggleBypassBlauert->getToggleState());
}
