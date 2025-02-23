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
    gainSlider->setRange(-Constants::GainDbMax, Constants::GainDbMax, 0.5);
    gainSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider->setTextValueSuffix(" dB");
    gainSlider->addListener(this);
    gainSlider->setDoubleClickReturnValue(true, 0.0);

    nameEditor.reset(new juce::TextEditor("editName"));
    addAndMakeVisible(nameEditor.get());
    nameEditor->setJustification(Justification::centredTop);
    nameEditor->addListener(this);

    colorField.reset(new ColorEditorCustomComponent(*this, true));
    addAndMakeVisible(colorField.get());
    colorField->setRowAndColumn(0, 0);

    muteButton.reset(new juce::TextButton("btnMute", "Mute"));
    addAndMakeVisible(muteButton.get());
    muteButton->setClickingTogglesState(true);
    muteButton->setButtonText("M");
    muteButton->setColour(TextButton::ColourIds::buttonOnColourId, Colours::red);
    muteButton->addListener(this);
    
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

    filterComponent.reset(new CheckBoxFilterCustomComponent(*this));
    addAndMakeVisible(filterComponent.get());

    setLookAndFeel(&ambiLookAndFeel);
    
    addChangeListener(_pChangeListener);

    updateUI();
}

DecoderSectionControlComponent::~DecoderSectionControlComponent()
{
    setLookAndFeel(nullptr);

    nameEditor = nullptr;
    gainSlider = nullptr;
    muteButton = nullptr;
    colorField = nullptr;

    pointsButton = nullptr;
    orderButton = nullptr;
    weightingButton = nullptr;
    filterComponent = nullptr;
}

void DecoderSectionControlComponent::controlDimming()
{
    auto speakerCount = 0;
    for (int i = 0; i < pSpeakerSet->size(); i++)
    {
        if (pSpeakerSet->get(i)->getEnabled() && (pAmbiSettings->speakerMask & (static_cast<unsigned long long>(1) << (i))))
        {
            speakerCount++;
        }
    }

    pointsButton->setButtonText(String(speakerCount) + " speakers");
    pointsButton->setTooltip(String(speakerCount) + " speakers selected");

    int order = pAmbiSettings->ambiSettings.getAmbiOrder();
    String orderString = String(order) + (order == 1 ? "st" : (order == 2 ? "nd" : "th")) + " order";
    orderButton->setButtonText(orderString);
    orderButton->setTooltip(orderString + " ambisonics");

    auto mode = pAmbiSettings->ambiSettings.getWeightMode();
    weightingButton->setButtonText(mode == AmbiSettings::BASIC ? "basic" : (mode == AmbiSettings::INPHASE ? "inPhase" : (mode == AmbiSettings::MAXRE ? "maxRe" : "manual")));
    weightingButton->setTooltip("Weighting: " + String(mode == AmbiSettings::BASIC ? "basic" : (mode == AmbiSettings::INPHASE ? "inPhase" : (mode == AmbiSettings::MAXRE ? "maxRe" : "manual"))));

    //int nbActiveFilters = pAmbiSettings->filterInfo.getFilterBypass() ? 0 : pAmbiSettings->filterInfo.filterCount();
    //filterButton->setTooltip(nbActiveFilters > 0 ? "Filter (" + String(nbActiveFilters) + " individual filters active)" : "No filters active");
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
        updateUI();
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
    sendChangeMessage();
    updateUI();
}

void DecoderSectionControlComponent::textEditorTextChanged(TextEditor& editor)
{
    if (&editor == nameEditor.get())
    {
        pAmbiSettings->name = editor.getText();
    }

    controlDimming();
}

bool DecoderSectionControlComponent::getBypass(int /*rowNumber*/)
{
    return pAmbiSettings->filterInfo.getFilterBypass();
}

void DecoderSectionControlComponent::setBypass(int /*rowNumber*/, bool newValue)
{
    pAmbiSettings->filterInfo.setFilterBypass(newValue);
    updateUI();
    sendChangeMessage();
}

FilterBankInfo* DecoderSectionControlComponent::getFilterInfo(int /*rowNumber*/)
{
    return &pAmbiSettings->filterInfo;
}

dsp::ProcessSpec* DecoderSectionControlComponent::getFilterSpecification()
{
    return pFilterSpecification;
}

void DecoderSectionControlComponent::showFilterEditor(int /*rowNumber*/, Rectangle<int> screenBounds)
{
    auto topComponent = getParentComponent()->getParentComponent();
    auto relPos = topComponent->getScreenPosition();
    CallOutBox::launchAsynchronously(std::make_unique<FilterSettingsComponent>(&(pAmbiSettings->filterInfo), pFilterSpecification, this, pFilterPresetHelper, FFT_INDEX_OFFSET_MULTIDECODER + decoderIndex), screenBounds.translated(-relPos.getX(), -relPos.getY()), topComponent);
}


void DecoderSectionControlComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
    g.setColour(pAmbiSettings->color);
    g.drawRoundedRectangle(1.0f, 1.0f, getWidth() - 2.0f, getHeight() - 2.0f, 4.0f, 2.0f);
}

void DecoderSectionControlComponent::resized()
{
    auto border = 2;
    auto width = getWidth();
    auto height = getHeight();
    auto standardWidth = width - 2 * border;
    auto standardHeight = 20;
    //auto secondColumnStart = width / 2;
    //auto secondColumnWidth = width - width / 2 - 2;
    
    nameEditor->setBounds(border, border, standardWidth, standardHeight + border);
    pointsButton->setBounds(border, standardHeight + 3 * border, standardWidth, standardHeight);
    orderButton->setBounds(border, 2 * standardHeight + 4 * border, standardWidth, standardHeight);
    weightingButton->setBounds(border, 3 * standardHeight + 5 * border, standardWidth, standardHeight);
    filterComponent->setBounds(border, 4 * standardHeight + 6 * border, standardWidth, standardHeight + border);
    gainSlider->setBounds(border, 5 * standardHeight + 7 * border, standardWidth, height - standardHeight - 5 * standardHeight - 8 * border);
    gainSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, width - 4 * border, standardHeight);
    muteButton->setBounds(border, height - standardHeight, width / 2 - 2 * border, standardHeight - border);
    colorField->setBounds(border, height - standardHeight - border, standardWidth, standardHeight);
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
    
    controlDimming();
}

void DecoderSectionControlComponent::updateUI()
{
    nameEditor->setText(pAmbiSettings->name);
    gainSlider->setValue(Decibels::gainToDecibels(pAmbiSettings->gain));
    gainSlider->setColour(juce::Slider::rotarySliderFillColourId, pAmbiSettings->color);
    gainSlider->repaint();
    muteButton->setToggleState(pAmbiSettings->mute, dontSendNotification);
    filterComponent->setRow(0, pAmbiSettings->color);
    colorField->setRowAndColumn(0, 0);
    controlDimming();
    resized();
    repaint();
}

