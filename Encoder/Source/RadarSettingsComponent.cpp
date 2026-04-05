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

#include "RadarSettingsComponent.h"

RadarSettingsComponent::RadarSettingsComponent (ChangeListener* pChangeListener, ZoomSettings* _pZoomSettings)
    : pZoomSettings(_pZoomSettings)
{
    addChangeListener(pChangeListener);
    pZoomSettings->addChangeListener(this);

    groupZoom.reset (new juce::GroupComponent ("groupZoom",
                                               TRANS("Zoom")));
    addAndMakeVisible (groupZoom.get());

    sliderCenterPointX.reset (new juce::Slider ("sliderCenterPointX"));
    addAndMakeVisible (sliderCenterPointX.get());
    sliderCenterPointX->setRange (-1, 1, 0.01);
    sliderCenterPointX->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterPointX->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderCenterPointX->addListener (this);

    labelCenterPointX.reset (new juce::Label ("labelCenterPointX",
                                              TRANS("Center X")));
    addAndMakeVisible (labelCenterPointX.get());
    labelCenterPointX->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelCenterPointX->setJustificationType (juce::Justification::centredLeft);
    labelCenterPointX->setEditable (false, false, false);
    labelCenterPointX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterPointX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderCenterPointY.reset (new juce::Slider ("sliderCenterPointY"));
    addAndMakeVisible (sliderCenterPointY.get());
    sliderCenterPointY->setRange (-1, 1, 0.01);
    sliderCenterPointY->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterPointY->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderCenterPointY->addListener (this);

    labelCenterPointY.reset (new juce::Label ("labelCenterPointY",
                                              TRANS("Center Y")));
    addAndMakeVisible (labelCenterPointY.get());
    labelCenterPointY->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelCenterPointY->setJustificationType (juce::Justification::centredLeft);
    labelCenterPointY->setEditable (false, false, false);
    labelCenterPointY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterPointY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderCenterPointZ.reset (new juce::Slider ("sliderCenterPointZ"));
    addAndMakeVisible (sliderCenterPointZ.get());
    sliderCenterPointZ->setRange (-1, 1, 0.01);
    sliderCenterPointZ->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterPointZ->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderCenterPointZ->addListener (this);

    labelCenterPointZ.reset (new juce::Label ("labelCenterPointZ",
                                              TRANS("Center Z")));
    addAndMakeVisible (labelCenterPointZ.get());
    labelCenterPointZ->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelCenterPointZ->setJustificationType (juce::Justification::centredLeft);
    labelCenterPointZ->setEditable (false, false, false);
    labelCenterPointZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterPointZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    groupDisplay.reset (new juce::GroupComponent ("groupDisplay",
                                                  TRANS("Display")));
    addAndMakeVisible (groupDisplay.get());

    sliderPointScaler.reset (new juce::Slider ("sliderPointScaler"));
    addAndMakeVisible (sliderPointScaler.get());
    sliderPointScaler->setRange (0.2, 4, 0.01);
    sliderPointScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderPointScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderPointScaler->addListener (this);

    labelPointScaler.reset (new juce::Label ("labelPointScaler",
                                             TRANS("Point Scaler:")));
    addAndMakeVisible (labelPointScaler.get());
    labelPointScaler->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelPointScaler->setJustificationType (juce::Justification::centredLeft);
    labelPointScaler->setEditable (false, false, false);
    labelPointScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPointScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleLabelInPoint.reset(new juce::ToggleButton("toggleLabelInPoint"));
    addAndMakeVisible(toggleLabelInPoint.get());
    toggleLabelInPoint->setButtonText(TRANS("Label in Point"));
    toggleLabelInPoint->addListener(this);

    toggleLabelInPoint->setBounds(8, 10, 150, 24);

    sliderRadius.reset (new juce::Slider ("sliderRadius"));
    addAndMakeVisible (sliderRadius.get());
    sliderRadius->setRange (0.01, 100000, 0.01);
    sliderRadius->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderRadius->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderRadius->addListener (this);

    labelRadius.reset (new juce::Label ("labelRadius",
                                        TRANS("Radius")));
    addAndMakeVisible (labelRadius.get());
    labelRadius->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRadius->setJustificationType (juce::Justification::centredLeft);
    labelRadius->setEditable (false, false, false);
    labelRadius->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRadius->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnReset.reset (new juce::TextButton ("btnReset"));
    addAndMakeVisible (btnReset.get());
    btnReset->setButtonText (TRANS("Reset"));
    btnReset->addListener (this);

    sliderGroupPointScaler.reset (new juce::Slider ("sliderGroupPointScaler"));
    addAndMakeVisible (sliderGroupPointScaler.get());
    sliderGroupPointScaler->setRange (0.2, 4, 0.01);
    sliderGroupPointScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderGroupPointScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderGroupPointScaler->addListener (this);

    labelGroupPointScaler.reset (new juce::Label ("labelGroupPointScaler",
                                                  TRANS("Group Point Scaler:")));
    addAndMakeVisible (labelGroupPointScaler.get());
    labelGroupPointScaler->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelGroupPointScaler->setJustificationType (juce::Justification::centredLeft);
    labelGroupPointScaler->setEditable (false, false, false);
    labelGroupPointScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGroupPointScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    setSize (600, 400);

    sliderPointScaler->setSkewFactorFromMidPoint(1.0);
    sliderGroupPointScaler->setSkewFactorFromMidPoint(1.0);
    sliderRadius->setSkewFactorFromMidPoint(100.0);
    displaySettings();
}

