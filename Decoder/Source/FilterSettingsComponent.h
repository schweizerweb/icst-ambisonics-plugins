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

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "IIRFilterGraph.h"
#include "FilterPresetHelper.h"
#include "SingleFilterSettingsComponent.h"

#define INITIAL_FFT_SCALER 10.0
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FilterSettingsComponent  : public Component,
                                 public ChangeBroadcaster,
                                 public ActionListener,
                                 public ChangeListener,
                                 public Timer,
                                 public juce::ComboBox::Listener,
                                 public juce::Button::Listener,
                                 public juce::Slider::Listener
{
public:
    //==============================================================================
    FilterSettingsComponent (FilterBankInfo* pFilterBankInfo, dsp::ProcessSpec* pFilterSpecification, ChangeListener* pChangeListener, FilterPresetHelper* pPresetHelper, int channelIndex);
    ~FilterSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updatePresetComboBox();
    void actionListenerCallback(const String &message) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void timerCallback() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    FilterPresetHelper* pPresetHelper;
    FilterBankInfo* pFilterBankInfo;
    int channelIndex;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<IIRFilterGraph> filterGraph;
    std::unique_ptr<juce::ComboBox> comboBoxFilterPreset;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;
    OwnedArray<SingleFilterSettingsComponent> filterControls;
    std::unique_ptr<juce::ToggleButton> toggleFFT;
    std::unique_ptr<juce::ToggleButton> toggleBypass;
    std::unique_ptr<juce::Slider> sliderFFTScaler;
    std::unique_ptr<juce::Label> labelFFTScaler;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
