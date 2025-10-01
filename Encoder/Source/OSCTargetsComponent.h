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
#include <JuceHeader.h>
#include "EncoderSettings.h"
#include "CustomOscTableListModel.h"
#include "CustomOscTxPresetHelper.h"
#include "../../Common/PresetManagerDialog.h"

#define COMMON_OSC_INFO_STRING "Allowed objects:\n{x}, {y}, {z} = Real X/Y/Z-Coordinates\n{a}, {e}, {d} = Real A/E/D-Coordinates\n{sx}, {sy}, {sz} = Scaled X/Y/Z-Coordinates (0..1)\n{sa}, {se}, {sd} = Scaled A/E/D-Coordinates (0..1)\n{i} = index\n{n} = name\n{g} = gain\n{c} = color\n\nUser defined scaling available for sx, sy, sz, sa, se, sd:\nSyntax: {s*,[lowLimit],[highLimit]}\nExample: {sx,-0.5,0.5}\n\nDual-Scaling for sx, sy, sz, se:\nSyntax: {s*,[lowLimit],[zeroValue],[highLimit]}\nExample: {sz, 1.0, 0.1, 1.0}\n\nConstant values:\n{ci,[int]} = Constant integer value\n{cf,[float]} = Constant float value\n{cs,[string]} = Constant string"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OSCTargetsComponent  : public juce::Component,
                             public ChangeBroadcaster,
                             public TextEditor::Listener,
                             public ActionListener,
                             public juce::Slider::Listener,
                             public juce::Button::Listener
{
public:
    //==============================================================================
    OSCTargetsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, CustomOscTxPresetHelper* pCustomOscTxPresetHelper);
    ~OSCTargetsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void textEditorTextChanged(TextEditor&) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void controlDimming();
    void actionListenerCallback(const String& message) override;
    EncoderSettings* pSettings;
    std::unique_ptr<CustomOscTableListModel> customOscTableModel;
    CustomOscTxPresetHelper* pCustomOscTxPresetHelper;
    PresetManagerDialog presetManagerDialog;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupTargets;
    std::unique_ptr<TableListBox> targetList;
    std::unique_ptr<juce::Slider> sliderInterval;
    std::unique_ptr<juce::Label> labelInterval;
    std::unique_ptr<juce::TextButton> btnAdd;
    std::unique_ptr<juce::TextButton> btnDelete;
    std::unique_ptr<juce::ImageButton> btnInfo;
    std::unique_ptr<juce::ToggleButton> toggleSendContinuous;
    std::unique_ptr<juce::TextButton> btnManagePresets;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCTargetsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
