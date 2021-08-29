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

//[Headers] You can add your own extra header files here...
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/TrackColors.h"
#include "../../Common/ZoomSettings.h"
//[/Headers]

#include "SourceDefinitionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
SourceDefinitionComponent::SourceDefinitionComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, ZoomSettings* pZoomSettings)
    : pSources(pSourceSet), pPointSelection(pPointSelection), pAudioParams(pAudioParams), pEncoderSettings(pSettings)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    groupModel.reset(new GroupTableListModel(pSourceSet, pPointSelection, this, pZoomSettings->getScalingInfo()));
    sourceModel.reset(new SourceTableListModel(pSourceSet, pPointSelection, this, pZoomSettings->getScalingInfo()));
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
    pPointSelection->addChangeListener(this);
    controlDimming();
    //[/Constructor]
}

SourceDefinitionComponent::~SourceDefinitionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPointSelection->removeChangeListener(this);
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
        if(pAudioParams != nullptr && pSources->groupCount() < pAudioParams->groupParams.size())
        {
            Uuid newId = Uuid();
            pSources->addGroup(newId.toString(), Point3D<double>(0.0, 0.0, 0.0, pAudioParams->groupParams.getUnchecked(pSources->groupCount())), "G", Colours::orange);
            pPointSelection->selectGroup(pSources->groupCount() - 1, false);
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
        int selection = pPointSelection->getMainSelectedPointIndex();
        if (pPointSelection->getSelectionMode() == PointSelection::Group && selection >= 0 && selection < pSources->size())
        {
            pPointSelection->unselectPoint();
            pSources->removeGroup(selection);
            groupList->updateContent();
            groupList->repaint();
        }

        //[/UserButtonCode_buttonRemoveGroup]
    }
    else if (buttonThatWasClicked == buttonAdd.get())
    {
        //[UserButtonCode_buttonAdd] -- add your button handler code here..
        if (pAudioParams != nullptr && pSources->size() < pAudioParams->sourceParams.size())
        {
            Uuid newId = Uuid();
            pSources->addNew(newId.toString(), Point3D<double>(0.0, 0.0, 0.0, pAudioParams->sourceParams.getUnchecked(pSources->size())), pSources->getNewUniqueName(), TrackColors::getColor(pSources->size() + 1));
            pPointSelection->selectPoint(pSources->size() - 1);
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
        int selection = pPointSelection->getMainSelectedPointIndex();
        if (pPointSelection->getSelectionMode() == PointSelection::Point && selection >= 0 && selection < pSources->size())
        {
            pPointSelection->unselectPoint();
            pSources->remove(selection);
            sourceList->updateContent();
            sourceList->repaint();
        }

        //[/UserButtonCode_buttonRemove]
    }
    else if (buttonThatWasClicked == buttonMoveDown.get())
    {
        //[UserButtonCode_buttonMoveDown] -- add your button handler code here..
        int selection = pPointSelection->getMainSelectedPointIndex();
        if (selection >= 0 && selection < pSources->size() - 1)
        {
            pPointSelection->unselectPoint();
            pSources->swap(selection, selection + 1);
            pPointSelection->selectPoint(selection + 1);
        }
        //[/UserButtonCode_buttonMoveDown]
    }
    else if (buttonThatWasClicked == buttonMoveUp.get())
    {
        //[UserButtonCode_buttonMoveUp] -- add your button handler code here..
        int selection = pPointSelection->getMainSelectedPointIndex();
        if (selection >= 1 && selection < pSources->size())
        {
            pPointSelection->unselectPoint();
            pSources->swap(selection, selection - 1);
            pPointSelection->selectPoint(selection - 1);
        }
        //[/UserButtonCode_buttonMoveUp]
    }
    else if (buttonThatWasClicked == buttonMoveGroupDown.get())
    {
        //[UserButtonCode_buttonMoveGroupDown] -- add your button handler code here..
        if(pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int selection = pPointSelection->getMainSelectedPointIndex();
            if (selection >= 0 && selection < pSources->groupCount() - 1)
            {
                pPointSelection->unselectPoint();
                pSources->swapGroup(selection, selection + 1);
                pPointSelection->selectGroup(selection + 1, false);
            }
        }
        //[/UserButtonCode_buttonMoveGroupDown]
    }
    else if (buttonThatWasClicked == buttonMoveGroupUp.get())
    {
        //[UserButtonCode_buttonMoveGroupUp] -- add your button handler code here..
        if(pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int selection = pPointSelection->getMainSelectedPointIndex();
            if (selection >= 1 && selection < pSources->groupCount())
            {
                pPointSelection->unselectPoint();
                pSources->swapGroup(selection, selection - 1);
                pPointSelection->selectGroup(selection - 1, false);
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

    if (source == pPointSelection)
    {
        if (pPointSelection->getSelectionMode() == PointSelection::Point)
        {
            sourceList->selectRow(pPointSelection->getMainSelectedPointIndex());
            groupList->selectRow(-1);
        }
        else
        {
            sourceList->selectRow(-1);
            groupList->selectRow(pPointSelection->getMainSelectedPointIndex());
        }

        controlDimming();
    }
}

void SourceDefinitionComponent::controlDimming() const
{
    buttonAdd->setEnabled(pSources->size() < pAudioParams->sourceParams.size());
    buttonRemove->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Point && pSources->size() > 0);
    buttonMoveUp->setEnabled(pPointSelection->getMainSelectedPointIndex() > 0);
    buttonMoveDown->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getMainSelectedPointIndex() < pSources->size() - 1);
    buttonAddGroup->setEnabled(true);
    buttonRemoveGroup->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Group && pSources->groupCount() > 0);
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
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, ZoomSettings* pZoomSettings"
                 variableInitialisers="pSources(pSourceSet), pPointSelection(pPointSelection), pAudioParams(pAudioParams), pEncoderSettings(pSettings)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupGroups" id="983b0a3b2c5c945a" memberName="groupGroups"
                  virtualName="" explicitFocusOrder="0" pos="0 0Rr 0M 40.127%"
                  posRelativeX="73249ab85d6bba3a" posRelativeY="73249ab85d6bba3a"
                  posRelativeW="73249ab85d6bba3a" posRelativeH="73249ab85d6bba3a"
                  title="Groups"/>
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
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 59.873%" posRelativeX="73249ab85d6bba3a"
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

