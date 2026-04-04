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

#include "../../Common/FilterBankInfo.h"
#include "../../Common/FFTAnalyzer.h"
#include "../../Common/FilterColors.h"
#include "FilterSettingsComponent.h"

FilterSettingsComponent::FilterSettingsComponent (FilterBankInfo* _pFilterBankInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* _pPresetHelper, int _channelIndex)
    : pPresetHelper(_pPresetHelper), pFilterBankInfo(_pFilterBankInfo), channelIndex(_channelIndex)
{
    std::vector<FilterBankInfo*> filterInfo;
    filterInfo.push_back(pFilterBankInfo);

    for (int i = 0; i < MAX_FILTER_COUNT; i++)
    {
        auto filterControl = new SingleFilterSettingsComponent(pFilterBankInfo->get(i), pFilterSpecification, this, FilterColors::getColor(i));
        filterControls.add(filterControl);
        filterControl->setName("filter" + String(i));
        addAndMakeVisible(filterControl);
    }
    
    filterGraph.reset (new IIRFilterGraph (filterInfo, pFilterSpecification, std::vector<Colour*>(), &filterControls));
    addAndMakeVisible (filterGraph.get());
    filterGraph->setName ("filterGraph");

    comboBoxFilterPreset.reset (new juce::ComboBox ("comboBoxFilterPreset"));
    addAndMakeVisible (comboBoxFilterPreset.get());
    comboBoxFilterPreset->setEditableText (false);
    comboBoxFilterPreset->setJustificationType (juce::Justification::centredLeft);
    comboBoxFilterPreset->setTextWhenNothingSelected (TRANS("-"));
    comboBoxFilterPreset->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxFilterPreset->addListener (this);

    labelPresets.reset (new juce::Label ("labelPresets",
                                         TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelPresets->setJustificationType (juce::Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPresets->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPresets->setBounds (0, 8, 64, 24);

    buttonSave.reset (new juce::TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    toggleBypass.reset (new juce::ToggleButton ("toggleBypass"));
    addAndMakeVisible (toggleBypass.get());
    toggleBypass->setButtonText (TRANS("Bypass filter"));
    toggleBypass->addListener (this);

    toggleFFT.reset (new juce::ToggleButton ("toggleFFT"));
    addAndMakeVisible (toggleFFT.get());
    toggleFFT->setButtonText (TRANS("FFT"));
    toggleFFT->addListener (this);

    sliderFFTScaler.reset (new juce::Slider ("sliderFFTScaler"));
    addAndMakeVisible (sliderFFTScaler.get());
    sliderFFTScaler->setRange (0, 50, 1);
    sliderFFTScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderFFTScaler->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 30, 20);
    sliderFFTScaler->addListener (this);

    labelFFTScaler.reset (new juce::Label ("labelFFTScaler",
                                           TRANS("Scaler [dB]:")));
    addAndMakeVisible (labelFFTScaler.get());
    labelFFTScaler->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelFFTScaler->setJustificationType (juce::Justification::centredLeft);
    labelFFTScaler->setEditable (false, false, false);
    labelFFTScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelFFTScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    setSize (800, 500);

    labelFFTScaler->setVisible(false);
    sliderFFTScaler->setVisible(false);

	addChangeListener(pChangeListener);

    updatePresetComboBox();
    if (pPresetHelper != nullptr)
    {
        pPresetHelper->addActionListener(this);
    }

    sliderFFTScaler->setValue(INITIAL_FFT_SCALER);
    toggleBypass->setToggleState(pFilterBankInfo->getFilterBypass(), dontSendNotification);
    filterGraph->rebuildHandles();
}

FilterSettingsComponent::~FilterSettingsComponent()
{
    if (pPresetHelper != nullptr)
    {
        pPresetHelper->removeActionListener(this);
    }

    stopTimer();
    FFTAnalyzer::getInstance()->disable();

    filterGraph = nullptr;
    comboBoxFilterPreset = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    filterControls.clear();
    toggleBypass = nullptr;
    toggleFFT = nullptr;
    sliderFFTScaler = nullptr;
    labelFFTScaler = nullptr;
}

void FilterSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void FilterSettingsComponent::resized()
{
    filterGraph->setBounds (0, 40, getWidth() - 0, getHeight() - 180);
    comboBoxFilterPreset->setBounds (72, 8, getWidth() - 167, 24);
    buttonSave->setBounds (getWidth() - 7 - 80, 8, 80, 24);
    for (int i = 0; i < filterControls.size(); i++)
    {
        filterControls[i]->setBounds(proportionOfWidth(1.0f/filterControls.size()*i), getHeight() - 140, proportionOfWidth(1.0f/filterControls.size()), 140);
    }

    toggleBypass->setBounds (((getWidth() - 170) + 0 - 85) + 0 - 58 - 128, 42, 128, 24);
    toggleFFT->setBounds (((getWidth() - 170) + 0 - 85) + 0 - 58, 42, 58, 24);
    sliderFFTScaler->setBounds (getWidth() - 170, 42, 170, 24);
    labelFFTScaler->setBounds ((getWidth() - 170) + 0 - 85, 42, 85, 24);
}

void FilterSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == comboBoxFilterPreset.get())
    {
        String presetName = comboBoxFilterPreset->getText();
        pPresetHelper->selectPresetName(presetName);
        comboBoxFilterPreset->setSelectedItemIndex(-1);
    }

    filterGraph->rebuildHandles();
	filterGraph->repaint();
}

void FilterSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == buttonSave.get())
    {
        pPresetHelper->tryCreateNewPreset([&](File* newFile) {
            if (newFile != nullptr)
            {
                pPresetHelper->writeToXmlFile(*newFile, pFilterBankInfo);
                comboBoxFilterPreset->setText("", dontSendNotification);
            }
        });
    }
    else if (buttonThatWasClicked == toggleBypass.get())
    {
        pFilterBankInfo->setFilterBypass(toggleBypass->getToggleState());
        sendChangeMessage();
    }
    else if (buttonThatWasClicked == toggleFFT.get())
    {
        if(toggleFFT->getToggleState())
        {
            filterGraph->setFFTParams(true, sliderFFTScaler->getValue());
            FFTAnalyzer::getInstance()->setActive(channelIndex);
            startTimer(50);
        }
        else
        {
            stopTimer();
            FFTAnalyzer::getInstance()->disable();
            filterGraph->setFFTParams(false);
        }

        labelFFTScaler->setVisible(toggleFFT->getToggleState());
        sliderFFTScaler->setVisible(toggleFFT->getToggleState());
    }
}

void FilterSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderFFTScaler.get())
    {
        filterGraph->setFFTParams(true, sliderFFTScaler->getValue());
    }
}

void FilterSettingsComponent::updatePresetComboBox()
{
    comboBoxFilterPreset->clear();
    int i = 1;
    for (File file : pPresetHelper->presetFiles)
    {
        comboBoxFilterPreset->addItem(file.getFileNameWithoutExtension(), i++);
    }
}

void FilterSettingsComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        updatePresetComboBox();
    }
    else if(message.startsWith(pPresetHelper->UniqueActionMessageSelectPreset()))
    {
        File presetFile(message.substring(pPresetHelper->UniqueActionMessageSelectPreset().length()));
        pPresetHelper->loadFromXmlFile(presetFile, pFilterBankInfo);
        pPresetHelper->notifyPresetChanged();
        
        for (int i = 0; i < filterControls.size(); i++)
        {
            filterControls[i]->updateUi();
        }
    }
}

void FilterSettingsComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    filterGraph->rebuildHandles();
    filterGraph->repaint();
    sendChangeMessage();
}

void FilterSettingsComponent::timerCallback()
{
    float scope[SCOPE_SIZE];
    int fftSize;
    if (FFTAnalyzer::getInstance()->scopeRequest(&scope[0], &fftSize))
    {
        filterGraph->setFFTResult(&scope[0], SCOPE_SIZE, fftSize);
    }
}
