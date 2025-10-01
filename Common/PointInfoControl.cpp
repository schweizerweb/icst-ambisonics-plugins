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
#include "RadarOptions.h"
#include "TrackColors.h"
#include "Constants.h"
#include "ColorDefinition.h"
//[/Headers]

#include "PointInfoControl.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#if JUCE_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif
//[/MiscUserDefs]

//==============================================================================
PointInfoControl::PointInfoControl (AmbiDataSet* _pEditablePoints, PointSelection* _pPointSelection, RadarOptions* _pRadarOptions)
    : pEditablePoints(_pEditablePoints), pPointSelection(_pPointSelection), pRadarOptions(_pRadarOptions)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    helperSlider.reset (new juce::Component());
    addAndMakeVisible (helperSlider.get());
    helperSlider->setName ("helperSlider");

    labelBackground.reset (new juce::Label ("labelBackground",
                                            juce::String()));
    addAndMakeVisible (labelBackground.get());
    labelBackground->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelBackground->setJustificationType (juce::Justification::centredLeft);
    labelBackground->setEditable (false, false, false);
    labelBackground->setColour (juce::Label::backgroundColourId, juce::Colour (0xabf0ffff));
    labelBackground->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelBackground->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelBackground->setBounds (7, 7, 90, 28);

    textName.reset (new juce::TextEditor ("textName"));
    addAndMakeVisible (textName.get());
    textName->setMultiLine (false);
    textName->setReturnKeyStartsNewLine (false);
    textName->setReadOnly (true);
    textName->setScrollbarsShown (true);
    textName->setCaretVisible (false);
    textName->setPopupMenuEnabled (true);
    textName->setText (juce::String());

    labelName.reset (new juce::Label ("labelName",
                                      TRANS("Name:")));
    addAndMakeVisible (labelName.get());
    labelName->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelName->setJustificationType (juce::Justification::centredRight);
    labelName->setEditable (false, false, false);
    labelName->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelName->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelName->setBounds (177, 7, 56, 24);

    textX.reset (new juce::TextEditor ("textX"));
    addAndMakeVisible (textX.get());
    textX->setMultiLine (false);
    textX->setReturnKeyStartsNewLine (false);
    textX->setReadOnly (false);
    textX->setScrollbarsShown (true);
    textX->setCaretVisible (true);
    textX->setPopupMenuEnabled (true);
    textX->setText (juce::String());

    textY.reset (new juce::TextEditor ("textY"));
    addAndMakeVisible (textY.get());
    textY->setMultiLine (false);
    textY->setReturnKeyStartsNewLine (false);
    textY->setReadOnly (false);
    textY->setScrollbarsShown (true);
    textY->setCaretVisible (true);
    textY->setPopupMenuEnabled (true);
    textY->setText (juce::String());

    textZ.reset (new juce::TextEditor ("textZ"));
    addAndMakeVisible (textZ.get());
    textZ->setMultiLine (false);
    textZ->setReturnKeyStartsNewLine (false);
    textZ->setReadOnly (false);
    textZ->setScrollbarsShown (true);
    textZ->setCaretVisible (true);
    textZ->setPopupMenuEnabled (true);
    textZ->setText (juce::String());

    labelCartesian.reset (new juce::Label ("labelCartesian",
                                           TRANS("Cartesian XYZ:")));
    addAndMakeVisible (labelCartesian.get());
    labelCartesian->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelCartesian->setJustificationType (juce::Justification::centredLeft);
    labelCartesian->setEditable (false, false, false);
    labelCartesian->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCartesian->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCartesian->setBounds (0, 40, 104, 24);

    textA.reset (new juce::TextEditor ("textA"));
    addAndMakeVisible (textA.get());
    textA->setMultiLine (false);
    textA->setReturnKeyStartsNewLine (false);
    textA->setReadOnly (false);
    textA->setScrollbarsShown (true);
    textA->setCaretVisible (true);
    textA->setPopupMenuEnabled (true);
    textA->setText (juce::String());

    textE.reset (new juce::TextEditor ("textE"));
    addAndMakeVisible (textE.get());
    textE->setMultiLine (false);
    textE->setReturnKeyStartsNewLine (false);
    textE->setReadOnly (false);
    textE->setScrollbarsShown (true);
    textE->setCaretVisible (true);
    textE->setPopupMenuEnabled (true);
    textE->setText (juce::String());

    textD.reset (new juce::TextEditor ("textD"));
    addAndMakeVisible (textD.get());
    textD->setMultiLine (false);
    textD->setReturnKeyStartsNewLine (false);
    textD->setReadOnly (false);
    textD->setScrollbarsShown (true);
    textD->setCaretVisible (true);
    textD->setPopupMenuEnabled (true);
    textD->setText (juce::String());

    labelPolar.reset (new juce::Label ("labelPolar",
                                       TRANS("Polar AED:")));
    addAndMakeVisible (labelPolar.get());
    labelPolar->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelPolar->setJustificationType (juce::Justification::centredLeft);
    labelPolar->setEditable (false, false, false);
    labelPolar->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPolar->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPolar->setBounds (0, 72, 80, 24);

    labelCH.reset (new juce::Label ("labelCH",
                                    TRANS("CH:")));
    addAndMakeVisible (labelCH.get());
    labelCH->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelCH->setJustificationType (juce::Justification::centredLeft);
    labelCH->setEditable (false, false, false);
    labelCH->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCH->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCH->setBounds (104, 7, 32, 24);

    textCH.reset (new juce::TextEditor ("textCH"));
    addAndMakeVisible (textCH.get());
    textCH->setMultiLine (false);
    textCH->setReturnKeyStartsNewLine (false);
    textCH->setReadOnly (true);
    textCH->setScrollbarsShown (true);
    textCH->setCaretVisible (false);
    textCH->setPopupMenuEnabled (true);
    textCH->setText (juce::String());

    btnUngroup.reset (new juce::ImageButton ("btnUngroup"));
    addAndMakeVisible (btnUngroup.get());
    btnUngroup->setButtonText (TRANS("ungroup"));
    btnUngroup->addListener (this);

    btnUngroup->setImages (false, true, true,
                           juce::ImageCache::getFromMemory (BinaryData::objects_ungroup_png, BinaryData::objects_ungroup_pngSize), 1.000f, juce::Colour (0x00000000),
                           juce::ImageCache::getFromMemory (BinaryData::objects_ungroup_png, BinaryData::objects_ungroup_pngSize), 1.000f, juce::Colour (0xd7bb0000),
                           juce::ImageCache::getFromMemory (BinaryData::objects_ungroup_png, BinaryData::objects_ungroup_pngSize), 1.000f, juce::Colours::blue);
    btnGroup.reset (new juce::ImageButton ("btnGroup"));
    addAndMakeVisible (btnGroup.get());
    btnGroup->setButtonText (TRANS("group"));
    btnGroup->addListener (this);

    btnGroup->setImages (false, true, true,
                         juce::ImageCache::getFromMemory (BinaryData::objects_group_png, BinaryData::objects_group_pngSize), 1.000f, juce::Colour (0x00000000),
                         juce::ImageCache::getFromMemory (BinaryData::objects_group_png, BinaryData::objects_group_pngSize), 1.000f, juce::Colour (0xd7bb0000),
                         juce::ImageCache::getFromMemory (BinaryData::objects_group_png, BinaryData::objects_group_pngSize), 1.000f, juce::Colours::blue);
    btnDelete.reset (new juce::ImageButton ("btnDelete"));
    addAndMakeVisible (btnDelete.get());
    btnDelete->setButtonText (TRANS("delete"));
    btnDelete->addListener (this);

    btnDelete->setImages (false, true, true,
                          juce::ImageCache::getFromMemory (BinaryData::trash_bin_png, BinaryData::trash_bin_pngSize), 1.000f, juce::Colour (0x00000000),
                          juce::ImageCache::getFromMemory (BinaryData::trash_bin_png, BinaryData::trash_bin_pngSize), 1.000f, juce::Colour (0xd7bb0000),
                          juce::ImageCache::getFromMemory (BinaryData::trash_bin_png, BinaryData::trash_bin_pngSize), 1.000f, juce::Colours::blue);

    //[UserPreSize]
    textX->setJustification(Justification::centredRight);
    textY->setJustification(Justification::centredRight);
    textZ->setJustification(Justification::centredRight);
    textA->setJustification(Justification::centredRight);
    textE->setJustification(Justification::centredRight);
    textD->setJustification(Justification::centredRight);
    textName->setSelectAllWhenFocused(true);
    textX->setSelectAllWhenFocused(true);
    textY->setSelectAllWhenFocused(true);
    textZ->setSelectAllWhenFocused(true);
    textA->setSelectAllWhenFocused(true);
    textE->setSelectAllWhenFocused(true);
    textD->setSelectAllWhenFocused(true);
    //[/UserPreSize]

    setSize (390, 100);


    //[Constructor] You can add your own custom stuff here..
	textCH->setVisible(pRadarOptions->maxNumberEditablePoints > 1);
	labelCH->setVisible(pRadarOptions->maxNumberEditablePoints > 1);
	updateSelectedPoint();
	pPointSelection->addChangeListener(this);
    //[/Constructor]
}

