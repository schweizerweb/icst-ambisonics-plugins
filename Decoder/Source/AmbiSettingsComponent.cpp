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

#include "AmbiSettingsComponent.h"

AmbiSettingsComponent::AmbiSettingsComponent(AmbiSettings* _pAmbiSettings, ChangeListener* pChangeListener, ChannelLayout* _pChannelLayout): pAmbiSettings(_pAmbiSettings), pChannelLayout(_pChannelLayout)
{
    pChannelLayout->addActionListener(this);

    OwnedArray<String> ambiChannelNames;
    for (int i = 0; i < MAX_NB_OF_AMBISONICS_GAINS; i++)
        ambiChannelNames.add(new String("m = " + String(i)));

    ambiChannelControl.reset(new MultiSliderControl(MAX_NB_OF_AMBISONICS_GAINS, pAmbiSettings->getAmbiOrderWeightPointer(), &ambiChannelNames, 0.0, 1.5, 0.00001));
    addAndMakeVisible(ambiChannelControl.get());
    ambiChannelControl->setName("ambiChannelControl");

    labelChannelWeights.reset(new juce::Label("labelChannelWeights",
        TRANS("Channel weights:")));
    addAndMakeVisible(labelChannelWeights.get());
    labelChannelWeights->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    labelChannelWeights->setJustificationType(juce::Justification::centredLeft);
    labelChannelWeights->setEditable(false, false, false);
    labelChannelWeights->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelChannelWeights->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    comboBoxChannelWeightingMode.reset(new juce::ComboBox("comboBoxChannelWeightingMode"));
    addAndMakeVisible(comboBoxChannelWeightingMode.get());
    comboBoxChannelWeightingMode->setEditableText(false);
    comboBoxChannelWeightingMode->setJustificationType(juce::Justification::centredLeft);
    comboBoxChannelWeightingMode->setTextWhenNothingSelected(juce::String());
    comboBoxChannelWeightingMode->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    comboBoxChannelWeightingMode->addListener(this);

    labelAmbiOrder.reset(new juce::Label("labelAmbiOrder",
        TRANS("Ambisonics order:")));
    addAndMakeVisible(labelAmbiOrder.get());
    labelAmbiOrder->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    labelAmbiOrder->setJustificationType(juce::Justification::centredLeft);
    labelAmbiOrder->setEditable(false, false, false);
    labelAmbiOrder->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelAmbiOrder->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    comboAmbiOrder.reset(new juce::ComboBox("comboAmbiOrder"));
    addAndMakeVisible(comboAmbiOrder.get());
    comboAmbiOrder->setEditableText(false);
    comboAmbiOrder->setJustificationType(juce::Justification::centredLeft);
    comboAmbiOrder->setTextWhenNothingSelected(juce::String());
    comboAmbiOrder->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    comboAmbiOrder->addItem(TRANS("1st (4ch)"), 1);
    comboAmbiOrder->addItem(TRANS("2nd (9ch)"), 2);
    comboAmbiOrder->addItem(TRANS("3rd (16ch)"), 3);
    comboAmbiOrder->addItem(TRANS("4th (25ch)"), 4);
    comboAmbiOrder->addItem(TRANS("5th (36ch)"), 5);
    comboAmbiOrder->addItem(TRANS("6th (49ch)"), 6);
    comboAmbiOrder->addItem(TRANS("7th (64ch)"), 7);
    comboAmbiOrder->addListener(this);

    // prepare weighting comboBox
    comboBoxChannelWeightingMode->addItem("Basic", AmbiSettings::BASIC);
    comboBoxChannelWeightingMode->addItem("In-Phase", AmbiSettings::INPHASE);
    comboBoxChannelWeightingMode->addItem("Max-rE", AmbiSettings::MAXRE);
    comboBoxChannelWeightingMode->addItem("Manual", AmbiSettings::MANUAL);

    updateUI();
    handleAmbiOrders();

    addChangeListener(pChangeListener);

    controlDimming();
    setSize (600, 400);

}

AmbiSettingsComponent::~AmbiSettingsComponent()
{
    pChannelLayout->removeActionListener(this);
    
    ambiChannelControl = nullptr;
    labelChannelWeights = nullptr;
    comboBoxChannelWeightingMode = nullptr;
    labelAmbiOrder = nullptr;
    comboAmbiOrder = nullptr;
}

void AmbiSettingsComponent::updateUI() const
{
    ambiChannelControl->updateValues();
    comboBoxChannelWeightingMode->setSelectedId(pAmbiSettings->getWeightMode());

    comboAmbiOrder->setSelectedId(pAmbiSettings->getAmbiOrder(), dontSendNotification);
    ambiChannelControl->setVisibleSliderCount(pAmbiSettings->getGainCount());
}

void AmbiSettingsComponent::handleAmbiOrders()
{
    for (int i = 1; i <= MAX_AMBISONICS_ORDER; i++)
        comboAmbiOrder->setItemEnabled(i, i <= pChannelLayout->getMaxAmbiOrder(false));
    comboAmbiOrder->setSelectedId(pAmbiSettings->getAmbiOrder(), sendNotification);
}

void AmbiSettingsComponent::controlDimming()
{
    ambiChannelControl->setEnabled(pAmbiSettings->getWeightMode() == AmbiSettings::MANUAL);

}

void AmbiSettingsComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    sendChangeMessage();
}

void AmbiSettingsComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == comboBoxChannelWeightingMode.get())
    {
        //[UserComboBoxCode_comboBoxChannelWeightingMode] -- add your combo box handling code here..
        pAmbiSettings->setWeightMode(AmbiSettings::AmbiWeightMode(comboBoxChannelWeightingMode->getSelectedId()));
        ambiChannelControl->updateValues();
        controlDimming();
        sendChangeMessage();
        //[/UserComboBoxCode_comboBoxChannelWeightingMode]
    }
    else if (comboBoxThatHasChanged == comboAmbiOrder.get())
    {
        //[UserComboBoxCode_comboAmbiOrder] -- add your combo box handling code here..
        pAmbiSettings->setAmbiOrder(comboAmbiOrder->getSelectedId());
        ambiChannelControl->setVisibleSliderCount(pAmbiSettings->getGainCount());
        ambiChannelControl->updateValues();
        sendChangeMessage();
        //[/UserComboBoxCode_comboAmbiOrder]
    }
}

void AmbiSettingsComponent::actionListenerCallback(const String& message)
{
    if (message == ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED)
    {
        handleAmbiOrders();
        controlDimming();
    }
}


void AmbiSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff505050));
}

void AmbiSettingsComponent::resized()
{
    int top = 30;
    int border = 2;
    auto width = getWidth();
    auto height = getHeight();

    ambiChannelControl->setBounds(border, top, width - 2*border, height - top - border);
    labelChannelWeights->setBounds(width - 293 - 124 - 116, border, 112, 24);
    comboBoxChannelWeightingMode->setBounds(width - 293 - 124, border, 120, 24);
    labelAmbiOrder->setBounds(width - 293, border, 136, 24);
    comboAmbiOrder->setBounds(width - 157, border, 142, 24);
}

