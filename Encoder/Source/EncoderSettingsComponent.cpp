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

#include "EncoderSettingsComponent.h"

EncoderSettingsComponent::EncoderSettingsComponent (EncoderSettingsComponentArgs args)
{
    tabbedComponent.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabbedComponent.get());
    tabbedComponent->setTabBarDepth (35);
    tabbedComponent->addTab (TRANS("Sources"), juce::Colours::lightgrey, new SourceDefinitionComponent (args), true);
    tabbedComponent->addTab (TRANS("Encoding"), juce::Colours::lightgrey, new EncodingSettingsComponent (args), true);
    tabbedComponent->addTab (TRANS("Radar"), juce::Colours::lightgrey, new RadarSettingsComponent (args.pChangeListener, args.pZoomSettings), true);
    tabbedComponent->addTab (TRANS("OSC In"), juce::Colours::lightgrey, new OSCRxSettingsComponent (args.pSettings, args.pStatusMessageHandler, args.pCustomOscRxPresetHelper, args.pOscLogManager, args.pOscHandler), true);
    tabbedComponent->addTab (TRANS("OSC Out"), juce::Colours::lightgrey, new OSCSettingsComponent (args.pChangeListener, args.pSettings, args.pCustomOscTxPresetHelper), true);
    tabbedComponent->setCurrentTabIndex (0);

    labelDevelopmentVersion.reset (new juce::Label ("labelDevelopmentVersion",
                                                    TRANS("Unofficial Pre-Release")));
    addAndMakeVisible (labelDevelopmentVersion.get());
    labelDevelopmentVersion->setFont (juce::Font (juce::FontOptions(25.00f, juce::Font::plain)));
    labelDevelopmentVersion->setJustificationType (juce::Justification::centred);
    labelDevelopmentVersion->setEditable (false, false, false);
    labelDevelopmentVersion->setColour (juce::Label::backgroundColourId, juce::Colour (0xbded0d0d));
    labelDevelopmentVersion->setColour (juce::Label::textColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::Label::outlineColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDevelopmentVersion->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    setSize (650, 300);

	labelDevelopmentVersion->setVisible(Constants::isDevelopmentVersion() && !Constants::isNonVisibleVersionPrerelease());
}

EncoderSettingsComponent::~EncoderSettingsComponent()
{
    tabbedComponent = nullptr;
    labelDevelopmentVersion = nullptr;
}

void EncoderSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void EncoderSettingsComponent::resized()
{
    tabbedComponent->setBounds (8, 8, getWidth() - 15, getHeight() - 13);
    labelDevelopmentVersion->setBounds (getWidth() - 10 - 245, 8, 245, 24);
}
