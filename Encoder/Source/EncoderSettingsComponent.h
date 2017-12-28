/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "EncoderSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EncoderSettingsComponent  : public Component,
                                  public TextEditorListener,
                                  public ButtonListener
{
public:
    //==============================================================================
    EncoderSettingsComponent (EncoderSettings* pSettings);
    ~EncoderSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	static void showAsDialog(EncoderSettings* encoder_settings);
	void checkForNumbers(TextEditor* pEditor, int* pParameter) const;
	void textEditorTextChanged(TextEditor&) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	EncoderSettings* pEncoderSettings;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> groupOscReceive;
    ScopedPointer<ToggleButton> toggleReceiveOsc;
    ScopedPointer<TextEditor> textOscReceivePort;
    ScopedPointer<Label> labelOscPort;
    ScopedPointer<GroupComponent> groupOscSend;
    ScopedPointer<ToggleButton> toggleSendOsc;
    ScopedPointer<TextEditor> textOscSendIp;
    ScopedPointer<Label> labelOscSendIp;
    ScopedPointer<TextEditor> textOscSendPort;
    ScopedPointer<Label> labelOscSendPort;
    ScopedPointer<TextEditor> textOscSendInterval;
    ScopedPointer<Label> labelOscSendInterval;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncoderSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
