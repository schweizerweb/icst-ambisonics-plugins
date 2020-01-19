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

//[Headers] You can add your own extra header files here...
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/ColorEditorCustomComponent.h"
#include "../../Common/TrackColors.h"
//[/Headers]

#include "SourceDefinitionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define COLUMN_ID_NB        1
#define COLUMN_ID_NAME        2
#define    COLUMN_ID_X            7
#define    COLUMN_ID_Y            8
#define    COLUMN_ID_Z            9
#define    COLUMN_ID_A            10
#define    COLUMN_ID_E            11
#define    COLUMN_ID_D            12
#define COLUMN_ID_GAIN        5
#define COLUMN_ID_COLOR        13

//[/MiscUserDefs]

//==============================================================================
SourceDefinitionComponent::SourceDefinitionComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams)
    : pEncoderSettings(pSettings), pSources(pSourceSet), pPointSelection(pPointSelection), pAudioParams(pAudioParams)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);

    groupModel.reset(new GroupTableListModel(pSourceSet, pPointSelection, this));
    //[/Constructor_pre]

    groupGroups.reset (new GroupComponent ("groupGroups",
                                           TRANS("Groups")));
    addAndMakeVisible (groupGroups.get());

    groupList.reset (new TableListBox());
    addAndMakeVisible (groupList.get());
    groupList->setName ("groupList");

    buttonAddGroup.reset (new TextButton ("buttonAddGroup"));
    addAndMakeVisible (buttonAddGroup.get());
    buttonAddGroup->setButtonText (TRANS("add"));
    buttonAddGroup->addListener (this);

    buttonRemoveGroup.reset (new TextButton ("buttonRemoveGroup"));
    addAndMakeVisible (buttonRemoveGroup.get());
    buttonRemoveGroup->setButtonText (TRANS("remove"));
    buttonRemoveGroup->addListener (this);

    groupSources.reset (new GroupComponent ("groupSources",
                                            TRANS("Sources")));
    addAndMakeVisible (groupSources.get());

    sourceList.reset (new TableListBox());
    addAndMakeVisible (sourceList.get());
    sourceList->setName ("sourceList");

    buttonAdd.reset (new TextButton ("buttonAdd"));
    addAndMakeVisible (buttonAdd.get());
    buttonAdd->setButtonText (TRANS("add"));
    buttonAdd->addListener (this);

    buttonRemove.reset (new TextButton ("buttonRemove"));
    addAndMakeVisible (buttonRemove.get());
    buttonRemove->setButtonText (TRANS("remove"));
    buttonRemove->addListener (this);

    buttonMoveDown.reset (new TextButton ("buttonMoveDown"));
    addAndMakeVisible (buttonMoveDown.get());
    buttonMoveDown->setButtonText (TRANS("down"));
    buttonMoveDown->addListener (this);

    buttonMoveUp.reset (new TextButton ("buttonMoveUp"));
    addAndMakeVisible (buttonMoveUp.get());
    buttonMoveUp->setButtonText (TRANS("up"));
    buttonMoveUp->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    groupSources->setVisible(MULTI_ENCODER_MODE);
    buttonAdd->setVisible(MULTI_ENCODER_MODE);
    buttonRemove->setVisible(MULTI_ENCODER_MODE);
    buttonMoveUp->setVisible(MULTI_ENCODER_MODE);
    buttonMoveDown->setVisible(MULTI_ENCODER_MODE);
    sourceList->setVisible(MULTI_ENCODER_MODE);
    groupGroups->setVisible(MULTI_ENCODER_MODE);
    groupList->setVisible(MULTI_ENCODER_MODE);
    buttonAddGroup->setVisible(MULTI_ENCODER_MODE);
    buttonRemoveGroup->setVisible(MULTI_ENCODER_MODE);

    // table
    sourceList->setModel(this);
    sourceList->getHeader().addColumn("CH", COLUMN_ID_NB, 30);
    sourceList->getHeader().addColumn("Name", COLUMN_ID_NAME, 100);
    sourceList->getHeader().addColumn("X", COLUMN_ID_X, 50);
    sourceList->getHeader().addColumn("Y", COLUMN_ID_Y, 50);
    sourceList->getHeader().addColumn("Z", COLUMN_ID_Z, 50);
    sourceList->getHeader().addColumn("A", COLUMN_ID_A, 50);
    sourceList->getHeader().addColumn("E", COLUMN_ID_E, 50);
    sourceList->getHeader().addColumn("D", COLUMN_ID_D, 50);
    sourceList->getHeader().addColumn("Gain [dB]", COLUMN_ID_GAIN, 80);
    sourceList->getHeader().addColumn("Color", COLUMN_ID_COLOR, 60);
    sourceList->getHeader().setStretchToFitActive(true);
    sourceList->getHeader().resizeAllColumnsToFit(sourceList->getWidth());

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


    //[Destructor]. You can add your own custom destruction code here..
    groupModel = nullptr;
    //[/Destructor]
}

