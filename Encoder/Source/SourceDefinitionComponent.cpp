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

//[Headers] You can add your own extra header files here...
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/TrackColors.h"
#include "../../Common/ZoomSettings.h"
//[/Headers]

#include "SourceDefinitionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
SourceDefinitionComponent::SourceDefinitionComponent (EncoderSettingsComponentArgs args)
    : m_args(args)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(m_args.pChangeListener);
    groupModel.reset(new GroupTableListModel(m_args.pSourceSet, m_args.pPointSelection, this, m_args.pZoomSettings->getScalingInfo()));
    sourceModel.reset(new SourceTableListModel(m_args.pSourceSet, m_args.pPointSelection, this, m_args.pZoomSettings->getScalingInfo()));
    //[/Constructor_pre]

    groupGroups.reset (new juce::GroupComponent ("groupGroups",
                                                 TRANS("Groups")));
    addAndMakeVisible (groupGroups.get());

    groupList.reset (new TableListBox());
    addAndMakeVisible (groupList.get());
    groupList->setName ("groupList");

    buttonAddGroup.reset (new juce::TextButton ("buttonAddGroup"));
    addAndMakeVisible (buttonAddGroup.get());
    buttonAddGroup->setButtonText (TRANS("add"));
    buttonAddGroup->addListener (this);

    buttonRemoveGroup.reset (new juce::TextButton ("buttonRemoveGroup"));
    addAndMakeVisible (buttonRemoveGroup.get());
    buttonRemoveGroup->setButtonText (TRANS("remove"));
    buttonRemoveGroup->addListener (this);

    groupSources.reset (new juce::GroupComponent ("groupSources",
                                                  TRANS("Sources")));
    addAndMakeVisible (groupSources.get());

    sourceList.reset (new TableListBox());
    addAndMakeVisible (sourceList.get());
    sourceList->setName ("sourceList");

    buttonAdd.reset (new juce::TextButton ("buttonAdd"));
    addAndMakeVisible (buttonAdd.get());
    buttonAdd->setButtonText (TRANS("add"));
    buttonAdd->addListener (this);

    buttonRemove.reset (new juce::TextButton ("buttonRemove"));
    addAndMakeVisible (buttonRemove.get());
    buttonRemove->setButtonText (TRANS("remove"));
    buttonRemove->addListener (this);

    buttonMoveDown.reset (new juce::TextButton ("buttonMoveDown"));
    addAndMakeVisible (buttonMoveDown.get());
    buttonMoveDown->setButtonText (TRANS("down"));
    buttonMoveDown->addListener (this);

    buttonMoveUp.reset (new juce::TextButton ("buttonMoveUp"));
    addAndMakeVisible (buttonMoveUp.get());
    buttonMoveUp->setButtonText (TRANS("up"));
    buttonMoveUp->addListener (this);

    buttonMoveGroupDown.reset (new juce::TextButton ("buttonMoveGroupDown"));
    addAndMakeVisible (buttonMoveGroupDown.get());
    buttonMoveGroupDown->setButtonText (TRANS("down"));
    buttonMoveGroupDown->addListener (this);

    buttonMoveGroupUp.reset (new juce::TextButton ("buttonMoveGroupUp"));
    addAndMakeVisible (buttonMoveGroupUp.get());
    buttonMoveGroupUp->setButtonText (TRANS("up"));
    buttonMoveGroupUp->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    groupSources->setVisible(MULTI_ENCODER_MODE);
    buttonAdd->setVisible(false);
    buttonRemove->setVisible(false);
    buttonMoveUp->setVisible(MULTI_ENCODER_MODE);
    buttonMoveDown->setVisible(MULTI_ENCODER_MODE);
    sourceList->setVisible(MULTI_ENCODER_MODE);
    groupGroups->setVisible(MULTI_ENCODER_MODE);
    groupList->setVisible(MULTI_ENCODER_MODE);
    buttonAddGroup->setVisible(MULTI_ENCODER_MODE);
    buttonRemoveGroup->setVisible(MULTI_ENCODER_MODE);
    buttonMoveGroupUp->setVisible(MULTI_ENCODER_MODE);
    buttonMoveGroupDown->setVisible(MULTI_ENCODER_MODE);

    sourceModel->initTable(sourceList.get());
    groupModel->initTable(groupList.get());
    m_args.pPointSelection->addChangeListener(this);
    controlDimming();
    //[/Constructor]
}

