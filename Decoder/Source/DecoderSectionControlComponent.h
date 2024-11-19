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

#pragma once

#include <JuceHeader.h>
#include "IIRFilterGraph.h"
#include "FilterPresetHelper.h"
#include "../../Common/AmbiSettingsCollection.h"
#include "../../Common/ColorEditorCustomComponent.h"
#include "../../Common/AmbiLookAndFeel.h"
#include "../../Common/AmbiSpeakerSet.h"
#include "../../Common/ChannelLayout.h"

class DecoderSectionControlComponent  : public juce::Component,
                               public juce::Slider::Listener,
                               public juce::ToggleButton::Listener,
                               public juce::TextEditor::Listener,
                               public TableColumnCallback,
                               public ChangeListener,
                               public ChangeBroadcaster
{
public:
    DecoderSectionControlComponent(AmbiSettingsSection* pSection, AmbiSpeakerSet* _pSpeakerSet, FilterPresetHelper* _pFilterPresetHelper, dsp::ProcessSpec* pFilterSpecification, ChangeListener* _pChangeListener, ChannelLayout* _pChannelLayout, int _decoderIndex);
    ~DecoderSectionControlComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked(Button*) override;
    void updateUI();

private:
    void controlDimming();
    AmbiLookAndFeel ambiLookAndFeel;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::TextEditor> nameEditor;
    std::unique_ptr<juce::TextButton> muteButton;
    std::unique_ptr<juce::TextButton> orderButton;
    std::unique_ptr<juce::TextButton> weightingButton;
    std::unique_ptr<juce::TextButton> pointsButton;
    std::unique_ptr<juce::TextButton> filterButton;
    std::unique_ptr<ColorEditorCustomComponent> colorField;

    AmbiSettingsSection* pAmbiSettings;
    AmbiSpeakerSet* pSpeakerSet;
    FilterPresetHelper* pFilterPresetHelper;
    dsp::ProcessSpec* pFilterSpecification;
    ChannelLayout* pChannelLayout;
    int decoderIndex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecoderSectionControlComponent)

        // Inherited via TableColumnCallback
        double getValue(int columnId, int rowNumber) override;
    void setValue(int columnId, int rowNumber, double newValue) override;
    SliderRange getSliderRange(int columnId) override;
    TableListBox* getTable() override;
    String getTableText(const int columnId, const int rowNumber) override;
    void setTableText(const int columnId, const int rowNumber, const String& newText) override;
    bool getEnabled(const int columnId, const int rowNumber) override;

    // Inherited via ChangeListener
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void textEditorTextChanged(TextEditor& editor) override;
};