//==============================================================================
void SourceDefinitionComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SourceDefinitionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupGroups->setBounds (0, getHeight() - proportionOfHeight (0.4004f), getWidth() - 0, proportionOfHeight (0.4004f));
    groupList->setBounds (0 + 16, (getHeight() - proportionOfHeight (0.4004f)) + 19, (getWidth() - 0) - 31, proportionOfHeight (0.4004f) - 67);
    buttonAddGroup->setBounds (0 + 17, (getHeight() - proportionOfHeight (0.4004f)) + proportionOfHeight (0.4004f) - 40, 64, 24);
    buttonRemoveGroup->setBounds (0 + 89, (getHeight() - proportionOfHeight (0.4004f)) + proportionOfHeight (0.4004f) - 40, 64, 24);
    groupSources->setBounds (0, 0, getWidth() - 0, proportionOfHeight (0.5996f));
    sourceList->setBounds (0 + 16, 0 + 19, (getWidth() - 0) - 31, proportionOfHeight (0.5996f) - 67);
    buttonAdd->setBounds (0 + 17, 0 + proportionOfHeight (0.5996f) - 40, 64, 24);
    buttonRemove->setBounds (0 + 89, 0 + proportionOfHeight (0.5996f) - 40, 64, 24);
    buttonMoveDown->setBounds (0 + (getWidth() - 0) - 80, 0 + proportionOfHeight (0.5996f) - 40, 64, 24);
    buttonMoveUp->setBounds (0 + (getWidth() - 0) - 152, 0 + proportionOfHeight (0.5996f) - 40, 64, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SourceDefinitionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonAddGroup.get())
    {
        //[UserButtonCode_buttonAddGroup] -- add your button handler code here..
        Uuid newId = Uuid();
        pSources->addGroup(newId.toString(), Point3D<double>(0.0, 0.0, 0.0), "G", Colours::orange);
        pPointSelection->selectGroup(pSources->groupCount() - 1, false);
        groupList->updateContent();
        groupList->repaint();

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
        if (pAudioParams != nullptr && pSources->size() < pAudioParams->size())
        {
            Uuid newId = Uuid();
            pSources->addNew(newId.toString(), Point3D<double>(0.0, 0.0, 0.0, pAudioParams->getUnchecked(pSources->size())), pSources->getNewUniqueName(), TrackColors::getColor(pSources->size() + 1));
            pPointSelection->selectPoint(pSources->size() - 1);
            sourceList->updateContent();
            sourceList->repaint();
        }
        else
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "No more sources allowed!");
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
int SourceDefinitionComponent::getNumRows()
{
    return pSources->size();
}

void SourceDefinitionComponent::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    const Colour alternateColour(getLookAndFeel().findColour(ListBox::backgroundColourId)
        .interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
    if (rowIsSelected)
        g.fillAll(Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll(alternateColour);
}

void SourceDefinitionComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    AmbiPoint* pt = pSources->get(rowNumber);
    if (pt == nullptr)
        return;

    g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
    String text;
    switch (columnId)
    {
    case COLUMN_ID_NB: text = String(rowNumber + 1); break;
    case COLUMN_ID_NAME: text = pt->getName(); break;
    default: text = "";
    }
    g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}

double SourceDefinitionComponent::getValue(int columnId, int rowNumber)
{
    AmbiPoint* pt = pSources->get(rowNumber);
    if (pt == nullptr)
        return 0.0;

    switch (columnId)
    {
    case COLUMN_ID_GAIN: return 10.0 * log10(pt->getGain());
    case COLUMN_ID_X: return pt->getPoint()->getX();
    case COLUMN_ID_Y: return pt->getPoint()->getY();
    case COLUMN_ID_Z: return pt->getPoint()->getZ();
    case COLUMN_ID_A: return Constants::RadToGrad(pt->getPoint()->getAzimuth());
    case COLUMN_ID_E: return Constants::RadToGrad(pt->getPoint()->getElevation());
    case COLUMN_ID_D: return pt->getPoint()->getDistance();
    case COLUMN_ID_COLOR: return pt->getColor().getARGB();
    default: return 0.0;
    }
}

void SourceDefinitionComponent::setValue(int columnId, int rowNumber, double newValue)
{
    switch (columnId)
    {
    case COLUMN_ID_GAIN: pSources->setGain(rowNumber, pow(10.0, 0.1 * newValue)); break;
    case COLUMN_ID_X: pSources->setX(rowNumber, newValue); break;
    case COLUMN_ID_Y: pSources->setY(rowNumber, newValue); break;
    case COLUMN_ID_Z: pSources->setZ(rowNumber, newValue); break;
    case COLUMN_ID_A: pSources->setAzimuth(rowNumber, Constants::GradToRad(newValue)); break;
    case COLUMN_ID_E: pSources->setElevation(rowNumber, Constants::GradToRad(newValue)); break;
    case COLUMN_ID_D: pSources->setDistance(rowNumber, newValue); break;
    case COLUMN_ID_COLOR: pSources->setChannelColor(rowNumber, Colour(uint32(newValue))); break;
    default: throw;
    }

    sourceList->updateContent();
    sourceList->repaint();
}