SourceDefinitionComponent::~SourceDefinitionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    m_args.pPointSelection->removeChangeListener(this);
    //[/Destructor_pre]

    groupGroups = nullptr;
    groupList = nullptr;
    buttonAddGroup = nullptr;
    buttonRemoveGroup = nullptr;
    groupSources = nullptr;
    sourceList = nullptr;
    buttonAdd = nullptr;
    buttonRemove = nullptr;
    buttonMoveDown = nullptr;
    buttonMoveUp = nullptr;
    buttonMoveGroupDown = nullptr;
    buttonMoveGroupUp = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    groupModel = nullptr;
    //[/Destructor]
}

//==============================================================================
void SourceDefinitionComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SourceDefinitionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupGroups->setBounds (0, getHeight() - proportionOfHeight (0.4013f), getWidth() - 0, proportionOfHeight (0.4013f));
    groupList->setBounds (0 + 16, (getHeight() - proportionOfHeight (0.4013f)) + 19, (getWidth() - 0) - 31, proportionOfHeight (0.4013f) - 67);
    buttonAddGroup->setBounds (0 + 17, (getHeight() - proportionOfHeight (0.4013f)) + proportionOfHeight (0.4013f) - 40, 64, 24);
    buttonRemoveGroup->setBounds (0 + 89, (getHeight() - proportionOfHeight (0.4013f)) + proportionOfHeight (0.4013f) - 40, 64, 24);
    groupSources->setBounds (0, 0, getWidth() - 0, proportionOfHeight (0.5987f));
    sourceList->setBounds (0 + 16, 0 + 19, (getWidth() - 0) - 31, proportionOfHeight (0.5987f) - 67);
    buttonAdd->setBounds (0 + 17, 0 + proportionOfHeight (0.5987f) - 40, 64, 24);
    buttonRemove->setBounds (0 + 89, 0 + proportionOfHeight (0.5987f) - 40, 64, 24);
    buttonMoveDown->setBounds (0 + (getWidth() - 0) - 80, 0 + proportionOfHeight (0.5987f) - 40, 64, 24);
    buttonMoveUp->setBounds (0 + (getWidth() - 0) - 152, 0 + proportionOfHeight (0.5987f) - 40, 64, 24);
    buttonMoveGroupDown->setBounds (0 + (getWidth() - 0) - 80, (getHeight() - proportionOfHeight (0.4013f)) + proportionOfHeight (0.4013f) - 40, 64, 24);
    buttonMoveGroupUp->setBounds (0 + (getWidth() - 0) - 152, (getHeight() - proportionOfHeight (0.4013f)) + proportionOfHeight (0.4013f) - 40, 64, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SourceDefinitionComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonAddGroup.get())
    {
        //[UserButtonCode_buttonAddGroup] -- add your button handler code here..
        if(m_args.pAudioParams != nullptr && m_args.pSourceSet->groupCount() < m_args.pAudioParams->groupParams.size())
        {
            Uuid newId = Uuid();
            m_args.pSourceSet->addGroup(newId.toString(), Point3D<double>(0.0, 0.0, 0.0, m_args.pAudioParams->groupParams.getUnchecked(m_args.pSourceSet->groupCount())), "G", Colours::orange);
            m_args.pPointSelection->selectGroup(m_args.pSourceSet->groupCount() - 1, false);
            groupList->updateContent();
            groupList->repaint();
        }
        else
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error", "No more groups allowed!");
        }
        //[/UserButtonCode_buttonAddGroup]
    }
    else if (buttonThatWasClicked == buttonRemoveGroup.get())
    {
        //[UserButtonCode_buttonRemoveGroup] -- add your button handler code here..
        int selection = m_args.pPointSelection->getMainSelectedPointIndex();
        if (m_args.pPointSelection->getSelectionMode() == PointSelection::Group && selection >= 0 && selection < m_args.pSourceSet->size())
        {
            m_args.pPointSelection->unselectPoint();
            m_args.pSourceSet->removeGroup(selection);
            groupList->updateContent();
            groupList->repaint();
        }

        //[/UserButtonCode_buttonRemoveGroup]
    }
    else if (buttonThatWasClicked == buttonAdd.get())
    {
        //[UserButtonCode_buttonAdd] -- add your button handler code here..
        if (m_args.pAudioParams != nullptr && m_args.pSourceSet->size() < m_args.pAudioParams->sourceParams.size())
        {
            Uuid newId = Uuid();
            m_args.pSourceSet->addNew(newId.toString(), Point3D<double>(0.0, 0.0, 0.0, m_args.pAudioParams->sourceParams.getUnchecked(m_args.pSourceSet->size())), m_args.pSourceSet->getNewUniqueName(), TrackColors::getColor(m_args.pSourceSet->size() + 1));
            m_args.pPointSelection->selectPoint(m_args.pSourceSet->size() - 1);
            sourceList->updateContent();
            sourceList->repaint();
        }
        else
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error", "No more sources allowed!");
        }

        //[/UserButtonCode_buttonAdd]
    }
    else if (buttonThatWasClicked == buttonRemove.get())
    {
        //[UserButtonCode_buttonRemove] -- add your button handler code here..
        int selection = m_args.pPointSelection->getMainSelectedPointIndex();
        if (m_args.pPointSelection->getSelectionMode() == PointSelection::Point && selection >= 0 && selection < m_args.pSourceSet->size())
        {
            m_args.pPointSelection->unselectPoint();
            m_args.pSourceSet->remove(selection);
            sourceList->updateContent();
            sourceList->repaint();
        }

        //[/UserButtonCode_buttonRemove]
    }
    else if (buttonThatWasClicked == buttonMoveDown.get())
    {
        //[UserButtonCode_buttonMoveDown] -- add your button handler code here..
        int selection = m_args.pPointSelection->getMainSelectedPointIndex();
        if (selection >= 0 && selection < m_args.pSourceSet->size() - 1)
        {
            m_args.pPointSelection->unselectPoint();
            m_args.pSourceSet->swap(selection, selection + 1);
            m_args.pPointSelection->selectPoint(selection + 1);
        }
        //[/UserButtonCode_buttonMoveDown]
    }
    else if (buttonThatWasClicked == buttonMoveUp.get())
    {
        //[UserButtonCode_buttonMoveUp] -- add your button handler code here..
        int selection = m_args.pPointSelection->getMainSelectedPointIndex();
        if (selection >= 1 && selection < m_args.pSourceSet->size())
        {
            m_args.pPointSelection->unselectPoint();
            m_args.pSourceSet->swap(selection, selection - 1);
            m_args.pPointSelection->selectPoint(selection - 1);
        }
        //[/UserButtonCode_buttonMoveUp]
    }
    else if (buttonThatWasClicked == buttonMoveGroupDown.get())
    {
        //[UserButtonCode_buttonMoveGroupDown] -- add your button handler code here..
        if(m_args.pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int selection = m_args.pPointSelection->getMainSelectedPointIndex();
            if (selection >= 0 && selection < m_args.pSourceSet->groupCount() - 1)
            {
                m_args.pPointSelection->unselectPoint();
                m_args.pSourceSet->swapGroup(selection, selection + 1);
                m_args.pPointSelection->selectGroup(selection + 1, false);
            }
        }
        //[/UserButtonCode_buttonMoveGroupDown]
    }
    else if (buttonThatWasClicked == buttonMoveGroupUp.get())
    {
        //[UserButtonCode_buttonMoveGroupUp] -- add your button handler code here..
        if(m_args.pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int selection = m_args.pPointSelection->getMainSelectedPointIndex();
            if (selection >= 1 && selection < m_args.pSourceSet->groupCount())
            {
                m_args.pPointSelection->unselectPoint();
                m_args.pSourceSet->swapGroup(selection, selection - 1);
                m_args.pPointSelection->selectGroup(selection - 1, false);
            }
        }
        //[/UserButtonCode_buttonMoveGroupUp]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SourceDefinitionComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    refresh();

    if (source == m_args.pPointSelection)
    {
        if (m_args.pPointSelection->getSelectionMode() == PointSelection::Point)
        {
            sourceList->selectRow(m_args.pPointSelection->getMainSelectedPointIndex());
            groupList->selectRow(-1);
        }
        else
        {
            sourceList->selectRow(-1);
            groupList->selectRow(m_args.pPointSelection->getMainSelectedPointIndex());
        }

        controlDimming();
    }
}

