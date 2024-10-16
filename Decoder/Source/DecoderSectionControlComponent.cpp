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

#include "DecoderSectionControlComponent.h"
#include "FilterSettingsComponent.h"
#include "AmbiSettingsComponent.h"
#include "../../Common/SpeakerSelectionComponent.h"

DecoderSectionControlComponent::DecoderSectionControlComponent(AmbiSettingsSection* _pAmbiSettings, AmbiSpeakerSet* _pSpeakerSet, FilterPresetHelper* _pFilterPresetHelper, dsp::ProcessSpec* _pFilterSpecification, ChangeListener* _pChangeListener, ChannelLayout* _pChannelLayout, int _decoderIndex): pAmbiSettings(_pAmbiSettings), pSpeakerSet(_pSpeakerSet), pFilterPresetHelper(_pFilterPresetHelper), pFilterSpecification(_pFilterSpecification), pChannelLayout(_pChannelLayout), decoderIndex(_decoderIndex)
{
    gainSlider.reset(new juce::Slider());
    addAndMakeVisible(gainSlider.get());
    gainSlider->setRange(Constants::GainDbMin, Constants::GainDbMax, 0.5);
    gainSlider->setSliderStyle(juce::Slider::LinearBarVertical);
    gainSlider->setTextValueSuffix(" dB");
    gainSlider->addListener(this);
    
    muteButton.reset(new juce::TextButton("btnMute", "Mute"));
    addAndMakeVisible(muteButton.get());
    muteButton->setClickingTogglesState(true);
    muteButton->setButtonText("M");
    muteButton->setColour(TextButton::ColourIds::buttonOnColourId, Colours::red);
    muteButton->addListener(this);
    
    colorField.reset(new ColorEditorCustomComponent(*this, true));
    addAndMakeVisible(colorField.get());
    colorField->setRowAndColumn(0, 0);

    pointsButton.reset(new juce::TextButton("btnPoints", "P"));
    addAndMakeVisible(pointsButton.get());
    pointsButton->addListener(this);

    orderButton.reset(new juce::TextButton("btnOrder", "O"));
    addAndMakeVisible(orderButton.get());
    orderButton->addListener(this);

    weightingButton.reset(new juce::TextButton("btnWeighting", "W"));
    addAndMakeVisible(weightingButton.get());
    weightingButton->setButtonText("W");
    weightingButton->addListener(this);

    filterButton.reset(new juce::TextButton("btnFilter", "F"));
    addAndMakeVisible(filterButton.get());
    filterButton->addListener(this);

    setLookAndFeel(&ambiLookAndFeel);
    
    addChangeListener(_pChangeListener);

    updateUI();
}

DecoderSectionControlComponent::~DecoderSectionControlComponent()
{
    setLookAndFeel(nullptr);

    gainSlider = nullptr;
    muteButton = nullptr;
    colorField = nullptr;

    pointsButton = nullptr;
    orderButton = nullptr;
    weightingButton = nullptr;
    filterButton = nullptr;
}

void DecoderSectionControlComponent::controlDimming()
{
    gainSlider->setColour(juce::Slider::ColourIds::thumbColourId, pAmbiSettings->color);
    gainSlider->repaint();

    auto speakerCount = 0;
    for (int i = 0; i < pSpeakerSet->size(); i++)
    {
        if (pSpeakerSet->get(i)->getEnabled() && (pAmbiSettings->speakerMask & (static_cast<unsigned long long>(1) << (i))))
        {
            speakerCount++;
        }
    }

    pointsButton->setButtonText(String(speakerCount) + " S");
    pointsButton->setTooltip(String(speakerCount) + " speakers selected");

    orderButton->setButtonText("O " + String(pAmbiSettings->ambiSettings.getAmbiOrder()));
    orderButton->setTooltip("Order " + String(pAmbiSettings->ambiSettings.getAmbiOrder()));

    auto mode = pAmbiSettings->ambiSettings.getWeightMode();
    weightingButton->setButtonText(mode == AmbiSettings::BASIC ? "Bas" : (mode == AmbiSettings::INPHASE ? "inP" : (mode == AmbiSettings::MAXRE ? "mRe" : "Man")));
    weightingButton->setTooltip("Weighting: " + String(mode == AmbiSettings::BASIC ? "Basic" : (mode == AmbiSettings::INPHASE ? "inPhase" : (mode == AmbiSettings::MAXRE ? "maxRe" : "Manual"))));

    filterButton->setButtonText("F");
    filterButton->setTooltip("Filter");
}

