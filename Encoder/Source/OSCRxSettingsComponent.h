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
#include "CustomOscInputTableListModel.h"
#include "CustomOscRxPresetHelper.h"
#include "OSCHandlerEncoder.h"
#include "../../Common/StatusMessageHandler.h"
#include "../../Common/OSCLogDialogManager.h"
#include "../../Common/PresetManagerDialog.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OSCRxSettingsComponent  : public Component,
                                public ActionListener,
                                public juce::Button::Listener,
                                public juce::Slider::Listener
{
public:
    //==============================================================================
    OSCRxSettingsComponent (EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, CustomOscRxPresetHelper* pCustomOscRxPresetHelper, OSCLogDialogManager* pOscLogManager, OSCHandlerEncoder* pOscHandler);
    ~OSCRxSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void controlDimming();
    void actionListenerCallback(const String& message) override;

    EncoderSettings* pSettings;
    StatusMessageHandler* pStatusMessageHandler;
    OSCLogDialogManager* pOscLogManager;
    CustomOscRxPresetHelper* pCustomOscRxPresetHelper;
    OSCHandlerEncoder* pOscHandler;
    std::unique_ptr<CustomOscInputTableListModel> customOscTableModel;
    PresetManagerDialog presetManagerDialog;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ToggleButton> toggleReceiveOsc;
    std::unique_ptr<juce::Label> labelOscPort;
    std::unique_ptr<juce::Slider> sliderReceiveOscPort;
    std::unique_ptr<juce::GroupComponent> groupDefinitions;
    std::unique_ptr<TableListBox> customOscList;
    std::unique_ptr<juce::TextButton> btnAdd;
    std::unique_ptr<juce::TextButton> btnDelete;
    std::unique_ptr<juce::ImageButton> btnInfo;
    std::unique_ptr<juce::TextButton> buttonShowOscLog;
    std::unique_ptr<juce::ToggleButton> toggleHideWarnings;
    std::unique_ptr<juce::TextButton> btnManagePresets;
    std::unique_ptr<juce::ToggleButton> toggleReceiveStandardOsc;
    std::unique_ptr<juce::ImageButton> btnInfoStandardOsc;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCRxSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

