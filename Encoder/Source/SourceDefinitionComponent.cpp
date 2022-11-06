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
#include "../../Common/LabelCreator.h"
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

    sliderDistanceScaler.reset (new juce::Slider ("sliderDistanceScaler"));
    addAndMakeVisible (sliderDistanceScaler.get());
    sliderDistanceScaler->setRange (1, 1000, 0.1);
    sliderDistanceScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);

    labelDistanceScaler.reset (new juce::Label ("labelDistanceScaler",
                                                TRANS("Distance Scaler:")));
    addAndMakeVisible (labelDistanceScaler.get());
    labelDistanceScaler->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceScaler->setJustificationType (juce::Justification::centredLeft);
    labelDistanceScaler->setEditable (false, false, false);
    labelDistanceScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDistanceScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelDistanceScaler->setBounds (6, 39, 109, 24);

    toggleInfiniteDistance.reset (new juce::ToggleButton ("toggleInfiniteDistance"));
    addAndMakeVisible (toggleInfiniteDistance.get());
    toggleInfiniteDistance->setButtonText (TRANS("Infinite"));
    toggleInfiniteDistance->addListener (this);

    labelMasterGain.reset (new juce::Label ("labelMasterGain",
                                            TRANS("Master Gain [dB]:")));
    addAndMakeVisible (labelMasterGain.get());
    labelMasterGain->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelMasterGain->setJustificationType (juce::Justification::centredLeft);
    labelMasterGain->setEditable (false, false, false);
    labelMasterGain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelMasterGain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelMasterGain->setBounds (6, 69, 109, 24);

    sliderMasterGain.reset (new juce::Slider ("sliderMasterGain"));
    addAndMakeVisible (sliderMasterGain.get());
    sliderMasterGain->setRange (0, 36, 0.1);
    sliderMasterGain->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderMasterGain->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderMasterGain->addListener (this);

    toggleGroupMode.reset (new juce::ToggleButton ("toggleGroupMode"));
    addAndMakeVisible (toggleGroupMode.get());
    toggleGroupMode->setButtonText (TRANS("Group Mode"));
    toggleGroupMode->addListener (this);

    toggleGroupMode->setBounds (8, 8, 199, 24);

    comboBoxPresets.reset (new juce::ComboBox ("comboBoxPresets"));
    addAndMakeVisible (comboBoxPresets.get());
    comboBoxPresets->setEditableText (false);
    comboBoxPresets->setJustificationType (juce::Justification::centredLeft);
    comboBoxPresets->setTextWhenNothingSelected (TRANS("-"));
    comboBoxPresets->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxPresets->addListener (this);

    labelPresets.reset (new juce::Label ("labelPresets",
                                         TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (juce::Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPresets->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonSave.reset (new juce::TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    buttonManagePresets.reset (new juce::TextButton ("buttonManagePresets"));
    addAndMakeVisible (buttonManagePresets.get());
    buttonManagePresets->setButtonText (TRANS("manage..."));
    buttonManagePresets->addListener (this);

    dummyHeight.reset (new juce::Component());
    addAndMakeVisible (dummyHeight.get());
    dummyHeight->setName ("dummyHeight");


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

    sliderDistanceScaler->setSkewFactorFromMidPoint(50.0);
    updateEncodingUiElements();

    labelPresets->setVisible(MULTI_ENCODER_MODE);
    comboBoxPresets->setVisible(MULTI_ENCODER_MODE);
    buttonSave->setVisible(MULTI_ENCODER_MODE);
    buttonManagePresets->setVisible(MULTI_ENCODER_MODE);
    labelMasterGain->setVisible(MULTI_ENCODER_MODE);
    sliderMasterGain->setVisible(MULTI_ENCODER_MODE);

    sliderMasterGain->setRange(EncoderConstants::MasterGainMin, EncoderConstants::MasterGainMax, EncoderConstants::MasterGainResolution);
    sliderMasterGain->setNumDecimalPlacesToDisplay(1);
    sliderMasterGain->setTextValueSuffix(" dB");
    sliderMasterGain->setValue(m_args.pSourceSet->getMasterGain());
    // load stored presets
    m_args.pPresetHelper->addActionListener(this);
    m_args.pSourceSet->addChangeListener(this);
    initializePresets();
    
    sourceList->addMouseListener(this, true);
    groupList->addMouseListener(this, true);
    
    controlDimming();
    //[/Constructor]
}

SourceDefinitionComponent::~SourceDefinitionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    m_args.pPointSelection->removeChangeListener(this);
    m_args.pSourceSet->removeChangeListener(this);
    m_args.pPresetHelper->removeActionListener(this);
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
    sliderDistanceScaler = nullptr;
    labelDistanceScaler = nullptr;
    toggleInfiniteDistance = nullptr;
    labelMasterGain = nullptr;
    sliderMasterGain = nullptr;
    toggleGroupMode = nullptr;
    comboBoxPresets = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    buttonManagePresets = nullptr;
    dummyHeight = nullptr;


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

    groupGroups->setBounds (0, 96 + (getHeight() - 134) - (juce::roundToInt ((getHeight() - 134) * 0.4343f)), getWidth() - 0, juce::roundToInt ((getHeight() - 134) * 0.4343f));
    groupList->setBounds (0 + 16, (96 + (getHeight() - 134) - (juce::roundToInt ((getHeight() - 134) * 0.4343f))) + 19, (getWidth() - 0) - 31, (juce::roundToInt ((getHeight() - 134) * 0.4343f)) - 67);
    buttonAddGroup->setBounds (0 + 17, (96 + (getHeight() - 134) - (juce::roundToInt ((getHeight() - 134) * 0.4343f))) + (juce::roundToInt ((getHeight() - 134) * 0.4343f)) - 40, 64, 24);
    buttonRemoveGroup->setBounds (0 + 89, (96 + (getHeight() - 134) - (juce::roundToInt ((getHeight() - 134) * 0.4343f))) + (juce::roundToInt ((getHeight() - 134) * 0.4343f)) - 40, 64, 24);
    groupSources->setBounds (0, 96 + 0, getWidth() - 0, juce::roundToInt ((getHeight() - 134) * 0.5657f));
    sourceList->setBounds (0 + 16, (96 + 0) + 19, (getWidth() - 0) - 31, (juce::roundToInt ((getHeight() - 134) * 0.5657f)) - 67);
    buttonAdd->setBounds (0 + 17, (96 + 0) + (juce::roundToInt ((getHeight() - 134) * 0.5657f)) - 40, 64, 24);
    buttonRemove->setBounds (0 + 89, (96 + 0) + (juce::roundToInt ((getHeight() - 134) * 0.5657f)) - 40, 64, 24);
    buttonMoveDown->setBounds (0 + (getWidth() - 0) - 80, (96 + 0) + (juce::roundToInt ((getHeight() - 134) * 0.5657f)) - 40, 64, 24);
    buttonMoveUp->setBounds (0 + (getWidth() - 0) - 152, (96 + 0) + (juce::roundToInt ((getHeight() - 134) * 0.5657f)) - 40, 64, 24);
    buttonMoveGroupDown->setBounds (0 + (getWidth() - 0) - 80, (96 + (getHeight() - 134) - (juce::roundToInt ((getHeight() - 134) * 0.4343f))) + (juce::roundToInt ((getHeight() - 134) * 0.4343f)) - 40, 64, 24);
    buttonMoveGroupUp->setBounds (0 + (getWidth() - 0) - 152, (96 + (getHeight() - 134) - (juce::roundToInt ((getHeight() - 134) * 0.4343f))) + (juce::roundToInt ((getHeight() - 134) * 0.4343f)) - 40, 64, 24);
    sliderDistanceScaler->setBounds (getWidth() - 98 - (getWidth() - 301), 39, getWidth() - 301, 24);
    toggleInfiniteDistance->setBounds (getWidth() - 90, 39, 72, 24);
    sliderMasterGain->setBounds (getWidth() - 98 - (getWidth() - 301), 69, getWidth() - 301, 24);
    comboBoxPresets->setBounds (83, getHeight() - 8 - 24, getWidth() - 290, 24);
    labelPresets->setBounds (8, getHeight() - 8 - 24, 64, 24);
    buttonSave->setBounds (getWidth() - 110 - 90, getHeight() - 8 - 24, 90, 24);
    buttonManagePresets->setBounds (getWidth() - 8 - 90, getHeight() - 8 - 24, 90, 24);
    dummyHeight->setBounds (0, 96, 24, getHeight() - 134);
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
        if(m_args.pAudioParams != nullptr && m_args.pSourceSet->activeGroupCount() < m_args.pAudioParams->groupParams.size())
        {
            Uuid newId = Uuid();
            int newIndex = m_args.pSourceSet->addGroup(newId.toString(), Vector3D<double>(0.0, 0.0, 0.0), "G", COLOR_DEFINITION_GROUP_DEFAULT);
            m_args.pPointSelection->selectGroup(newIndex, false);
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
            if (selection >= 0 && selection < m_args.pSourceSet->activeGroupCount() - 1)
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
            if (selection >= 1 && selection < m_args.pSourceSet->activeGroupCount())
            {
                m_args.pPointSelection->unselectPoint();
                m_args.pSourceSet->swapGroup(selection, selection - 1);
                m_args.pPointSelection->selectGroup(selection - 1, false);
            }
        }
        //[/UserButtonCode_buttonMoveGroupUp]
    }
    else if (buttonThatWasClicked == toggleInfiniteDistance.get())
    {
        //[UserButtonCode_toggleInfiniteDistance] -- add your button handler code here..
        m_args.pSourceSet->setDistanceScaler(toggleInfiniteDistance->getToggleState() ? m_args.pZoomSettings->getScalingInfo()->Infinite : sliderDistanceScaler->getValue());
        m_args.pZoomSettings->getScalingInfo()->SetScaler(m_args.pSourceSet->getDistanceScaler());
        refresh();
        updateEncodingUiElements();
        //[/UserButtonCode_toggleInfiniteDistance]
    }
    else if (buttonThatWasClicked == toggleGroupMode.get())
    {
        //[UserButtonCode_toggleGroupMode] -- add your button handler code here..
        m_args.pSourceSet->setGroupModeFlag(toggleGroupMode->getToggleState());
        refresh();
        //[/UserButtonCode_toggleGroupMode]
    }
    else if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = m_args.pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
            return;

        m_args.pPresetHelper->writeToXmlFile(*newFile, m_args.pSourceSet, m_args.pSettings);
        comboBoxPresets->setText("", dontSendNotification);
        delete newFile;
        //[/UserButtonCode_buttonSave]
    }
    else if (buttonThatWasClicked == buttonManagePresets.get())
    {
        //[UserButtonCode_buttonManagePresets] -- add your button handler code here..
        presetManagerDialog.show(this, m_args.pPresetHelper);
        //[/UserButtonCode_buttonManagePresets]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SourceDefinitionComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderDistanceScaler.get())
    {
        //[UserSliderCode_sliderDistanceScaler] -- add your slider handling code here..
        m_args.pSourceSet->setDistanceScaler(sliderDistanceScaler->getValue());
        m_args.pZoomSettings->getScalingInfo()->SetScaler(m_args.pSourceSet->getDistanceScaler());
        m_args.pZoomSettings->Reset();
        sendChangeMessage();
        refresh();
        //[/UserSliderCode_sliderDistanceScaler]
    }
    else if (sliderThatWasMoved == sliderMasterGain.get())
    {
        //[UserSliderCode_sliderMasterGain] -- add your slider handling code here..
        m_args.pSourceSet->setMasterGain((float)sliderMasterGain->getValue());
        //[/UserSliderCode_sliderMasterGain]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SourceDefinitionComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxPresets.get())
    {
        //[UserComboBoxCode_comboBoxPresets] -- add your combo box handling code here..
        m_args.pPresetHelper->selectPresetName(comboBoxPresets->getText());
        comboBoxPresets->setText("", dontSendNotification);
        //[/UserComboBoxCode_comboBoxPresets]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SourceDefinitionComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    refresh();

    if (source == m_args.pPointSelection)
    {
        if (m_args.pPointSelection->getSelectionMode() == PointSelection::Point)
        {
            groupList->setSelectedRows(SparseSet<int>(), dontSendNotification);
            SparseSet<int> sel;
            for (int index : m_args.pPointSelection->getSelectedIndices())
            {
                sel.addRange(Range<int>(index, index + 1));
            }

            sourceList->setSelectedRows(sel, dontSendNotification);
        }
        else
        {
            sourceList->setSelectedRows(SparseSet<int>(), dontSendNotification);
            SparseSet<int> sel;
            for (int index : m_args.pPointSelection->getSelectedIndices())
            {
                sel.addRange(Range<int>(index, index + 1));
            }

            groupList->setSelectedRows(sel, dontSendNotification);
        }

        controlDimming();
    }
    else
    {
        sliderMasterGain->setValue(m_args.pSourceSet->getMasterGain(), dontSendNotification);
    }
}

void SourceDefinitionComponent::controlDimming() const
{
    buttonAdd->setEnabled(m_args.pSourceSet->size() < m_args.pAudioParams->sourceParams.size());
    buttonRemove->setEnabled(m_args.pPointSelection->getSelectionMode() == PointSelection::Point && m_args.pSourceSet->size() > 0);
    buttonMoveUp->setEnabled(!m_args.pSourceSet->getGroupModeFlag() && m_args.pPointSelection->getMainSelectedPointIndex() > 0);
    buttonMoveDown->setEnabled(!m_args.pSourceSet->getGroupModeFlag() && m_args.pPointSelection->getSelectionMode() == PointSelection::Point && m_args.pPointSelection->getMainSelectedPointIndex() < m_args.pSourceSet->size() - 1);
    buttonAddGroup->setEnabled(true);
    buttonRemoveGroup->setEnabled(m_args.pPointSelection->getSelectionMode() == PointSelection::Group && m_args.pSourceSet->activeGroupCount() > 0);
}

void SourceDefinitionComponent::refresh() const
{
    sourceList->updateContent();
    sourceList->repaint();
    groupList->updateContent();
    groupList->repaint();
}

void SourceDefinitionComponent::updateEncodingUiElements()
{
    toggleGroupMode->setToggleState(m_args.pSourceSet->getGroupModeFlag(), dontSendNotification);
    toggleInfiniteDistance->setToggleState(m_args.pSourceSet->getDistanceScaler() == 0.0, dontSendNotification);
    labelDistanceScaler->setEnabled(!toggleInfiniteDistance->getToggleState());
    sliderDistanceScaler->setEnabled(!toggleInfiniteDistance->getToggleState());
    if(!toggleInfiniteDistance->getToggleState())
        sliderDistanceScaler->setValue(m_args.pSourceSet->getDistanceScaler(), dontSendNotification);
}

void SourceDefinitionComponent::initializePresets()
{
    comboBoxPresets->clear();

    int id = 1;
    for(File file : m_args.pPresetHelper->presetFiles)
    {
        comboBoxPresets->addItem(file.getFileNameWithoutExtension(), id++);
    }
}

void SourceDefinitionComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        initializePresets();
    }

    if(message == ACTION_MESSAGE_PRESET_CHANGED)
    {
        refresh();
        updateEncodingUiElements();
        m_args.pZoomSettings->Reset();
        controlDimming();
        sendChangeMessage();
    }
}


void SourceDefinitionComponent::mouseUp(const MouseEvent &event)
{
    if(event.mods.isCtrlDown() || event.mods.isRightButtonDown())
    {
        if(groupList->getScreenBounds().contains(event.getScreenPosition()))
        {
            auto rel = event.getScreenPosition()-groupList->getScreenPosition();
            int row = groupList->getRowContainingPosition(rel.x, rel.y);

            if(row != -1 && m_args.pPointSelection->getSelectionMode() == PointSelection::Group)
            {
                PopupMenu m;
                
                // reset stretch
                m.addItem(PopupMenu::Item("Reset Stretch")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("R", Colours::lightgrey, Colours::red, Colours::black, false)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->getGroup(index)->setStretch(1.0f);
                        }
                        groupList->updateContent();
                        groupList->repaint();
                    })
                );
                
                // reset rotation
                m.addItem(PopupMenu::Item("Reset Rotation")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("R", Colours::lightgrey, Colours::red, Colours::black, false)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->getGroup(index)->setRotation(Quaternion<double>(0.0, 0.0, 0.0, 1.0));
                        }
                        groupList->updateContent();
                        groupList->repaint();
                    })
                );
                
                // apply color to sources
                m.addItem(PopupMenu::Item("Apply color to sources")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("C", Colours::lightgrey, Colours::red, Colours::black, false)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->getGroup(index)->setChildrenColor();
                        }
                        groupList->updateContent();
                        groupList->repaint();
                        sourceList->updateContent();
                        sourceList->repaint();
                    })
                );
                
                m.show();
            }
        }
        else if(sourceList->getScreenBounds().contains(event.getScreenPosition()))
        {
            auto rel = event.getScreenPosition()-sourceList->getScreenPosition();
            int row = sourceList->getRowContainingPosition(rel.x, rel.y);

            if(row != -1 && m_args.pPointSelection->getSelectionMode() == PointSelection::Point)
            {
                bool multirow = m_args.pPointSelection->getSelectionMode() == PointSelection::SelectionMode::Point && m_args.pPointSelection->getSelectedIndices().size() > 1;
            
                PopupMenu m;
                
                m.addItem(PopupMenu::Item("Mute")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("M", Colours::lightgrey, Colours::red, Colours::black, false)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->get(index)->setMute(true, true);
                        }
                        sourceList->updateContent();
                        sourceList->repaint();
                    })
                );
                
                m.addItem(PopupMenu::Item("Unmute")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("M", Colours::lightgrey, Colours::red, Colours::black, true)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->get(index)->setMute(false, true);
                        }
                        sourceList->updateContent();
                        sourceList->repaint();
                    })
                );
                
                m.addSeparator();
                m.addItem(PopupMenu::Item("Solo On")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("S", Colours::lightgrey, Colours::blue, Colours::black, false)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->get(index)->setSolo(true);
                        }
                        sourceList->updateContent();
                        sourceList->repaint();
                    })
                );
                
                m.addItem(PopupMenu::Item("Solo Off")
                    .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("S", Colours::lightgrey, Colours::blue, Colours::black, true)))
                    .setAction([this](){
                        auto selection = m_args.pPointSelection->getSelectedIndices();
                        for (auto index : selection)
                        {
                            m_args.pSourceSet->get(index)->setSolo(false);
                        }
                        sourceList->updateContent();
                        sourceList->repaint();
                    })
                );
                
                if(multirow)
                {
                    // color
                    m.addSeparator();
                    m.addItem(PopupMenu::Item("Apply Color from CH " + String(row + 1))
                        .setImage(std::unique_ptr<Drawable>(LabelCreator::createColorField(m_args.pSourceSet->get(row)->getColor())))
                        .setAction([this, row](){
                            Colour color = m_args.pSourceSet->get(row)->getColor();
                            auto selection = m_args.pPointSelection->getSelectedIndices();
                            for (int i = 0; i < selection.size(); i++)
                            {
                                if (selection[i] != row)
                                {
                                    m_args.pSourceSet->get(selection[i])->setColor(color);
                                }
                            }
                            sourceList->updateContent();
                            sourceList->repaint();
                        })
                    );
                    
                    if(m_args.pDawParams->updateTrackPropertiesWorking)
                    {
                        m.addItem(PopupMenu::Item("Apply Track Color")
                            .setImage(std::unique_ptr<Drawable>(LabelCreator::createColorField(m_args.pDawParams->lastTrackProperties.colour)))
                            .setAction([this, row](){
                                Colour color = m_args.pDawParams->lastTrackProperties.colour;
                                auto selection = m_args.pPointSelection->getSelectedIndices();
                                for (int i = 0; i < selection.size(); i++)
                                {
                                    if (selection[i] != row)
                                    {
                                        m_args.pSourceSet->get(selection[i])->setColor(color);
                                    }
                                }
                                sourceList->updateContent();
                                sourceList->repaint();
                            })
                        );
                    }
                    // gain
                    m.addSeparator();
                    m.addItem(PopupMenu::Item("Apply Gain from CH " + String(row + 1) + " (" + String(Decibels::gainToDecibels(m_args.pSourceSet->get(row)->getGain()), 1) + " dB)")
                        .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("=", Colours::white, Colours::black, Colours::black, false)))
                        .setAction([this, row](){
                            double gainFactor = m_args.pSourceSet->get(row)->getGain();
                            auto selection = m_args.pPointSelection->getSelectedIndices();
                            for (int i = 0; i < selection.size(); i++)
                            {
                                if (selection[i] != row)
                                {
                                    m_args.pSourceSet->get(selection[i])->setGain(gainFactor, true);
                                }
                            }
                            sourceList->updateContent();
                            sourceList->repaint();
                        })
                    );

                    m.addItem(PopupMenu::Item("Gain -0.5 dB")
                        .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("-", Colours::white, Colours::black, Colours::black, false)))
                        .setAction([this](){
                            float gainFactor = float(Decibels::decibelsToGain(-0.5));
                            auto selection = m_args.pPointSelection->getSelectedIndices();
                            for (auto index : selection)
                            {
                                m_args.pSourceSet->get(index)->setGain(m_args.pSourceSet->get(index)->getGain() * gainFactor, true);
                            }
                            sourceList->updateContent();
                            sourceList->repaint();
                        })
                    );
                    
                    m.addItem(PopupMenu::Item("Gain +0.5 dB")
                        .setImage(std::unique_ptr<Drawable>(LabelCreator::createIcon("+", Colours::white, Colours::black, Colours::black, false)))
                        .setAction([this](){
                            float gainFactor = float(Decibels::decibelsToGain(0.5));
                            auto selection = m_args.pPointSelection->getSelectedIndices();
                            for (auto index : selection)
                            {
                                m_args.pSourceSet->get(index)->setGain(m_args.pSourceSet->get(index)->getGain() * gainFactor, true);
                            }
                            sourceList->updateContent();
                            sourceList->repaint();
                        })
                    );
                }
                
                m.show();
            }
        }
    }
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SourceDefinitionComponent"
                 componentName="" parentClasses="public Component, public ChangeListener, public ActionListener, public ChangeBroadcaster"
                 constructorParams="EncoderSettingsComponentArgs args" variableInitialisers="m_args(args)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupGroups" id="983b0a3b2c5c945a" memberName="groupGroups"
                  virtualName="" explicitFocusOrder="0" pos="0 0Rr 0M 43.432%"
                  posRelativeX="73249ab85d6bba3a" posRelativeY="862016958b331dfc"
                  posRelativeW="73249ab85d6bba3a" posRelativeH="862016958b331dfc"
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
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 56.568%" posRelativeX="73249ab85d6bba3a"
                  posRelativeY="862016958b331dfc" posRelativeW="73249ab85d6bba3a"
                  posRelativeH="862016958b331dfc" title="Sources"/>
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
  <SLIDER name="sliderDistanceScaler" id="86549d5794437a4a" memberName="sliderDistanceScaler"
          virtualName="" explicitFocusOrder="0" pos="98Rr 39 301M 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="1.0" max="1000.0" int="0.1"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelDistanceScaler" id="3db2cd25c7d2d40f" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="6 39 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="toggleInfiniteDistance" id="6a3353481b4b5310" memberName="toggleInfiniteDistance"
                virtualName="" explicitFocusOrder="0" pos="90R 39 72 24" buttonText="Infinite"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelMasterGain" id="5a6c2906ed7799ee" memberName="labelMasterGain"
         virtualName="" explicitFocusOrder="0" pos="6 69 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Master Gain [dB]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderMasterGain" id="48f17ace33ebcbca" memberName="sliderMasterGain"
          virtualName="" explicitFocusOrder="0" pos="98Rr 69 301M 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="0.0" max="36.0" int="0.1"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TOGGLEBUTTON name="toggleGroupMode" id="88b6a5b9193adc18" memberName="toggleGroupMode"
                virtualName="" explicitFocusOrder="0" pos="8 8 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Group Mode" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="comboBoxPresets" id="4b25adf5b07e9492" memberName="comboBoxPresets"
            virtualName="" explicitFocusOrder="0" pos="83 8Rr 290M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="8 8Rr 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="110Rr 8Rr 90 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonManagePresets" id="47314282f0cb05bc" memberName="buttonManagePresets"
              virtualName="" explicitFocusOrder="0" pos="8Rr 8Rr 90 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="dummyHeight" id="862016958b331dfc" memberName="dummyHeight"
                    virtualName="" explicitFocusOrder="0" pos="0 96 24 134M" class="juce::Component"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