double DecoderSectionControlComponent::getValue(int /*columnId*/, int /*rowNumber*/)
{
    return double(uint32(pAmbiSettings->color.getARGB()));
}

void DecoderSectionControlComponent::setValue(int /*columnId*/, int /*rowNumber*/, double newValue)
{
    if (newValue == -1)
    {
        for (int i = 0; i < pSpeakerSet->size(); i++)
        {
            if (pAmbiSettings->speakerMask & (static_cast<unsigned long long>(1) << (i)))
            {
                AmbiSpeaker* p = pSpeakerSet->get(i);
                if (p != nullptr)
                {
                    p->setColor(pAmbiSettings->color);
                }
            }
        }
    }
    else
    {
        pAmbiSettings->color = Colour(uint32(newValue));
        colorField->setRowAndColumn(0, 0);
        controlDimming();
        repaint();
    }

    sendChangeMessage();
}

// unused inherited methods
SliderRange DecoderSectionControlComponent::getSliderRange(int /*columnId*/) { return SliderRange(); }
TableListBox* DecoderSectionControlComponent::getTable() { return nullptr; }
String DecoderSectionControlComponent::getTableText(const int /*columnId*/, const int /*rowNumber*/) { return String(); }
void DecoderSectionControlComponent::setTableText(const int /*columnId*/, const int /*rowNumber*/, const String& /*newText*/) { }
bool DecoderSectionControlComponent::getEnabled(const int /*columnId*/, const int /*rowNumber*/) { return true; }

void DecoderSectionControlComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    controlDimming();
    sendChangeMessage();
}


void DecoderSectionControlComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
    g.setColour(pAmbiSettings->color);
    g.drawRoundedRectangle(1.0f, 1.0f, getWidth() - 2.0f, getHeight() - 2.0f, 4.0f, 2.0f);
}

void DecoderSectionControlComponent::resized()
{
    auto width = getWidth();
    auto height = getHeight();
    auto secondColumnStart = width / 2;
    auto secondColumnWidth = width - width / 2 - 2;
    
    gainSlider->setBounds(2, 2, secondColumnStart - 4, height - 24);
    pointsButton->setBounds(secondColumnStart, 2, secondColumnWidth, 20);
    orderButton->setBounds(secondColumnStart, 22, secondColumnWidth, 20);
    weightingButton->setBounds(secondColumnStart, 42, secondColumnWidth, 20);
    filterButton->setBounds(secondColumnStart, 62, secondColumnWidth, 20);
    muteButton->setBounds(2, height - 20, secondColumnStart - 4, 18);
    colorField->setBounds(secondColumnStart, height - 20, secondColumnWidth, 18);
}

void DecoderSectionControlComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == gainSlider.get())
    {
        pAmbiSettings->gain = Decibels::decibelsToGain(sliderThatWasMoved->getValue());
    }
}

void DecoderSectionControlComponent::buttonClicked(Button* btn)
{
    if (btn == muteButton.get())
    {
        pAmbiSettings->mute = btn->getToggleState();
    }
    else if (btn == pointsButton.get())
    {
        CallOutBox::launchAsynchronously(std::make_unique<SpeakerSelectionComponent>(pSpeakerSet, &(pAmbiSettings->speakerMask), this), getScreenBounds(), nullptr);
    }
    else if (btn == orderButton.get() || btn == weightingButton.get())
    {
        CallOutBox::launchAsynchronously(std::make_unique<AmbiSettingsComponent>(&(pAmbiSettings->ambiSettings), this, pChannelLayout), getScreenBounds(), nullptr);
    }
    else if (btn == filterButton.get())
    {
        CallOutBox::launchAsynchronously(std::make_unique<FilterSettingsComponent>(&(pAmbiSettings->filterInfo), pFilterSpecification, this, pFilterPresetHelper, FFT_INDEX_OFFSET_MULTIDECODER + decoderIndex), getScreenBounds(), nullptr);
    }

    controlDimming();
}

void DecoderSectionControlComponent::updateUI()
{
    gainSlider->setValue(Decibels::gainToDecibels(pAmbiSettings->gain));
    muteButton->setToggleState(pAmbiSettings->mute, dontSendNotification);
    colorField->setRowAndColumn(0, 0);
    controlDimming();
    resized();
}

