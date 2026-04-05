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

#include "HelpRadarInteraction.h"
#include "HelpOscSyntax.h"
#include "HelpWebBrowserComponent.h"
#include "Constants.h"
#include "HelpComponent.h"

HelpComponent::HelpComponent (bool isEncoder)
{
    tabHelp.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabHelp.get());
    tabHelp->setTabBarDepth (30);
    tabHelp->addTab (TRANS("Radar Interaction"), juce::Colours::lightgrey, new HelpRadarInteraction(), true);
    tabHelp->setCurrentTabIndex (0);

    label.reset (new juce::Label ("new label",
                                  juce::CharPointer_UTF8 ("\xc2\xa9 2024 Martin Neukom, Johannes Schuett & Christian Schweizer @ ICST")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    label->setJustificationType (juce::Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    if(isEncoder)
    {
        tabHelp->addTab (TRANS("OSC Syntax"), Colours::lightgrey, new HelpOscSyntax(), true);
    }

    String documentationUrl = isEncoder
    ? "https://github.com/schweizerweb/icst-ambisonics-plugins/wiki/ICST-AmbiEncoder"
    : "https://github.com/schweizerweb/icst-ambisonics-plugins/wiki/ICST-AmbiDecoder";
    tabHelp->addTab(TRANS("Documentation"), Colours::lightgrey, new HelpWebBrowserComponent(documentationUrl), true);
    tabHelp->addTab(TRANS("Tutorials"), Colours::lightgrey, new HelpWebBrowserComponent("https://ambisonics.ch/icst-ambisonics-plugins"), true);

    label->setText(String(JucePlugin_Name).upToFirstOccurrenceOf("_", false, false) + Constants::getUiVersionString(true) + " " + String::fromUTF8("\xc2\xa9 2024 Martin Neukom, Johannes Schuett & Christian Schweizer @ ICST"), dontSendNotification);
    label->setTooltip(Constants::getBuildInfo());

    setSize (600, 400);
}

HelpComponent::~HelpComponent()
{
    tabHelp = nullptr;
    label = nullptr;
}

void HelpComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void HelpComponent::resized()
{
    tabHelp->setBounds (0, 0, getWidth() - 0, getHeight() - 22);
    label->setBounds (0, getHeight() - 3 - 16, getWidth() - 0, 16);
}
