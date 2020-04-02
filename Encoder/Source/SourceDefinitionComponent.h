/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
#include "EncoderSettings.h"
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
                                   public Button::Listener
{
public:
    //==============================================================================
    SourceDefinitionComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams);
    ~SourceDefinitionComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void controlDimming() const;
    void refresh() const;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AmbiSourceSet* pSources;
    PointSelection* pPointSelection;
    Array<AudioParameterSet>* pAudioParams;
    std::unique_ptr<GroupTableListModel> groupModel;
    std::unique_ptr<SourceTableListModel> sourceModel;
    EncoderSettings* pEncoderSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupGroups;
    std::unique_ptr<TableListBox> groupList;
    std::unique_ptr<TextButton> buttonAddGroup;
    std::unique_ptr<TextButton> buttonRemoveGroup;
    std::unique_ptr<GroupComponent> groupSources;
    std::unique_ptr<TableListBox> sourceList;
    std::unique_ptr<TextButton> buttonAdd;
    std::unique_ptr<TextButton> buttonRemove;
    std::unique_ptr<TextButton> buttonMoveDown;
    std::unique_ptr<TextButton> buttonMoveUp;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceDefinitionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

