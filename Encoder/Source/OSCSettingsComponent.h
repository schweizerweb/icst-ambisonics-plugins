/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "EncoderSettings.h"
#include "../../Common/StatusMessageHandler.h"
#include "../../Common/OSCLogDialogManager.h"
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
    OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, OSCLogDialogManager* pOscLogManager);
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
    StatusMessageHandler* pStatusMessageHandler;
    OSCLogDialogManager* pOscLogManager;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupExternal;
    std::unique_ptr<juce::GroupComponent> groupInternal;
    std::unique_ptr<juce::ToggleButton> toggleReceiveOsc;
    std::unique_ptr<juce::Label> labelOscPort;
    std::unique_ptr<juce::ToggleButton> toggleSendOsc;
    std::unique_ptr<juce::TextEditor> textOscSendIp;
    std::unique_ptr<juce::Label> labelOscSendIp;
    std::unique_ptr<juce::Label> labelOscSendInterval;
    std::unique_ptr<juce::ToggleButton> toggleSendOscExt;
    std::unique_ptr<juce::Label> labelExternalOscInfo;
    std::unique_ptr<juce::Slider> sliderReceiveOscPort;
    std::unique_ptr<juce::Slider> sliderSendOscPort;
    std::unique_ptr<juce::Slider> sliderInterval;
    std::unique_ptr<juce::TextButton> buttonShowOscLog;
    std::unique_ptr<OSCTargetsComponent> oscTargets;
    std::unique_ptr<juce::Label> labelLoadInfo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

