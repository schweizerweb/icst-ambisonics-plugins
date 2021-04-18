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
#include <JuceHeader.h>
#include "StatusMessageHandler.h"
#include "StatusMessageReceiver.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OSCLogComponent  : public juce::Component,
                         public StatusMessageReceiver
{
public:
    //==============================================================================
    OSCLogComponent (StatusMessageHandler* pStatusHandler);
    ~OSCLogComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void notifyOverflow(int discardedCount) override;
    void notify(StatusMessage msg) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    StatusMessageHandler* pStatusHandler;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextEditor> textLog;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCLogComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