void SourceDefinitionComponent::controlDimming() const
{
    buttonAdd->setEnabled(m_args.pSourceSet->size() < m_args.pAudioParams->sourceParams.size());
    buttonRemove->setEnabled(m_args.pPointSelection->getSelectionMode() == PointSelection::Point && m_args.pSourceSet->size() > 0);
    buttonMoveUp->setEnabled(m_args.pPointSelection->getMainSelectedPointIndex() > 0);
    buttonMoveDown->setEnabled(m_args.pPointSelection->getSelectionMode() == PointSelection::Point && m_args.pPointSelection->getMainSelectedPointIndex() < m_args.pSourceSet->size() - 1);
    buttonAddGroup->setEnabled(true);
    buttonRemoveGroup->setEnabled(m_args.pPointSelection->getSelectionMode() == PointSelection::Group && m_args.pSourceSet->groupCount() > 0);
}

void SourceDefinitionComponent::refresh() const
{
    sourceList->updateContent();
    sourceList->repaint();
    groupList->updateContent();
    groupList->repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SourceDefinitionComponent"
                 componentName="" parentClasses="public Component, public ChangeListener, public ChangeBroadcaster"
                 constructorParams="EncoderSettingsComponentArgs args" variableInitialisers="m_args(args)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupGroups" id="983b0a3b2c5c945a" memberName="groupGroups"
                  virtualName="" explicitFocusOrder="0" pos="0 0Rr 0M 40.09%" posRelativeX="73249ab85d6bba3a"
                  posRelativeY="73249ab85d6bba3a" posRelativeW="73249ab85d6bba3a"
                  posRelativeH="73249ab85d6bba3a" title="Groups"/>
  <GENERICCOMPONENT name="groupList" id="df462ef21c261681" memberName="groupList"
                    virtualName="" explicitFocusOrder="0" pos="16 19 31M 67M" posRelativeX="983b0a3b2c5c945a"
                    posRelativeY="983b0a3b2c5c945a" posRelativeW="983b0a3b2c5c945a"
                    posRelativeH="983b0a3b2c5c945a" class="TableListBox" params=""/>
  <TEXTBUTTON name="buttonAddGroup" id="84fdb7fb0d342ca6" memberName="buttonAddGroup"
              virtualName="" explicitFocusOrder="0" pos="17 40R 64 24" posRelativeX="983b0a3b2c5c945a"
              posRelativeY="983b0a3b2c5c945a" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonRemoveGroup" id="4b7306753c54f44c" memberName="buttonRemoveGroup"
              virtualName="" explicitFocusOrder="0" pos="89 40R 64 24" posRelativeX="983b0a3b2c5c945a"
              posRelativeY="983b0a3b2c5c945a" buttonText="remove" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="groupSources" id="da4e7711e3fff0be" memberName="groupSources"
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 59.91%" posRelativeX="73249ab85d6bba3a"
                  posRelativeY="73249ab85d6bba3a" posRelativeW="73249ab85d6bba3a"
                  posRelativeH="73249ab85d6bba3a" title="Sources"/>
  <GENERICCOMPONENT name="sourceList" id="54cde0d0bf4f7a53" memberName="sourceList"
                    virtualName="" explicitFocusOrder="0" pos="16 19 31M 67M" posRelativeX="da4e7711e3fff0be"
                    posRelativeY="da4e7711e3fff0be" posRelativeW="da4e7711e3fff0be"
                    posRelativeH="da4e7711e3fff0be" class="TableListBox" params=""/>
  <TEXTBUTTON name="buttonAdd" id="e1290b9a1a32d249" memberName="buttonAdd"
              virtualName="" explicitFocusOrder="0" pos="17 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonRemove" id="49c8de1156e72d8c" memberName="buttonRemove"
              virtualName="" explicitFocusOrder="0" pos="89 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="remove" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveDown" id="7291297cb3544d01" memberName="buttonMoveDown"
              virtualName="" explicitFocusOrder="0" pos="80R 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="down" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveUp" id="e2d399b90fa42e97" memberName="buttonMoveUp"
              virtualName="" explicitFocusOrder="0" pos="152R 40R 64 24" posRelativeX="da4e7711e3fff0be"
              posRelativeY="da4e7711e3fff0be" buttonText="up" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveGroupDown" id="9952d6c7fdf6c103" memberName="buttonMoveGroupDown"
              virtualName="" explicitFocusOrder="0" pos="80R 40R 64 24" posRelativeX="983b0a3b2c5c945a"
              posRelativeY="983b0a3b2c5c945a" buttonText="down" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonMoveGroupUp" id="a7a2e7a9aeab456c" memberName="buttonMoveGroupUp"
              virtualName="" explicitFocusOrder="0" pos="152R 40R 64 24" posRelativeX="983b0a3b2c5c945a"
              posRelativeY="983b0a3b2c5c945a" buttonText="up" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