SliderRange SourceDefinitionComponent::getSliderRange(int columnId)
{
    switch (columnId)
    {
    case COLUMN_ID_X:
    case COLUMN_ID_Y:
    case COLUMN_ID_Z:
        return SliderRange(-1.0, 1.0, 0.001);

    case COLUMN_ID_D:
        return SliderRange(Constants::DistanceMin, Constants::DistanceMax, 0.001);

    case COLUMN_ID_A:
        return SliderRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);

    case COLUMN_ID_E:
        return SliderRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);

    case COLUMN_ID_GAIN:
        return SliderRange(Constants::GainDbMin, Constants::GainDbMax, 0.5);
    default: return SliderRange(0.0, 1.0, 0.001);
    }
}

TableListBox* SourceDefinitionComponent::getTable()
{
    return sourceList.get();
}

String SourceDefinitionComponent::getTableText(const int columnId, const int rowNumber)
{
    AmbiPoint* pt = pSources->get(rowNumber);
    if (pt == nullptr)
        return "";

    switch (columnId)
    {
    case COLUMN_ID_NAME: return pt->getName();
    default: return "";
    }
}

void SourceDefinitionComponent::setTableText(const int columnId, const int rowNumber, const String& newText)
{
    switch (columnId)
    {
    case COLUMN_ID_NAME: pSources->setChannelName(rowNumber, newText); break;
    default: throw;
    }
}

void SourceDefinitionComponent::selectedRowsChanged(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < pSources->size())
        pPointSelection->selectPoint(lastRowSelected);
}

Component* SourceDefinitionComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate)
{
    if (columnId == COLUMN_ID_X
        || columnId == COLUMN_ID_Y
        || columnId == COLUMN_ID_Z
        || columnId == COLUMN_ID_A
        || columnId == COLUMN_ID_E
        || columnId == COLUMN_ID_D)
    {
        NumericColumnCustomComponent* numericBox = static_cast<NumericColumnCustomComponent*> (existingComponentToUpdate);
        if (numericBox == nullptr)
            numericBox = new NumericColumnCustomComponent(*this);

        numericBox->setRowAndColumn(rowNumber, columnId);
        return numericBox;
    }
    else if (columnId == COLUMN_ID_GAIN)
    {
        SliderColumnCustomComponent* gainBox = static_cast<SliderColumnCustomComponent*> (existingComponentToUpdate);
        if (gainBox == nullptr)
            gainBox = new SliderColumnCustomComponent(*this);

        gainBox->setRowAndColumn(rowNumber, columnId);
        return gainBox;
    }
    else if (columnId == COLUMN_ID_NAME)
    {
        EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
        if (textLabel == nullptr)
            textLabel = new EditableTextCustomComponent(*this);

        textLabel->setRowAndColumn(rowNumber, columnId);
        return textLabel;
    }
    else if (columnId == COLUMN_ID_COLOR)
    {
        ColorEditorCustomComponent* colorBox = static_cast<ColorEditorCustomComponent*> (existingComponentToUpdate);
        if (colorBox == nullptr)
            colorBox = new ColorEditorCustomComponent(*this);

        colorBox->setRowAndColumn(rowNumber, columnId);
        return colorBox;
    }

    return nullptr;
}

void SourceDefinitionComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    sourceList->updateContent();
    sourceList->repaint();
    groupList->updateContent();
    groupList->repaint();

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
    buttonAdd->setEnabled(pSources->size() < pAudioParams->size());
    buttonRemove->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Point && pSources->size() > 0);
    buttonMoveUp->setEnabled(pPointSelection->getMainSelectedPointIndex() > 0);
    buttonMoveDown->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getMainSelectedPointIndex() < pSources->size() - 1);
    buttonAdd->setEnabled(true);
    buttonRemoveGroup->setEnabled(pPointSelection->getSelectionMode() == PointSelection::Group && pSources->groupCount() > 0);
}

void SourceDefinitionComponent::refresh()
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
                 componentName="" parentClasses="public Component, public TableListBoxModel, public TableColumnCallback, public ChangeListener, public ChangeBroadcaster"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array&lt;AudioParameterSet&gt;* pAudioParams"
                 variableInitialisers="pEncoderSettings(pSettings), pSources(pSourceSet), pPointSelection(pPointSelection), pAudioParams(pAudioParams)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupGroups" id="983b0a3b2c5c945a" memberName="groupGroups"
                  virtualName="" explicitFocusOrder="0" pos="0 0Rr 0M 40.038%"
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
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 59.962%" posRelativeX="73249ab85d6bba3a"
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
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

