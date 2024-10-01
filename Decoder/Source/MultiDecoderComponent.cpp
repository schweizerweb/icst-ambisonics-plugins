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

#include "MultiDecoderComponent.h"

MultiDecoderComponent::MultiDecoderComponent (AmbiSettingsCollection* _pAmbiSettings, AmbiSpeakerSet* _pSpeakerSet, FilterPresetHelper* _pFilterPresetHelper, dsp::ProcessSpec* _pFilterSpecification, ChangeListener* pChangeListener, ChannelLayout* _pChannelLayout): pAmbiSettings(_pAmbiSettings), pSpeakerSet(_pSpeakerSet), pFilterPresetHelper(_pFilterPresetHelper), pFilterSpecification(_pFilterSpecification), pChannelLayout(_pChannelLayout)
{
    std::vector<FilterBankInfo*> filterInfo;
    std::vector<juce::Colour*> colorInfo;
    for(int i = 0; i < MAX_NB_OF_DECODER_SECTIONS; i++)
    {
        filterInfo.push_back(&pAmbiSettings->multiDecoderSections[i].filterInfo);
        colorInfo.push_back(&pAmbiSettings->multiDecoderSections[i].color);
    }

    filterCurve.reset (new IIRFilterGraph(filterInfo, pFilterSpecification, colorInfo));
    addAndMakeVisible (filterCurve.get());
    filterCurve->setName ("filterCurve");

    //filterCurve->setBounds (8, 40, 150, 24);

    sliderDecoderCount.reset (new juce::Slider ("sliderDecoderCount"));
    addAndMakeVisible (sliderDecoderCount.get());
    sliderDecoderCount->setRange (1, 3, 1);
    sliderDecoderCount->setSliderStyle (juce::Slider::IncDecButtons);
    sliderDecoderCount->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderDecoderCount->addListener (this);
    sliderDecoderCount->setValue(pAmbiSettings->getUsedDecoderCount());

    labelDecoderCount.reset (new juce::Label ("labelDecoderCount",
                                              TRANS ("# Decoders:")));
    addAndMakeVisible (labelDecoderCount.get());
    labelDecoderCount->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDecoderCount->setJustificationType (juce::Justification::centredLeft);
    labelDecoderCount->setEditable (false, false, false);
    labelDecoderCount->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDecoderCount->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    for (int i = 0; i < MAX_NB_OF_DECODER_SECTIONS; i++)
    {
        auto sectionControl = new DecoderSectionControlComponent(&pAmbiSettings->multiDecoderSections[i], pSpeakerSet, pFilterPresetHelper, pFilterSpecification, this, pChannelLayout);
        sectionControls.add(sectionControl);
        addAndMakeVisible(sectionControl);
    }

    addChangeListener(pChangeListener);

    controlDimming();
    setSize (600, 400);

}

MultiDecoderComponent::~MultiDecoderComponent()
{
    filterCurve = nullptr;
    sliderDecoderCount = nullptr;
    labelDecoderCount = nullptr;

    sectionControls.clear();
}

void MultiDecoderComponent::controlDimming()
{
    int nbSections = jmin(MAX_NB_OF_DECODER_SECTIONS, pAmbiSettings->getUsedDecoderCount());
    for (int i = 0; i < MAX_NB_OF_DECODER_SECTIONS; i++)
    {
        sectionControls[i]->setVisible(i < nbSections);
    }
}

void MultiDecoderComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    filterCurve->repaint();
    sendChangeMessage();
}


void MultiDecoderComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void MultiDecoderComponent::resized()
{
    int decoderSettingsWidth = 66;
    int top = 30;
    int border = 2;
    auto width = getWidth();
    auto height = getHeight();
    sliderDecoderCount->setBounds (getWidth() - 126, border, 126, 24);
    labelDecoderCount->setBounds (getWidth() - 147 - 80, border, 88, 24);

    int rightEnd = getWidth()-border;
    for (int i = pAmbiSettings->getUsedDecoderCount() - 1; i >= 0; i--)
    {
        rightEnd -= decoderSettingsWidth;
        sectionControls[i]->setBounds(rightEnd, top, decoderSettingsWidth, height - top - 2);
        rightEnd -= border;
    }


    filterCurve->setBounds(border, top, rightEnd - border, getHeight() - top - border);
}

void MultiDecoderComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderDecoderCount.get())
    {
        pAmbiSettings->setUsedDecoderCount(sliderDecoderCount->getValue());
        controlDimming();
        resized();
    }
}

void MultiDecoderComponent::buttonClicked(Button* btn)
{
    
}