RadarSettingsComponent::~RadarSettingsComponent()
{
    pZoomSettings->removeChangeListener(this);

    groupZoom = nullptr;
    sliderCenterPointX = nullptr;
    labelCenterPointX = nullptr;
    sliderCenterPointY = nullptr;
    labelCenterPointY = nullptr;
    sliderCenterPointZ = nullptr;
    labelCenterPointZ = nullptr;
    groupDisplay = nullptr;
    sliderPointScaler = nullptr;
    labelPointScaler = nullptr;
    sliderRadius = nullptr;
    labelRadius = nullptr;
    btnReset = nullptr;
    sliderGroupPointScaler = nullptr;
    labelGroupPointScaler = nullptr;
    toggleLabelInPoint = nullptr;
}

void RadarSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void RadarSettingsComponent::resized()
{
    groupZoom->setBounds (8, 129, getWidth() - 16, 192);
    sliderCenterPointX->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 129 + 24, (getWidth() - 16) - 152, 24);
    labelCenterPointX->setBounds (8 + 16, 129 + 24, 128, 24);
    sliderCenterPointY->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 129 + 56, (getWidth() - 16) - 152, 24);
    labelCenterPointY->setBounds (8 + 16, 129 + 56, 128, 24);
    sliderCenterPointZ->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 129 + 88, (getWidth() - 16) - 152, 24);
    labelCenterPointZ->setBounds (8 + 16, 129 + 88, 128, 24);
    groupDisplay->setBounds (8, 8, getWidth() - 16, 115);
    sliderPointScaler->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 8 + 24, (getWidth() - 16) - 152, 24);
    labelPointScaler->setBounds (8 + 8, 8 + 24, 128, 24);
    sliderRadius->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 129 + 120, (getWidth() - 16) - 152, 24);
    labelRadius->setBounds (8 + 16, 129 + 120, 128, 24);
    btnReset->setBounds (8 + juce::roundToInt ((getWidth() - 16) * 0.5000f) - (150 / 2), 129 + 152, 150, 24);
    sliderGroupPointScaler->setBounds (8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 8 + 56, (getWidth() - 16) - 152, 24);
    labelGroupPointScaler->setBounds (8 + 8, 8 + 56, 128, 24);
    toggleLabelInPoint->setBounds(8 + (getWidth() - 16) - 8 - ((getWidth() - 16) - 152), 8 + 80, (getWidth() - 16) - 152, 24);
}

void RadarSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderCenterPointX.get())
    {
        pZoomSettings->setCurrentCenterPointXY((float)sliderCenterPointX->getValue(), (float)pZoomSettings->getCurrentCenterPoint().getY());
    }
    else if (sliderThatWasMoved == sliderCenterPointY.get())
    {
        pZoomSettings->setCurrentCenterPointXY((float)pZoomSettings->getCurrentCenterPoint().getX(), (float)sliderCenterPointY->getValue());
    }
    else if (sliderThatWasMoved == sliderCenterPointZ.get())
    {
        pZoomSettings->setCurrentCenterPointXZ(pZoomSettings->getCurrentCenterPoint().getX(), (float)sliderCenterPointZ->getValue());
    }
    else if (sliderThatWasMoved == sliderPointScaler.get())
    {
        pZoomSettings->setPointScaler(sliderPointScaler->getValue());
    }
    else if (sliderThatWasMoved == sliderRadius.get())
    {
        pZoomSettings->setCurrentRadius((float)sliderRadius->getValue());
    }
    else if (sliderThatWasMoved == sliderGroupPointScaler.get())
    {
        pZoomSettings->setGroupPointScaler(sliderGroupPointScaler->getValue());
    }
}

void RadarSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnReset.get())
    {
        pZoomSettings->Reset();
    }
    else if (buttonThatWasClicked == toggleLabelInPoint.get())
    {
        pZoomSettings->setLabelInPointFlag(buttonThatWasClicked->getToggleState());
    }
}

void RadarSettingsComponent::displaySettings()
{
    sliderPointScaler->setValue(pZoomSettings->getPointScaler(), dontSendNotification);
    sliderGroupPointScaler->setValue(pZoomSettings->getGroupPointScaler(), dontSendNotification);
    toggleLabelInPoint->setToggleState(pZoomSettings->getLabelInPointFlag(), dontSendNotification);
    sliderCenterPointX->setValue(pZoomSettings->getCurrentCenterPoint().getX(), dontSendNotification);
    sliderCenterPointY->setValue(pZoomSettings->getCurrentCenterPoint().getY(), dontSendNotification);
    sliderCenterPointZ->setValue(pZoomSettings->getCurrentCenterPoint().getZ(), dontSendNotification);
    sliderRadius->setValue(pZoomSettings->getCurrentRadius(), dontSendNotification);
}

void RadarSettingsComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if(source != this)
    {
        displaySettings();
    }
}
