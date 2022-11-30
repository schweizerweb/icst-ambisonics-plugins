/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "EncoderSettings.h"
#include "CustomOscInputTableListModel.h"
#include "CustomOscRxPresetHelper.h"
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
                                public ChangeBroadcaster,
                                public ActionListener,
                                public juce::Button::Listener,
                                public juce::Slider::Listener
{
public:
    //==============================================================================
    OSCRxSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, CustomOscRxPresetHelper* pCustomOscRxPresetHelper, OSCLogDialogManager* pOscLogManager);
    ~OSCRxSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* help_png;
    static const int help_pngSize;
    static const char* save_png;
    static const int save_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void controlDimming();
    void actionListenerCallback(const String& message) override;

    EncoderSettings* pSettings;
    StatusMessageHandler* pStatusMessageHandler;
    OSCLogDialogManager* pOscLogManager;
    CustomOscRxPresetHelper* pCustomOscRxPresetHelper;
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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCRxSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

