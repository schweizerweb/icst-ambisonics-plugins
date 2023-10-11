/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



//[Headers] You can add your own extra header files here...
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/TrackColors.h"
#include "../../Common/ZoomSettings.h"
#include "../../Common/LabelCreator.h"
//[/Headers]

#include "SourceDefinitionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#if JUCE_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif
//[/MiscUserDefs]

//==============================================================================
SourceDefinitionComponent::SourceDefinitionComponent (EncoderSettingsComponentArgs args)
    : m_args(args)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(m_args.pChangeListener);
    groupModel.reset(new GroupTableListModel(m_args.pSourceSet, m_args.pPointSelection, this, m_args.pZoomSettings->getScalingInfo()));
    sourceModel.reset(new SourceTableListModel(m_args.pSourceSet, m_args.pPointSelection, this, m_args.pZoomSettings->getScalingInfo(), m_args.pChannelLayout));
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

    labelDistanceScaler->setBounds (312, 8, 109, 24);

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

    labelMasterGain->setBounds (8, 40, 109, 24);

    sliderMasterGain.reset (new juce::Slider ("sliderMasterGain"));
    addAndMakeVisible (sliderMasterGain.get());
    sliderMasterGain->setRange (0, 36, 0.1);
    sliderMasterGain->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderMasterGain->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderMasterGain->addListener (this);

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

    labelAmbiOrder.reset (new juce::Label ("labelAmbiOrder",
                                           TRANS("Ambisonics order:")));
    addAndMakeVisible (labelAmbiOrder.get());
    labelAmbiOrder->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelAmbiOrder->setJustificationType (juce::Justification::centredLeft);
    labelAmbiOrder->setEditable (false, false, false);
    labelAmbiOrder->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelAmbiOrder->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelAmbiOrder->setBounds (6, 8, 136, 24);

    comboAmbiOrder.reset (new juce::ComboBox ("comboAmbiOrder"));
    addAndMakeVisible (comboAmbiOrder.get());
    comboAmbiOrder->setEditableText (false);
    comboAmbiOrder->setJustificationType (juce::Justification::centredLeft);
    comboAmbiOrder->setTextWhenNothingSelected (juce::String());
    comboAmbiOrder->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboAmbiOrder->addItem (TRANS("1st (4 ch)"), 1);
    comboAmbiOrder->addItem (TRANS("2nd (9 ch)"), 2);
    comboAmbiOrder->addItem (TRANS("3rd (16 ch)"), 3);
    comboAmbiOrder->addItem (TRANS("4th (25 ch)"), 4);
    comboAmbiOrder->addItem (TRANS("5th (36 ch)"), 5);
    comboAmbiOrder->addItem (TRANS("6th (49 ch)"), 6);
    comboAmbiOrder->addItem (TRANS("7th (64 ch)"), 7);
    comboAmbiOrder->addListener (this);


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

    handleAmbiOrders();

    initializePresets();

    sourceList->addMouseListener(this, true);
    groupList->addMouseListener(this, true);
    m_args.pChannelLayout->addActionListener(this);

    controlDimming();
    //[/Constructor]
}

