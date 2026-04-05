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

#include "ScalingComponent.h"

ScalingComponent::ScalingComponent (ChangeListener* pListener, AmbiSpeakerSet* _pSpeakerSet, ZoomSettings* _pZoomSettings)
    : pSpeakerSet(_pSpeakerSet), pZoomSettings(_pZoomSettings)
{
    addChangeListener(pListener);

    labelRoomSize.reset (new juce::Label ("new label",
                                          TRANS("Room size")));
    addAndMakeVisible (labelRoomSize.get());
    labelRoomSize->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRoomSize->setJustificationType (juce::Justification::centredLeft);
    labelRoomSize->setEditable (false, false, false);
    labelRoomSize->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRoomSize->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelRoomSize->setBounds (0, 0, 80, 24);

    sliderRoomSizeX.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeX.get());
    sliderRoomSizeX->setRange (0.1, 1000, 0.1);
    sliderRoomSizeX->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeX->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeX->addListener (this);

    sliderRoomSizeX->setBounds (24, 24, 120, 24);

    sliderRoomSizeY.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeY.get());
    sliderRoomSizeY->setRange (0.1, 1000, 0.1);
    sliderRoomSizeY->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeY->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeY->addListener (this);

    sliderRoomSizeY->setBounds (24, 48, 120, 24);

    labelX.reset (new juce::Label ("new label",
                                   TRANS("X:")));
    addAndMakeVisible (labelX.get());
    labelX->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelX->setJustificationType (juce::Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelX->setBounds (0, 24, 24, 24);

    labelY.reset (new juce::Label ("new label",
                                   TRANS("Y:")));
    addAndMakeVisible (labelY.get());
    labelY->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelY->setJustificationType (juce::Justification::centredLeft);
    labelY->setEditable (false, false, false);
    labelY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelY->setBounds (0, 48, 24, 24);

    sliderRoomSizeZ.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRoomSizeZ.get());
    sliderRoomSizeZ->setRange (0.1, 1000, 0.1);
    sliderRoomSizeZ->setSliderStyle (juce::Slider::IncDecButtons);
    sliderRoomSizeZ->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRoomSizeZ->addListener (this);

    sliderRoomSizeZ->setBounds (24, 72, 120, 24);

    labelZ.reset (new juce::Label ("new label",
                                   TRANS("Z:")));
    addAndMakeVisible (labelZ.get());
    labelZ->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelZ->setJustificationType (juce::Justification::centredLeft);
    labelZ->setEditable (false, false, false);
    labelZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelZ->setBounds (0, 72, 24, 24);

    setSize (150, 100);

    calculateAndDisplayRoomSize();
    sliderRoomSizeX->setNumDecimalPlacesToDisplay(2);
    sliderRoomSizeY->setNumDecimalPlacesToDisplay(2);
    sliderRoomSizeZ->setNumDecimalPlacesToDisplay(2);
}

ScalingComponent::~ScalingComponent()
{
    labelRoomSize = nullptr;
    sliderRoomSizeX = nullptr;
    sliderRoomSizeY = nullptr;
    labelX = nullptr;
    labelY = nullptr;
    sliderRoomSizeZ = nullptr;
    labelZ = nullptr;
}

void ScalingComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void ScalingComponent::resized()
{
}

void ScalingComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderRoomSizeX.get())
    {
        if(currentRoomSizeX > 0.0)
        {
            for(int i = 0; i < pSpeakerSet->size(); i++)
                pSpeakerSet->get(i)->getRawPoint()->setX(pSpeakerSet->get(i)->getRawPoint()->getX() / currentRoomSizeX * sliderRoomSizeX->getValue());

            currentRoomSizeX = sliderRoomSizeX->getValue();
        }
    }
    else if (sliderThatWasMoved == sliderRoomSizeY.get())
    {
        if(currentRoomSizeY > 0.0)
        {
            for(int i = 0; i < pSpeakerSet->size(); i++)
                pSpeakerSet->get(i)->getRawPoint()->setY(pSpeakerSet->get(i)->getRawPoint()->getY() / currentRoomSizeY * sliderRoomSizeY->getValue());

            currentRoomSizeY = sliderRoomSizeY->getValue();
        }
    }
    else if (sliderThatWasMoved == sliderRoomSizeZ.get())
    {
        if(currentRoomSizeZ > 0.0)
        {
            for(int i = 0; i < pSpeakerSet->size(); i++)
                pSpeakerSet->get(i)->getRawPoint()->setZ(pSpeakerSet->get(i)->getRawPoint()->getZ() / currentRoomSizeZ * sliderRoomSizeZ->getValue());

            currentRoomSizeZ = sliderRoomSizeZ->getValue();
        }
    }

    sendChangeMessage();
    pZoomSettings->Reset(pSpeakerSet);
}

void ScalingComponent::calculateAndDisplayRoomSize()
{
    double maxX = 0.0, maxY = 0.0, maxZ = 0.0;
    for(int i = 0; i < pSpeakerSet->size(); i++)
    {
        maxX = jmax(maxX, fabs(pSpeakerSet->get(i)->getRawPoint()->getX()));
        maxY = jmax(maxY, fabs(pSpeakerSet->get(i)->getRawPoint()->getY()));
        maxZ = jmax(maxZ, fabs(pSpeakerSet->get(i)->getRawPoint()->getZ()));
    }

    currentRoomSizeX = maxX * 2.0;
    currentRoomSizeY = maxY * 2.0;
    currentRoomSizeZ = maxZ * 2.0;

    sliderRoomSizeX->setValue(currentRoomSizeX, dontSendNotification);
    sliderRoomSizeY->setValue(currentRoomSizeY, dontSendNotification);
    sliderRoomSizeZ->setValue(currentRoomSizeZ, dontSendNotification);

    sliderRoomSizeX->setEnabled(currentRoomSizeX > 0.0);
    sliderRoomSizeY->setEnabled(currentRoomSizeY > 0.0);
    sliderRoomSizeZ->setEnabled(currentRoomSizeZ > 0.0);
}
