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

#include "OSCSettingsComponent.h"

OSCSettingsComponent::OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* _pSettings, CustomOscTxPresetHelper*  pCustomOscTxPresetHelper)
    : pSettings(_pSettings)
{
    addChangeListener(pChangeListener);

    groupExternal.reset (new juce::GroupComponent ("groupExternal",
                                                   TRANS("External")));
    addAndMakeVisible (groupExternal.get());

    groupInternal.reset (new juce::GroupComponent ("groupInternal",
                                                   TRANS("Internal")));
    addAndMakeVisible (groupInternal.get());

    toggleSendOsc.reset (new juce::ToggleButton ("toggleSendOsc"));
    addAndMakeVisible (toggleSendOsc.get());
    toggleSendOsc->setButtonText (TRANS("Send Positions to Decoder"));
    toggleSendOsc->addListener (this);

    textOscSendIp.reset (new juce::TextEditor ("textOscSendIp"));
    addAndMakeVisible (textOscSendIp.get());
    textOscSendIp->setMultiLine (false);
    textOscSendIp->setReturnKeyStartsNewLine (false);
    textOscSendIp->setReadOnly (false);
    textOscSendIp->setScrollbarsShown (true);
    textOscSendIp->setCaretVisible (true);
    textOscSendIp->setPopupMenuEnabled (true);
    textOscSendIp->setText (juce::String());

    labelOscSendIp.reset (new juce::Label ("labelOscSendIp",
                                           TRANS("Target Host/Port:")));
    addAndMakeVisible (labelOscSendIp.get());
    labelOscSendIp->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelOscSendIp->setJustificationType (juce::Justification::centredRight);
    labelOscSendIp->setEditable (false, false, false);
    labelOscSendIp->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendIp->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelOscSendInterval.reset (new juce::Label ("labelOscSendInterval",
                                                 TRANS("Interval [ms]:")));
    addAndMakeVisible (labelOscSendInterval.get());
    labelOscSendInterval->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelOscSendInterval->setJustificationType (juce::Justification::centredRight);
    labelOscSendInterval->setEditable (false, false, false);
    labelOscSendInterval->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscSendInterval->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleSendOscExt.reset (new juce::ToggleButton ("toggleSendOscExt"));
    addAndMakeVisible (toggleSendOscExt.get());
    toggleSendOscExt->setButtonText (TRANS("Send Positions for External Usage"));
    toggleSendOscExt->addListener (this);

    labelExternalOscInfo.reset (new juce::Label ("labelExternalOscInfo",
                                                 TRANS("no active targets")));
    addAndMakeVisible (labelExternalOscInfo.get());
    labelExternalOscInfo->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelExternalOscInfo->setJustificationType (juce::Justification::centredRight);
    labelExternalOscInfo->setEditable (false, false, false);
    labelExternalOscInfo->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExternalOscInfo->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderSendOscPort.reset (new juce::Slider ("sliderSendOscPort"));
    addAndMakeVisible (sliderSendOscPort.get());
    sliderSendOscPort->setRange (0, 65535, 1);
    sliderSendOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderSendOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderSendOscPort->addListener (this);

    sliderInterval.reset (new juce::Slider ("sliderInterval"));
    addAndMakeVisible (sliderInterval.get());
    sliderInterval->setRange (10, 1000, 1);
    sliderInterval->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderInterval->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderInterval->addListener (this);

    oscTargets.reset (new OSCTargetsComponent (this, pSettings, pCustomOscTxPresetHelper));
    addAndMakeVisible (oscTargets.get());
    oscTargets->setName ("oscTargets");

    labelLoadInfo.reset (new juce::Label ("labelLoadInfo",
                                          TRANS("no network traffic")));
    addAndMakeVisible (labelLoadInfo.get());
    labelLoadInfo->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelLoadInfo->setJustificationType (juce::Justification::centredLeft);
    labelLoadInfo->setEditable (false, false, false);
    labelLoadInfo->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelLoadInfo->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    setSize (600, 400);

    textOscSendIp->addListener(this);
    sliderInterval->setSkewFactorFromMidPoint(200.0);

    toggleSendOsc->setToggleState(pSettings->oscSendFlag, dontSendNotification);
    textOscSendIp->setText(String(pSettings->oscSendTargetHost), false);
    sliderSendOscPort->setValue(pSettings->oscSendPort, dontSendNotification);
    sliderInterval->setValue(pSettings->oscSendIntervalMs, dontSendNotification);

    toggleSendOscExt->setToggleState(pSettings->oscSendExtMasterFlag, dontSendNotification);
    controlDimming();
}

OSCSettingsComponent::~OSCSettingsComponent()
{
    groupExternal = nullptr;
    groupInternal = nullptr;
    toggleSendOsc = nullptr;
    textOscSendIp = nullptr;
    labelOscSendIp = nullptr;
    labelOscSendInterval = nullptr;
    toggleSendOscExt = nullptr;
    labelExternalOscInfo = nullptr;
    sliderSendOscPort = nullptr;
    sliderInterval = nullptr;
    oscTargets = nullptr;
    labelLoadInfo = nullptr;
}

void OSCSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void OSCSettingsComponent::resized()
{
    groupExternal->setBounds (8, 113, getWidth() - 16, getHeight() - 155);
    groupInternal->setBounds (8, 8, getWidth() - 16, 96);
    toggleSendOsc->setBounds (8 + 14, 8 + 27, 199, 24);
    textOscSendIp->setBounds (8 + (getWidth() - 16) - 123 - 106, 8 + 27, 106, 24);
    labelOscSendIp->setBounds (8 + (getWidth() - 16) - 236 - 124, 8 + 27, 124, 24);
    labelOscSendInterval->setBounds (8 + (getWidth() - 16) - 236 - 110, 8 + 57, 110, 24);
    toggleSendOscExt->setBounds (8 + 14, 113 + 24, 245, 24);
    labelExternalOscInfo->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 280), 113 + 24, (getWidth() - 16) - 280, 24);
    sliderSendOscPort->setBounds (8 + (getWidth() - 16) - 16 - 100, 8 + 27, 100, 24);
    sliderInterval->setBounds (8 + (getWidth() - 16) - 16 - 210, 8 + 57, 210, 24);
    oscTargets->setBounds (8 + 16, 113 + 55, (getWidth() - 16) - 24, (getHeight() - 155) - 63);
    labelLoadInfo->setBounds (8, getHeight() - 8 - 24, getWidth() - 15, 24);
}

void OSCSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == toggleSendOsc.get())
    {
        pSettings->oscSendFlag = toggleSendOsc->getToggleState();
        sendChangeMessage();
    }
    else if (buttonThatWasClicked == toggleSendOscExt.get())
    {
        pSettings->oscSendExtMasterFlag = toggleSendOscExt->getToggleState();
        sendChangeMessage();
    }

    controlDimming();
}

void OSCSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderSendOscPort.get())
    {
        pSettings->oscSendPort = int(sliderSendOscPort->getValue());
    }
    else if (sliderThatWasMoved == sliderInterval.get())
    {
        pSettings->oscSendIntervalMs = int(sliderInterval->getValue());
    }

    sendChangeMessage();
    controlDimming();
}

void OSCSettingsComponent::textEditorTextChanged(TextEditor& textEditor)
{
    if (&textEditor == textOscSendIp.get())
        pSettings->oscSendTargetHost = textEditor.getText();

    sendChangeMessage();
}

void OSCSettingsComponent::controlDimming() const
{
    labelOscSendIp->setEnabled(pSettings->oscSendFlag);
    labelOscSendInterval->setEnabled(pSettings->oscSendFlag);
    textOscSendIp->setEnabled(pSettings->oscSendFlag);
    sliderSendOscPort->setEnabled(pSettings->oscSendFlag);
    sliderInterval->setEnabled(pSettings->oscSendFlag);
    oscTargets->setEnabled(pSettings->oscSendExtMasterFlag);
    labelExternalOscInfo->setEnabled(pSettings->oscSendExtMasterFlag);
    int targetCount = 0;
    if(pSettings->oscSendExtMasterFlag)
    {
        targetCount += (pSettings->oscSendExtXyz->enabledFlag ? 1 : 0) + (pSettings->oscSendExtAed->enabledFlag ? 1 : 0) +
            (pSettings->oscSendExtXyzIndex->enabledFlag ? 1 : 0) +
            (pSettings->oscSendExtAedIndex->enabledFlag ? 1 : 0);
        for (auto target : pSettings->customOscTargets)
        {
            if (target->enabledFlag) targetCount++;
        }
    }

    Colour textColor = pSettings->oscSendExtMasterFlag ? (targetCount > 0 ? Colours::green : Colours::orangered) : Colours::grey;
    labelExternalOscInfo->setText((targetCount > 0 ? (String(targetCount) + " active targets") : "no active targets"), dontSendNotification);
    labelExternalOscInfo->setColour(Label::textColourId, textColor);

    double packetsPerSecond = targetCount / (pSettings->oscSendExtIntervalMs * 0.001) + (pSettings->oscSendFlag ? (1.0 / (pSettings->oscSendIntervalMs * 0.001)) : 0.0);
    String info = approximatelyEqual(packetsPerSecond, 0.0)
        ? "no network traffic"
        : ("estimated network traffic: " + String(!pSettings->oscSendExtContinuousFlag ? "max. " : "") + (packetsPerSecond < 1.0
                                            ? "<1/s"
                                            : String(roundToInt(packetsPerSecond)) + "/s"));
    labelLoadInfo->setText(info, dontSendNotification);
}

void OSCSettingsComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    controlDimming();
    sendChangeMessage();
}
