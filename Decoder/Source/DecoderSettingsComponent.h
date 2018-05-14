/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
                                  public TextEditor::Listener,
                                  public Button::Listener
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
    ScopedPointer<TextEditor> textTimeout;
    ScopedPointer<Label> labelTimeout;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecoderSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
