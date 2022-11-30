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
#include "../JuceLibraryCode/JuceHeader.h"
#include "GroupTableListModel.h"
#include "SourceTableListModel.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/TableColumnCallback.h"
#include "../../Common/ZoomSettings.h"
#include "EncoderSettings.h"
#include "ComponentArguments.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SourceDefinitionComponent  : public Component,
                                   public ChangeListener,
                                   public ChangeBroadcaster,
                                   public juce::Button::Listener
{
public:
    //==============================================================================
    SourceDefinitionComponent (EncoderSettingsComponentArgs args);
    ~SourceDefinitionComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void controlDimming() const;
    void refresh() const;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EncoderSettingsComponentArgs m_args;
    std::unique_ptr<GroupTableListModel> groupModel;
    std::unique_ptr<SourceTableListModel> sourceModel;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupGroups;
    std::unique_ptr<TableListBox> groupList;
    std::unique_ptr<juce::TextButton> buttonAddGroup;
    std::unique_ptr<juce::TextButton> buttonRemoveGroup;
    std::unique_ptr<juce::GroupComponent> groupSources;
    std::unique_ptr<TableListBox> sourceList;
    std::unique_ptr<juce::TextButton> buttonAdd;
    std::unique_ptr<juce::TextButton> buttonRemove;
    std::unique_ptr<juce::TextButton> buttonMoveDown;
    std::unique_ptr<juce::TextButton> buttonMoveUp;
    std::unique_ptr<juce::TextButton> buttonMoveGroupDown;
    std::unique_ptr<juce::TextButton> buttonMoveGroupUp;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceDefinitionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

