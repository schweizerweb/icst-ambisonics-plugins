/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "EncoderSettings.h"
#include "CustomOscTableListModel.h"
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
                             public juce::Button::Listener,
                             public juce::Slider::Listener
{
public:
    //==============================================================================
    OSCTargetsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings);
    ~OSCTargetsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void textEditorTextChanged(TextEditor&) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void controlDimming();
    EncoderSettings* pSettings;
    std::unique_ptr<CustomOscTableListModel> customOscTableModel;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupStandard;
    std::unique_ptr<juce::ToggleButton> toggleEnableStandardXyz;
    std::unique_ptr<juce::TextEditor> textOscSendIpExtXyz;
    std::unique_ptr<juce::Label> labelOscSendIpExt;
    std::unique_ptr<juce::GroupComponent> groupCustom;
    std::unique_ptr<TableListBox> targetList;
    std::unique_ptr<juce::GroupComponent> groupGeneral;
    std::unique_ptr<juce::Slider> sliderInterval;
    std::unique_ptr<juce::Label> labelInterval;
    std::unique_ptr<juce::ToggleButton> toggleEnableStandardAed;
    std::unique_ptr<juce::TextEditor> textOscSendIpExtAed;
    std::unique_ptr<juce::Label> labelOscSendIpExt2;
    std::unique_ptr<juce::Slider> sliderPortExtXyz;
    std::unique_ptr<juce::Slider> sliderPortExtAed;
    std::unique_ptr<juce::TextButton> btnAdd;
    std::unique_ptr<juce::TextButton> btnDelete;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCTargetsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