SourceDefinitionComponent::~SourceDefinitionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    m_args.pChannelLayout->removeActionListener(this);
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
    comboBoxPresets = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    buttonManagePresets = nullptr;
    dummyHeight = nullptr;
    labelAmbiOrder = nullptr;
    comboAmbiOrder = nullptr;


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

    groupGroups->setBounds (0, 72 + (getHeight() - 110) - (juce::roundToInt ((getHeight() - 110) * 0.4349f)), getWidth() - 0, juce::roundToInt ((getHeight() - 110) * 0.4349f));
    groupList->setBounds (0 + 16, (72 + (getHeight() - 110) - (juce::roundToInt ((getHeight() - 110) * 0.4349f))) + 19, (getWidth() - 0) - 31, (juce::roundToInt ((getHeight() - 110) * 0.4349f)) - 67);
    buttonAddGroup->setBounds (0 + 17, (72 + (getHeight() - 110) - (juce::roundToInt ((getHeight() - 110) * 0.4349f))) + (juce::roundToInt ((getHeight() - 110) * 0.4349f)) - 40, 64, 24);
    buttonRemoveGroup->setBounds (0 + 89, (72 + (getHeight() - 110) - (juce::roundToInt ((getHeight() - 110) * 0.4349f))) + (juce::roundToInt ((getHeight() - 110) * 0.4349f)) - 40, 64, 24);
    groupSources->setBounds (0, 72 + 0, getWidth() - 0, juce::roundToInt ((getHeight() - 110) * 0.5651f));
    sourceList->setBounds (0 + 16, (72 + 0) + 19, (getWidth() - 0) - 31, (juce::roundToInt ((getHeight() - 110) * 0.5651f)) - 67);
    buttonAdd->setBounds (0 + 17, (72 + 0) + (juce::roundToInt ((getHeight() - 110) * 0.5651f)) - 40, 64, 24);
    buttonRemove->setBounds (0 + 89, (72 + 0) + (juce::roundToInt ((getHeight() - 110) * 0.5651f)) - 40, 64, 24);
    buttonMoveDown->setBounds (0 + (getWidth() - 0) - 80, (72 + 0) + (juce::roundToInt ((getHeight() - 110) * 0.5651f)) - 40, 64, 24);
    buttonMoveUp->setBounds (0 + (getWidth() - 0) - 152, (72 + 0) + (juce::roundToInt ((getHeight() - 110) * 0.5651f)) - 40, 64, 24);
    buttonMoveGroupDown->setBounds (0 + (getWidth() - 0) - 80, (72 + (getHeight() - 110) - (juce::roundToInt ((getHeight() - 110) * 0.4349f))) + (juce::roundToInt ((getHeight() - 110) * 0.4349f)) - 40, 64, 24);
    buttonMoveGroupUp->setBounds (0 + (getWidth() - 0) - 152, (72 + (getHeight() - 110) - (juce::roundToInt ((getHeight() - 110) * 0.4349f))) + (juce::roundToInt ((getHeight() - 110) * 0.4349f)) - 40, 64, 24);
    sliderDistanceScaler->setBounds (getWidth() - 93 - (getWidth() - 525), 8, getWidth() - 525, 24);
    toggleInfiniteDistance->setBounds (getWidth() - 81, 8, 72, 24);
    sliderMasterGain->setBounds (getWidth() - 5 - (getWidth() - 141), 40, getWidth() - 141, 24);
    comboBoxPresets->setBounds (83, getHeight() - 8 - 24, getWidth() - 290, 24);
    labelPresets->setBounds (8, getHeight() - 8 - 24, 64, 24);
    buttonSave->setBounds (getWidth() - 110 - 90, getHeight() - 8 - 24, 90, 24);
    buttonManagePresets->setBounds (getWidth() - 8 - 90, getHeight() - 8 - 24, 90, 24);
    dummyHeight->setBounds (0, 72, 24, getHeight() - 110);
    comboAmbiOrder->setBounds (280 - 144, 8, 144, 24);
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
        m_args.pSourceSet->setDistanceScaler(toggleInfiniteDistance->getToggleState() ? ScalingInfo::Infinite : sliderDistanceScaler->getValue());
        m_args.pZoomSettings->getScalingInfo()->SetScaler(m_args.pSourceSet->getDistanceScaler());
        refresh();
        updateEncodingUiElements();
        //[/UserButtonCode_toggleInfiniteDistance]
    }
    else if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = m_args.pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
            return;

        m_args.pPresetHelper->writeToXmlFile(*newFile, m_args.pSourceSet);
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
    else if (comboBoxThatHasChanged == comboAmbiOrder.get())
    {
        //[UserComboBoxCode_comboAmbiOrder] -- add your combo box handling code here..
        m_args.pSettings->ambiOrder = comboAmbiOrder->getSelectedId();
        //[/UserComboBoxCode_comboAmbiOrder]
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
    toggleInfiniteDistance->setToggleState(exactlyEqual(m_args.pSourceSet->getDistanceScaler(), ScalingInfo::Infinite), dontSendNotification);
    labelDistanceScaler->setEnabled(!toggleInfiniteDistance->getToggleState());
    sliderDistanceScaler->setEnabled(!toggleInfiniteDistance->getToggleState());
    if(!toggleInfiniteDistance->getToggleState())
        sliderDistanceScaler->setValue(m_args.pSourceSet->getDistanceScaler(), dontSendNotification);
}

void SourceDefinitionComponent::handleAmbiOrders()
{
    for(int i = 1; i <= 7; i++)
        comboAmbiOrder->setItemEnabled(i, i <= m_args.pChannelLayout->getMaxAmbiOrder(true));
    comboAmbiOrder->setSelectedId(m_args.pSettings->ambiOrder, dontSendNotification);
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
    if(message == ACTION_MESSAGE_CHANNEL_LAYOUT_CHANGED)
    {
        handleAmbiOrders();
        refresh();
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
                  virtualName="" explicitFocusOrder="0" pos="0 0Rr 0M 43.495%"
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
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 56.505%" posRelativeX="73249ab85d6bba3a"
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
          virtualName="" explicitFocusOrder="0" pos="93Rr 8 525M 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="1.0" max="1000.0" int="0.1"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelDistanceScaler" id="3db2cd25c7d2d40f" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="312 8 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="toggleInfiniteDistance" id="6a3353481b4b5310" memberName="toggleInfiniteDistance"
                virtualName="" explicitFocusOrder="0" pos="81R 8 72 24" buttonText="Infinite"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelMasterGain" id="5a6c2906ed7799ee" memberName="labelMasterGain"
         virtualName="" explicitFocusOrder="0" pos="8 40 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Master Gain [dB]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderMasterGain" id="48f17ace33ebcbca" memberName="sliderMasterGain"
          virtualName="" explicitFocusOrder="0" pos="5Rr 40 141M 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="0.0" max="36.0" int="0.1"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
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
                    virtualName="" explicitFocusOrder="0" pos="0 72 24 110M" class="juce::Component"
                    params=""/>
  <LABEL name="labelAmbiOrder" id="2bcfcb77cb1da104" memberName="labelAmbiOrder"
         virtualName="" explicitFocusOrder="0" pos="6 8 136 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Ambisonics order:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboAmbiOrder" id="c5142a4672d3a6b3" memberName="comboAmbiOrder"
            virtualName="" explicitFocusOrder="0" pos="280r 8 144 24" editable="0"
            layout="33" items="1st (4 ch)&#10;2nd (9 ch)&#10;3rd (16 ch)&#10;4th (25 ch)&#10;5th (36 ch)&#10;6th (49 ch)&#10;7th (64 ch)"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