PointInfoControl::~PointInfoControl()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPointSelection->removeChangeListener(this);
    //[/Destructor_pre]

    helperSlider = nullptr;
    labelBackground = nullptr;
    textName = nullptr;
    labelName = nullptr;
    textX = nullptr;
    textY = nullptr;
    textZ = nullptr;
    labelCartesian = nullptr;
    textA = nullptr;
    textE = nullptr;
    textD = nullptr;
    labelPolar = nullptr;
    labelCH = nullptr;
    textCH = nullptr;
    btnUngroup = nullptr;
    btnGroup = nullptr;
    btnDelete = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PointInfoControl::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PointInfoControl::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    helperSlider->setBounds (111, 40, getWidth() - 117, getHeight() - 44);
    textName->setBounds (242, 7, getWidth() - 253, 24);
    textX->setBounds (111 + 0, 40 + 0, juce::roundToInt ((getWidth() - 117) * 0.3223f), 24);
    textY->setBounds (111 + juce::roundToInt ((getWidth() - 117) * 0.3297f), 40 + 0, juce::roundToInt ((getWidth() - 117) * 0.3223f), 24);
    textZ->setBounds (111 + juce::roundToInt ((getWidth() - 117) * 0.6593f), 40 + 0, juce::roundToInt ((getWidth() - 117) * 0.3223f), 24);
    textA->setBounds (111 + juce::roundToInt ((getWidth() - 117) * 0.0000f), 40 + (getHeight() - 44) - 24, juce::roundToInt ((getWidth() - 117) * 0.3223f), 24);
    textE->setBounds (111 + juce::roundToInt ((getWidth() - 117) * 0.3297f), 40 + (getHeight() - 44) - 24, juce::roundToInt ((getWidth() - 117) * 0.3223f), 24);
    textD->setBounds (111 + juce::roundToInt ((getWidth() - 117) * 0.6593f), 40 + (getHeight() - 44) - 24, juce::roundToInt ((getWidth() - 117) * 0.3223f), 24);
    textCH->setBounds (140, 7, 24, 24);
    btnUngroup->setBounds (7 + 61, 7 + 1, 26, 26);
    btnGroup->setBounds (7 + 30, 7 + 1, 26, 26);
    btnDelete->setBounds (7 + 1, 7 + 1, 26, 26);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PointInfoControl::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnUngroup.get())
    {
        //[UserButtonCode_btnUngroup] -- add your button handler code here..
        if (pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int indexToRemove = pPointSelection->getMainSelectedPointIndex();
            pPointSelection->unselectPoint();
            pEditablePoints->removeGroup(indexToRemove);
        }
        //[/UserButtonCode_btnUngroup]
    }
    else if (buttonThatWasClicked == btnGroup.get())
    {
        //[UserButtonCode_btnGroup] -- add your button handler code here..
        Array<int> selection = pPointSelection->getSelectedIndices();
        if (pPointSelection->getSelectionMode() == PointSelection::Point && selection.size() > 1)
        {
            double minX = pRadarOptions->scalingInfo->CartesianMax();
            double maxX = pRadarOptions->scalingInfo->CartesianMin();
            double minY = pRadarOptions->scalingInfo->CartesianMax();
            double maxY = pRadarOptions->scalingInfo->CartesianMin();
            double minZ = pRadarOptions->scalingInfo->CartesianMax();
            double maxZ = pRadarOptions->scalingInfo->CartesianMin();

            for (int i : selection)
            {
                auto origPos = pEditablePoints->getAbsSourcePoint(i);
                minX = jmin(minX, origPos.x);
                maxX = jmax(maxX, origPos.x);
                minY = jmin(minY, origPos.y);
                maxY = jmax(maxY, origPos.y);
                minZ = jmin(minZ, origPos.z);
                maxZ = jmax(maxZ, origPos.z);
            }
            Point<double> centerXY = Rectangle<double>(minX, minY, maxX - minX, maxY - minY).getCentre();
            double centerZ = (minZ + maxZ) / 2.0;

            int newIndex = pEditablePoints->addGroup(Uuid().toString(), Vector3D<double>(centerXY.getX(), centerXY.getY(), centerZ), "G", COLOR_DEFINITION_GROUP_DEFAULT);
            for (int i : selection)
            {
                auto origPos = pEditablePoints->getAbsSourcePoint(i);
                pEditablePoints->getGroup(newIndex)->addPointToGroup(pEditablePoints->get(i));
                pEditablePoints->setAbsSourcePoint(i, origPos);
            }

            pPointSelection->selectGroup(newIndex, false);
        }
        //[/UserButtonCode_btnGroup]
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
        if(pPointSelection->getSelectionMode() == PointSelection::SelectionMode::Point)
        {
            for(auto i : pPointSelection->getSelectedIndices())
                pEditablePoints->setEnabled(i, false);
        }
        else if(pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            for(auto g : pPointSelection->getSelectedIndices())
            {
                // delete all children
                for(int i = 0; i < pEditablePoints->size(); i++)
                {
                    if(pEditablePoints->get(i)->getGroup() == pEditablePoints->getGroup(g))
                        pEditablePoints->get(i)->setEnabled(false);
                }
                
                // finally delete the group
                pEditablePoints->removeGroup(g);
            }
        }
        pPointSelection->unselectPoint();
        //[/UserButtonCode_btnDelete]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PointInfoControl::updateSelectedPoint(String exceptField)
{
	disableListeners();

	int selection = pPointSelection->getMainSelectedPointIndex();
	if (pPointSelection->getSelectionMode() != PointSelection::None && selection >= 0 && selection < pEditablePoints->size())
	{
		AmbiPoint* point = pPointSelection->getSelectionMode() == PointSelection::Group ? pEditablePoints->getGroup(selection) : pEditablePoints->get(selection);

		setFieldsEnabled(true);
		textName->setText(point->getName());
		textCH->setText(String(selection + 1));
		if (exceptField != textX->getName()) textX->setText(String(point->getRawPoint()->getX(), 3));
		if (exceptField != textY->getName()) textY->setText(String(point->getRawPoint()->getY(), 3));
		if (exceptField != textZ->getName()) textZ->setText(String(point->getRawPoint()->getZ(), 3));
		if (exceptField != textA->getName()) textA->setText(String(Constants::RadToGrad(point->getRawPoint()->getAzimuth()), 2));
		if (exceptField != textE->getName()) textE->setText(String(Constants::RadToGrad(point->getRawPoint()->getElevation()), 2));
		if (exceptField != textD->getName()) textD->setText(String(point->getRawPoint()->getDistance(), 3));
	}
	else
	{
		setFieldsEnabled(false);
		textName->setText("-");
		textCH->setText("-");
		textX->setText("-");
		textY->setText("-");
		textZ->setText("-");
		textA->setText("-");
		textE->setText("-");
		textD->setText("-");
	}

    bool enGroup = pRadarOptions->allowGroup && pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getSelectedIndices().size() > 1
    && ( pRadarOptions->audioParams == nullptr || pEditablePoints->activeGroupCount() < pRadarOptions->audioParams->groupParams.size());
    bool enUngroup = pRadarOptions->allowGroup && pPointSelection->getSelectionMode() == PointSelection::Group && pPointSelection->getSelectedIndices().size() == 1;
    bool enDelete = pRadarOptions->allowDelete && pPointSelection->getSelectedIndices().size() > 0;
	btnGroup->setEnabled(enGroup);
	btnUngroup->setEnabled(enUngroup);
    btnDelete->setEnabled(enDelete);
    bool show = enGroup || enUngroup || enDelete;
    btnGroup->setVisible(show);
    btnUngroup->setVisible(show);
    btnDelete->setVisible(show);
    labelBackground->setVisible(show);
    bool makeReadOnly = pPointSelection->getSelectionMode() == PointSelection::None;
	textX->setReadOnly(makeReadOnly);
	textY->setReadOnly(makeReadOnly);
	textZ->setReadOnly(makeReadOnly);
	textA->setReadOnly(makeReadOnly);
	textE->setReadOnly(makeReadOnly);
	textD->setReadOnly(makeReadOnly);
    if(pRadarOptions->checkNameFieldEditable)
    {
        makeReadOnly = pRadarOptions->dawParameter->updateTrackPropertiesWorking;
    }
    textName->setReadOnly(makeReadOnly);
	enableListeners();
}

void PointInfoControl::changeListenerCallback(ChangeBroadcaster* source)
{
	if(source == pPointSelection)
		updateSelectedPoint();
}

void PointInfoControl::textEditorTextChanged(TextEditor& source)
{
	int selection = pPointSelection->getMainSelectedPointIndex();

	if(pPointSelection->getSelectionMode() == PointSelection::Group)
	{
		if (source.getName() == textName->getName())
		{
			pEditablePoints->setGroupName(selection, textName->getText());
		}
        
        if (source.getName() == textX->getName())
        {
            pEditablePoints->setGroupX(selection, textX->getText().getFloatValue());
        }
        if (source.getName() == textY->getName())
        {
            pEditablePoints->setGroupY(selection, textY->getText().getFloatValue());
        }
        if (source.getName() == textZ->getName())
        {
            pEditablePoints->setGroupZ(selection, textZ->getText().getFloatValue());
        }

        if (source.getName() == textA->getName())
        {
            pEditablePoints->setGroupAed(selection, Constants::GradToRad(textA->getText().getFloatValue()), pEditablePoints->getGroup(selection)->getRawPoint()->getElevation(), pEditablePoints->getGroup(selection)->getRawPoint()->getDistance(), pEditablePoints->getGroupModeFlag());
        }
        if (source.getName() == textE->getName())
        {
            pEditablePoints->setGroupAed(selection, pEditablePoints->getGroup(selection)->getRawPoint()->getAzimuth(), Constants::GradToRad(textE->getText().getFloatValue()), pEditablePoints->getGroup(selection)->getRawPoint()->getDistance(), pEditablePoints->getGroupModeFlag());
        }
        if (source.getName() == textD->getName())
        {
            pEditablePoints->setGroupAed(selection, pEditablePoints->getGroup(selection)->getRawPoint()->getAzimuth(), pEditablePoints->getGroup(selection)->getRawPoint()->getElevation(), textD->getText().getFloatValue(), pEditablePoints->getGroupModeFlag());
        }
	}
	else
	{
		if (source.getName() == textName->getName())
		{
			pEditablePoints->setChannelName(selection, textName->getText());
			if (pRadarOptions->setTrackColorAccordingToName)
			{
				pEditablePoints->setChannelColor(selection, TrackColors::getColor(textName->getText().initialSectionContainingOnly("0123456789").getIntValue()));
			}
		}

		if (source.getName() == textX->getName())
		{
			pEditablePoints->setX(selection, textX->getText().getFloatValue());
		}
		if (source.getName() == textY->getName())
		{
			pEditablePoints->setY(selection, textY->getText().getFloatValue());
		}
		if (source.getName() == textZ->getName())
		{
			pEditablePoints->setZ(selection, textZ->getText().getFloatValue());
		}

		if (source.getName() == textA->getName())
		{
			pEditablePoints->setAzimuth(selection, Constants::GradToRad(textA->getText().getFloatValue()));
		}
		if (source.getName() == textE->getName())
		{
			pEditablePoints->setElevation(selection, Constants::GradToRad(textE->getText().getFloatValue()));
		}
		if (source.getName() == textD->getName())
		{
			pEditablePoints->setDistance(selection, textD->getText().getFloatValue());
		}
	}

	updateSelectedPoint(source.getName());
}

void PointInfoControl::disableListeners()
{
	textName->removeListener(this);
	textX->removeListener(this);
	textY->removeListener(this);
	textZ->removeListener(this);
	textA->removeListener(this);
	textE->removeListener(this);
	textD->removeListener(this);
}

void PointInfoControl::enableListeners()
{
	textName->addListener(this);
	textX->addListener(this);
	textY->addListener(this);
	textZ->addListener(this);
	textA->addListener(this);
	textE->addListener(this);
	textD->addListener(this);
}

void PointInfoControl::setFieldsEnabled(bool enable) const
{
	textName->setEnabled(enable);
	textX->setEnabled(enable);
	textY->setEnabled(enable);
	textZ->setEnabled(enable);
	textA->setEnabled(enable);
	textE->setEnabled(enable);
	textD->setEnabled(enable);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PointInfoControl" componentName=""
                 parentClasses="public Component, public ChangeListener, public TextEditor::Listener"
                 constructorParams="AmbiDataSet* _pEditablePoints, PointSelection* _pPointSelection, RadarOptions* _pRadarOptions"
                 variableInitialisers="pEditablePoints(_pEditablePoints), pPointSelection(_pPointSelection), pRadarOptions(_pRadarOptions)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="390" initialHeight="100">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="helperSlider" id="81eff694bb45ed02" memberName="helperSlider"
                    virtualName="" explicitFocusOrder="0" pos="111 40 117M 44M" class="juce::Component"
                    params=""/>
  <LABEL name="labelBackground" id="13e9adb32fa37aa3" memberName="labelBackground"
         virtualName="" explicitFocusOrder="0" pos="7 7 90 28" bkgCol="abf0ffff"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textName" id="2b706cdb3232f1d2" memberName="textName" virtualName=""
              explicitFocusOrder="0" pos="242 7 253M 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <LABEL name="labelName" id="7326de7683af8e2f" memberName="labelName"
         virtualName="" explicitFocusOrder="0" pos="177 7 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="textX" id="4dcd5fc970cdce6c" memberName="textX" virtualName=""
              explicitFocusOrder="0" pos="0 0 32.234% 24" posRelativeX="81eff694bb45ed02"
              posRelativeY="81eff694bb45ed02" posRelativeW="81eff694bb45ed02"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textY" id="46b4702ec60c236a" memberName="textY" virtualName=""
              explicitFocusOrder="0" pos="32.967% 0 32.234% 24" posRelativeX="81eff694bb45ed02"
              posRelativeY="81eff694bb45ed02" posRelativeW="81eff694bb45ed02"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textZ" id="93f9644500f2da7f" memberName="textZ" virtualName=""
              explicitFocusOrder="0" pos="65.934% 0 32.234% 24" posRelativeX="81eff694bb45ed02"
              posRelativeY="81eff694bb45ed02" posRelativeW="81eff694bb45ed02"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelCartesian" id="f2af390154f0c032" memberName="labelCartesian"
         virtualName="" explicitFocusOrder="0" pos="0 40 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cartesian XYZ:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textA" id="e76184fec37afbfb" memberName="textA" virtualName=""
              explicitFocusOrder="0" pos="0% 0Rr 32.234% 24" posRelativeX="81eff694bb45ed02"
              posRelativeY="81eff694bb45ed02" posRelativeW="81eff694bb45ed02"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textE" id="7fb39984576e614e" memberName="textE" virtualName=""
              explicitFocusOrder="0" pos="32.967% 0Rr 32.234% 24" posRelativeX="81eff694bb45ed02"
              posRelativeY="81eff694bb45ed02" posRelativeW="81eff694bb45ed02"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="textD" id="411a4503627ed096" memberName="textD" virtualName=""
              explicitFocusOrder="0" pos="65.934% 0Rr 32.234% 24" posRelativeX="81eff694bb45ed02"
              posRelativeY="81eff694bb45ed02" posRelativeW="81eff694bb45ed02"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="labelPolar" id="41fcd21e0ce12407" memberName="labelPolar"
         virtualName="" explicitFocusOrder="0" pos="0 72 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Polar AED:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCH" id="6db02d6b63396cba" memberName="labelCH" virtualName=""
         explicitFocusOrder="0" pos="104 7 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="CH:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textCH" id="d9bdcb7a01c25c33" memberName="textCH" virtualName=""
              explicitFocusOrder="0" pos="140 7 24 24" posRelativeW="411a4503627ed096"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="1" caret="0" popupmenu="1"/>
  <IMAGEBUTTON name="btnUngroup" id="fd1af95da982646a" memberName="btnUngroup"
               virtualName="" explicitFocusOrder="0" pos="61 1 26 26" posRelativeX="13e9adb32fa37aa3"
               posRelativeY="13e9adb32fa37aa3" buttonText="ungroup" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal="ungroupobjectsicon_png"
               opacityNormal="1.0" colourNormal="0" resourceOver="ungroupobjectsicon_png"
               opacityOver="1.0" colourOver="d7bb0000" resourceDown="ungroupobjectsicon_png"
               opacityDown="1.0" colourDown="ff0000ff"/>
  <IMAGEBUTTON name="btnGroup" id="ac2c254d3566960d" memberName="btnGroup" virtualName=""
               explicitFocusOrder="0" pos="30 1 26 26" posRelativeX="13e9adb32fa37aa3"
               posRelativeY="13e9adb32fa37aa3" buttonText="group" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal="groupobjectsicon_png"
               opacityNormal="1.0" colourNormal="0" resourceOver="groupobjectsicon_png"
               opacityOver="1.0" colourOver="d7bb0000" resourceDown="groupobjectsicon_png"
               opacityDown="1.0" colourDown="ff0000ff"/>
  <IMAGEBUTTON name="btnDelete" id="6cbf3f11936240c0" memberName="btnDelete"
               virtualName="" explicitFocusOrder="0" pos="1 1 26 26" posRelativeX="13e9adb32fa37aa3"
               posRelativeY="13e9adb32fa37aa3" posRelativeH="13e9adb32fa37aa3"
               buttonText="delete" connectedEdges="0" needsCallback="1" radioGroupId="0"
               keepProportions="1" resourceNormal="trashbinicon_png" opacityNormal="1.0"
               colourNormal="0" resourceOver="trashbinicon_png" opacityOver="1.0"
               colourOver="d7bb0000" resourceDown="trashbinicon_png" opacityDown="1.0"
               colourDown="ff0000ff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
