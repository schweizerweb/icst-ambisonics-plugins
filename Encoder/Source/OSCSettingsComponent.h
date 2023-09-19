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
#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "OSCTargetsComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OSCSettingsComponent  : public Component,
                              public TextEditor::Listener,
                              public ChangeBroadcaster,
                              public ChangeListener,
                              public juce::Button::Listener,
                              public juce::Slider::Listener
{
public:
    //==============================================================================
    OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, CustomOscTxPresetHelper*  pCustomOscTxPresetHelper);
    ~OSCSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void textEditorTextChanged(TextEditor&) override;
    void controlDimming() const;
    void changeListenerCallback(ChangeBroadcaster* source) override;

    EncoderSettings* pSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupExternal;
    std::unique_ptr<juce::GroupComponent> groupInternal;
    std::unique_ptr<juce::ToggleButton> toggleSendOsc;
    std::unique_ptr<juce::TextEditor> textOscSendIp;
    std::unique_ptr<juce::Label> labelOscSendIp;
    std::unique_ptr<juce::Label> labelOscSendInterval;
    std::unique_ptr<juce::ToggleButton> toggleSendOscExt;
    std::unique_ptr<juce::Label> labelExternalOscInfo;
    std::unique_ptr<juce::Slider> sliderSendOscPort;
    std::unique_ptr<juce::Slider> sliderInterval;
    std::unique_ptr<OSCTargetsComponent> oscTargets;
    std::unique_ptr<juce::Label> labelLoadInfo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

