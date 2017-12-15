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
#include "DecoderSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DecoderSettingsComponent  : public Component,
                                  public TextEditorListener,
                                  public ButtonListener
{
public:
    //==============================================================================
    DecoderSettingsComponent (DecoderSettings* pSettings);
    ~DecoderSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	static void showAsDialog(DecoderSettings* pSettings);
	void textEditorTextChanged(TextEditor& editor) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	DecoderSettings * pDecoderSettings;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> groupOsc;
    ScopedPointer<ToggleButton> toggleReceiveOsc;
    ScopedPointer<TextEditor> textOscPort;
    ScopedPointer<Label> labelOscPort;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecoderSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
