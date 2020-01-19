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
#include "GroupTableListModel.h"
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
                                   public TableListBoxModel,
                                   public TableColumnCallback,
                                   public ChangeListener,
                                   public ChangeBroadcaster,
                                   public Button::Listener
{
public:
    //==============================================================================
    SourceDefinitionComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams);
    ~SourceDefinitionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    double getValue(int columnId, int rowNumber) override;
    void setValue(int columnId, int rowNumber, double newValue) override;
    SliderRange getSliderRange(int columnId) override;
    TableListBox* getTable() override;
    String getTableText(const int columnId, const int rowNumber) override;
    void setTableText(const int columnId, const int rowNumber, const String& newText) override;
    void selectedRowsChanged(int lastRowSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void controlDimming() const;
    void refresh();
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

