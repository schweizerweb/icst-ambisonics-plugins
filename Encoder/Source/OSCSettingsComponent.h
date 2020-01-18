/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "EncoderSettings.h"
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
                              public Button::Listener
{
public:
    //==============================================================================
    OSCSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings);
    ~OSCSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void textEditorTextChanged(TextEditor&) override;
    void controlDimming() const;
    void checkForNumbers(TextEditor* pEditor, int* pParameter) const;
    void checkForNumbers(TextEditor* pEditor, float* pParameter) const;

    EncoderSettings* pSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ToggleButton> toggleReceiveOsc;
    std::unique_ptr<TextEditor> textOscReceivePort;
    std::unique_ptr<Label> labelOscPort;
    std::unique_ptr<ToggleButton> toggleSendOsc;
    std::unique_ptr<TextEditor> textOscSendIp;
    std::unique_ptr<Label> labelOscSendIp;
    std::unique_ptr<TextEditor> textOscSendPort;
    std::unique_ptr<TextEditor> textOscSendInterval;
    std::unique_ptr<Label> labelOscSendInterval;
    std::unique_ptr<ToggleButton> toggleSendOscExt;
    std::unique_ptr<TextEditor> textOscSendIpExt;
    std::unique_ptr<Label> labelOscSendIpExt;
    std::unique_ptr<TextEditor> textOscSendPortExt;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

