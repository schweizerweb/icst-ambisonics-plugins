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
    labelBackground->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
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
    labelName->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
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
    labelCartesian->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
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
    labelPolar->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
    labelPolar->setJustificationType (juce::Justification::centredLeft);
    labelPolar->setEditable (false, false, false);
    labelPolar->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPolar->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPolar->setBounds (0, 72, 80, 24);

    labelCH.reset (new juce::Label ("labelCH",
                                    TRANS("CH:")));
    addAndMakeVisible (labelCH.get());
    labelCH->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
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
                           juce::ImageCache::getFromMemory (ungroupobjectsicon_png, ungroupobjectsicon_pngSize), 1.000f, juce::Colour (0x00000000),
                           juce::ImageCache::getFromMemory (ungroupobjectsicon_png, ungroupobjectsicon_pngSize), 1.000f, juce::Colour (0xd7bb0000),
                           juce::ImageCache::getFromMemory (ungroupobjectsicon_png, ungroupobjectsicon_pngSize), 1.000f, juce::Colours::blue);
    btnGroup.reset (new juce::ImageButton ("btnGroup"));
    addAndMakeVisible (btnGroup.get());
    btnGroup->setButtonText (TRANS("group"));
    btnGroup->addListener (this);

    btnGroup->setImages (false, true, true,
                         juce::ImageCache::getFromMemory (groupobjectsicon_png, groupobjectsicon_pngSize), 1.000f, juce::Colour (0x00000000),
                         juce::ImageCache::getFromMemory (groupobjectsicon_png, groupobjectsicon_pngSize), 1.000f, juce::Colour (0xd7bb0000),
                         juce::ImageCache::getFromMemory (groupobjectsicon_png, groupobjectsicon_pngSize), 1.000f, juce::Colours::blue);
    btnDelete.reset (new juce::ImageButton ("btnDelete"));
    addAndMakeVisible (btnDelete.get());
    btnDelete->setButtonText (TRANS("delete"));
    btnDelete->addListener (this);

    btnDelete->setImages (false, true, true,
                          juce::ImageCache::getFromMemory (trashbinicon_png, trashbinicon_pngSize), 1.000f, juce::Colour (0x00000000),
                          juce::ImageCache::getFromMemory (trashbinicon_png, trashbinicon_pngSize), 1.000f, juce::Colour (0xd7bb0000),
                          juce::ImageCache::getFromMemory (trashbinicon_png, trashbinicon_pngSize), 1.000f, juce::Colours::blue);

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

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: group_png, 482, "../Resources/Group.png"
static const unsigned char resource_PointInfoControl_group_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,24,0,0,0,24,8,6,0,0,0,224,119,61,248,0,0,1,169,73,68,65,84,120,94,181,149,193,74,
195,64,16,134,103,76,136,55,189,74,239,125,9,95,160,52,82,240,33,90,168,80,240,98,139,167,30,234,69,72,105,161,189,104,159,194,147,245,212,67,31,193,107,223,160,13,68,197,170,77,211,174,195,176,176,139,
144,117,132,248,195,71,114,217,249,103,126,38,89,248,111,33,1,97,179,23,33,226,21,20,40,165,84,255,241,174,123,205,46,103,23,55,138,30,135,132,7,133,200,212,244,193,40,35,20,129,76,78,99,140,92,153,109,
176,111,52,26,151,203,229,114,152,166,41,216,242,60,15,74,165,82,111,50,153,220,2,64,74,236,64,166,157,109,0,171,213,106,24,69,17,148,203,101,176,181,88,44,160,221,110,119,1,224,158,72,136,47,2,156,147,
106,217,6,184,217,108,76,113,3,27,234,169,142,136,117,189,94,111,82,51,3,247,164,182,129,28,159,240,226,56,30,252,62,41,11,15,224,111,66,233,164,110,3,55,146,51,190,56,162,217,108,6,163,209,136,223,171,
213,234,51,104,213,106,53,104,181,90,80,169,84,114,38,21,26,140,199,99,112,228,205,6,226,113,131,32,224,131,182,4,121,139,178,98,146,36,57,239,116,58,15,84,20,228,146,27,168,249,124,254,68,207,19,226,
152,8,8,228,220,101,226,233,125,223,207,253,208,20,177,37,94,136,79,194,35,144,112,42,12,67,208,226,226,136,216,231,58,57,91,180,39,82,13,74,86,114,58,157,158,234,95,71,102,53,249,166,13,148,239,248,143,
40,16,137,139,199,196,135,110,112,103,77,160,220,29,154,205,114,229,157,17,107,226,85,243,206,166,174,239,64,176,89,118,222,169,54,217,51,90,78,3,247,102,177,240,71,222,91,19,169,196,192,189,89,70,38,
239,212,101,192,23,52,223,161,5,95,250,118,67,94,177,151,190,169,249,13,11,19,202,94,114,104,117,143,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PointInfoControl::group_png = (const char*) resource_PointInfoControl_group_png;
const int PointInfoControl::group_pngSize = 482;

// JUCER_RESOURCE: ungroup_png, 462, "../Resources/Ungroup.png"
static const unsigned char resource_PointInfoControl_ungroup_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,24,0,0,0,24,8,6,0,0,0,224,119,61,248,0,0,1,149,73,68,65,84,120,94,181,149,207,
78,194,64,16,198,191,110,87,52,141,122,86,145,35,15,83,53,225,45,236,205,11,129,35,7,15,94,56,225,133,224,83,144,16,77,76,120,19,94,0,244,104,81,91,105,235,180,89,157,148,205,166,127,8,191,228,75,152,
221,205,44,147,111,118,138,125,99,129,240,60,239,110,185,92,142,194,48,4,3,216,182,141,86,171,53,24,143,199,15,0,34,212,64,130,88,173,86,163,225,112,136,118,187,13,6,88,44,22,232,245,122,247,0,30,73,126,
221,75,224,186,110,146,50,1,50,17,185,24,192,57,233,128,227,12,152,99,70,96,139,219,36,201,253,158,186,238,255,57,15,176,82,129,48,197,216,66,106,201,75,98,242,110,74,186,177,237,228,207,59,137,154,148,
245,78,128,193,147,101,161,10,65,16,112,114,206,145,173,169,170,28,129,253,34,4,136,70,163,145,149,197,30,112,169,82,74,84,129,114,232,38,175,215,235,171,126,191,255,146,150,204,32,75,238,56,206,0,64,
140,234,240,5,243,249,252,21,192,201,4,248,240,128,38,242,237,22,145,62,139,31,25,123,192,85,112,155,198,42,73,202,59,116,162,186,85,88,48,80,102,62,117,58,157,141,169,77,187,221,46,102,179,217,165,220,
165,199,125,223,191,38,239,158,75,123,167,230,8,207,39,3,233,158,154,79,135,164,99,210,25,233,130,212,84,57,154,106,237,84,162,62,130,148,40,239,2,228,121,83,65,196,23,240,176,170,66,162,204,143,77,57,
4,246,140,208,60,216,145,130,239,129,58,192,143,38,147,22,243,217,112,251,131,83,250,29,80,143,39,69,61,174,204,252,41,24,21,58,37,231,211,87,253,151,204,123,71,234,79,8,189,123,176,33,125,23,141,144,
95,208,216,251,27,114,207,252,116,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PointInfoControl::ungroup_png = (const char*) resource_PointInfoControl_ungroup_png;
const int PointInfoControl::ungroup_pngSize = 462;

// JUCER_RESOURCE: groupobjectsicon_png, 827, "../Resources/group-objects-icon.png"
static const unsigned char resource_PointInfoControl_groupobjectsicon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,2,0,0,0,1,204,4,3,0,0,0,81,148,13,155,0,0,0,4,103,65,77,65,0,0,177,143,
11,252,97,5,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,0,18,80,76,84,69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,43,25,162,0,0,0,5,116,82,78,83,0,200,147,5,92,145,109,47,107,0,0,2,182,73,68,65,84,120,218,
237,221,65,78,194,64,24,134,225,150,112,128,18,241,2,132,30,128,88,247,200,9,180,202,253,175,226,210,144,252,139,9,118,154,233,204,243,238,229,175,79,190,157,128,221,41,232,220,101,110,31,93,189,230,190,
122,139,174,118,247,160,57,247,163,244,209,213,33,247,213,49,186,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,168,24,64,146,36,73,146,36,73,146,36,73,146,36,
73,146,36,73,146,36,73,146,164,182,219,29,50,119,44,28,32,252,216,227,146,253,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,32,192,247,18,47,185,7,0,0,64,211,0,147,5,88,128,5,88,128,5,88,
128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,
5,88,128,5,88,128,5,88,128,5,84,185,128,123,242,119,80,15,117,46,32,189,207,74,23,208,30,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,192,106,0,79,252,191,191,173,46,
224,35,250,93,187,232,101,231,58,23,48,70,175,80,8,192,212,58,128,5,88,128,5,36,254,113,180,111,97,1,13,2,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,5,88,128,
5,88,128,5,88,0,0,0,197,3,100,8,0,0,0,0,86,3,248,247,27,37,183,3,16,191,81,242,137,182,10,176,88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,11,96,151,252,157,167,175,15,79,251,149,250,99,199,194,1,
210,123,127,120,218,97,249,249,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,250,0,47,111,11,116,217,48,64,134,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,163,203,33,115,67,39,73,146,36,73,146,36,73,146,36,73,146,36,73,146,36,73,146,36,73,146,254,138,62,142,56,231,62,218,71,87,179,127,6,112,140,
174,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,6,56,5,157,115,63,202,20,93,189,230,190,122,139,174,254,2,131,24,81,84,170,168,136,212,0,0,0,0,73,69,78,68,
174,66,96,130,0,0};

const char* PointInfoControl::groupobjectsicon_png = (const char*) resource_PointInfoControl_groupobjectsicon_png;
const int PointInfoControl::groupobjectsicon_pngSize = 827;

// JUCER_RESOURCE: ungroupobjectsicon_png, 2514, "../Resources/ungroup-objects-icon.png"
static const unsigned char resource_PointInfoControl_ungroupobjectsicon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,2,0,0,0,1,194,4,3,0,0,0,107,158,108,235,0,0,0,4,103,65,77,65,0,0,177,
143,11,252,97,5,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,0,42,80,76,84,69,0,0,0,36,4,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,222,29,0,222,28,0,224,29,0,224,29,0,117,16,0,0,0,0,224,30,0,92,120,186,10,0,0,0,12,
116,82,78,83,0,25,169,213,71,110,243,207,49,95,148,234,91,224,157,69,0,0,9,46,73,68,65,84,120,218,237,157,59,111,27,71,20,133,151,64,140,93,110,69,231,1,53,22,64,164,86,17,195,72,111,4,74,151,42,78,58,
87,65,224,202,133,220,169,240,2,234,92,45,192,50,64,64,128,127,128,255,192,141,29,32,149,27,53,162,30,217,255,18,75,36,101,62,102,201,185,119,102,53,115,239,156,219,10,210,114,62,29,158,51,119,118,102,
55,251,100,168,97,214,105,189,52,92,242,67,183,151,252,219,112,201,143,243,31,153,0,252,208,237,167,57,122,112,0,69,243,150,4,96,168,13,192,97,51,75,91,1,85,211,92,164,172,128,188,249,92,175,19,86,192,
244,22,192,77,186,10,232,55,119,245,62,89,5,212,115,0,179,84,21,80,52,139,122,155,168,2,14,151,0,54,124,80,154,2,78,29,50,112,81,23,146,21,144,55,35,222,213,202,230,75,189,23,172,128,105,115,237,144,129,
139,186,145,171,128,219,40,155,56,100,160,193,7,101,41,224,54,202,102,3,126,6,54,219,81,40,74,1,243,40,99,248,96,209,172,215,133,80,5,44,162,236,140,124,173,131,13,0,43,81,40,74,1,139,40,187,114,200,192,
45,31,148,164,128,251,40,163,250,96,217,108,213,123,137,10,184,143,50,106,20,142,183,1,204,4,42,96,37,202,222,56,100,224,162,254,147,167,128,213,40,35,249,96,109,2,176,244,65,57,10,88,139,178,75,194,133,
122,198,241,47,163,80,142,2,14,215,62,62,161,37,200,205,0,22,62,40,71,1,235,81,70,240,193,170,5,192,141,44,5,108,70,217,137,67,6,174,250,160,24,5,76,55,115,204,182,37,24,183,2,184,243,65,41,10,216,142,
50,75,31,44,218,199,127,231,131,82,20,96,136,50,59,31,172,119,0,184,245,65,33,10,48,253,31,173,90,130,222,174,241,223,250,160,16,5,24,163,204,166,37,200,119,2,248,44,1,33,10,48,70,217,140,184,20,102,42,
33,10,104,137,178,253,75,35,229,158,241,55,23,50,20,208,22,101,123,91,130,241,62,0,205,31,18,20,208,111,251,244,251,124,176,216,59,254,230,70,130,2,218,163,108,226,144,129,107,70,18,181,2,118,68,217,238,
150,160,87,237,7,112,46,64,1,187,162,236,141,67,6,174,218,72,212,10,168,44,6,192,202,192,21,23,137,89,1,187,163,236,146,184,20,214,50,159,126,102,168,142,21,240,194,112,201,167,140,40,27,57,100,32,247,
78,227,131,86,193,29,131,69,6,218,175,42,4,172,154,59,8,251,12,140,186,122,251,71,49,200,232,222,73,95,91,13,85,57,119,24,132,12,140,186,166,4,43,231,102,96,204,85,90,140,223,56,16,155,12,156,8,0,48,182,
1,96,26,73,34,25,216,234,230,201,100,96,219,210,200,97,195,78,143,168,50,176,178,4,176,229,231,74,50,176,176,29,255,166,15,150,74,50,112,123,115,143,165,15,42,201,64,203,105,192,182,165,107,201,64,219,
121,192,214,210,72,173,35,3,9,19,129,245,239,180,150,12,36,250,224,37,37,3,155,129,20,0,217,59,107,2,35,101,25,72,158,11,92,107,203,64,178,15,78,180,101,32,213,7,231,147,91,69,25,72,72,245,149,111,182,
146,165,48,78,75,116,231,131,170,50,112,25,133,21,161,37,200,85,101,32,97,125,111,249,229,214,149,129,228,150,96,86,82,38,12,26,125,176,209,150,129,228,150,64,91,6,146,91,2,117,25,72,94,26,209,150,129,
100,31,212,212,6,240,90,2,117,25,232,215,7,71,98,1,248,241,193,235,76,110,213,201,102,32,121,105,68,93,6,250,243,193,115,209,0,60,68,225,153,108,0,253,116,51,208,147,15,142,164,3,40,170,100,51,144,188,
52,162,45,3,125,248,224,76,193,248,157,124,240,92,3,0,151,150,224,76,5,0,126,75,112,149,233,168,131,100,51,112,89,85,178,25,232,214,18,156,168,1,192,243,193,153,158,241,243,124,240,92,17,0,86,75,112,166,
9,0,99,105,228,42,83,85,101,186,25,200,108,9,174,149,141,159,220,18,156,104,3,64,244,193,217,64,29,0,218,210,200,101,166,175,242,100,51,144,225,131,87,26,199,79,241,193,137,74,0,172,141,228,186,124,48,
241,175,0,101,105,100,164,148,64,149,236,68,144,220,18,156,40,37,96,191,145,252,44,117,9,92,234,4,80,55,105,251,96,47,229,134,152,220,15,104,156,14,86,137,183,196,196,133,49,125,62,56,78,121,97,56,99,
220,30,208,214,18,208,239,14,232,242,193,30,121,252,202,162,144,179,93,232,141,38,0,172,221,66,138,124,144,119,147,92,145,15,50,247,10,169,105,9,184,91,133,212,248,32,123,211,172,146,165,145,30,119,252,
90,90,2,135,45,179,58,90,2,151,29,179,26,124,208,233,236,136,134,40,116,59,66,39,191,37,112,60,65,39,223,7,93,15,142,156,74,7,224,124,126,78,120,75,144,187,142,95,186,15,122,56,72,46,218,7,125,60,79,67,
116,75,224,229,24,185,224,165,17,79,79,211,144,235,131,135,169,63,72,161,242,3,64,108,75,144,123,26,63,217,7,191,254,190,211,26,62,96,6,242,150,70,126,249,212,105,217,190,210,197,227,51,165,136,45,65,
36,0,106,127,0,136,62,24,7,0,155,12,172,186,241,193,56,0,216,100,224,227,170,147,150,32,14,0,149,141,176,243,78,90,130,40,0,88,62,49,215,254,241,131,210,0,88,62,49,215,62,42,78,101,1,176,126,98,110,221,
65,75,16,3,0,235,39,230,118,177,145,60,2,0,132,39,230,30,248,247,193,8,0,216,140,106,64,157,49,95,11,2,80,17,38,119,188,55,51,196,13,128,246,196,92,239,7,106,194,3,24,147,44,173,240,221,18,4,7,64,125,
106,252,59,207,45,65,112,0,212,167,198,211,223,204,16,55,128,158,125,6,146,125,240,68,2,0,198,11,20,199,94,91,130,208,0,42,186,155,245,189,218,96,96,0,172,167,198,215,62,93,48,48,128,49,199,203,44,207,
216,75,48,193,130,55,171,207,61,182,3,97,1,112,223,156,50,245,101,129,129,1,216,100,224,57,119,246,116,46,0,0,255,37,194,99,29,189,192,148,29,101,133,175,86,96,3,192,239,238,245,171,61,0,151,55,167,28,
248,233,4,54,1,120,184,203,249,149,61,128,177,75,148,77,59,89,16,121,80,0,133,83,148,149,158,110,145,6,4,224,248,246,176,177,159,59,3,225,0,244,28,163,172,240,144,129,65,1,240,51,208,66,65,103,2,0,56,
191,64,113,199,210,8,225,246,104,48,0,125,247,118,174,244,113,131,60,102,5,236,141,178,169,135,125,66,225,0,236,151,192,222,40,235,123,216,38,20,113,12,90,68,89,237,124,119,60,232,68,168,114,110,231,204,
127,226,82,8,128,125,65,104,19,101,185,243,118,217,144,0,118,251,160,93,148,77,93,79,13,4,5,208,119,111,231,250,174,135,6,130,2,216,53,159,183,141,178,177,227,94,217,176,0,10,247,118,174,240,186,85,246,
161,1,180,175,107,216,111,120,61,112,59,60,23,24,64,235,157,33,66,148,85,78,7,6,66,3,40,221,79,126,148,78,71,70,66,3,104,241,65,82,148,141,93,78,206,5,7,208,119,63,255,86,184,28,156,11,14,192,56,159,39,
70,89,237,112,110,46,60,0,211,186,6,49,202,190,252,137,19,129,0,76,62,72,61,7,94,114,127,49,10,0,219,243,121,250,217,183,41,251,208,92,12,0,250,238,167,31,251,236,99,147,49,0,216,244,65,206,33,240,154,
251,139,81,0,40,220,207,128,223,45,141,76,164,2,88,159,207,243,94,30,149,51,127,49,14,0,107,62,200,124,40,212,148,247,139,145,0,40,29,50,240,222,7,7,130,1,172,204,231,217,231,191,255,202,36,3,40,66,29,
255,142,5,192,253,46,240,135,126,16,74,52,0,150,235,26,147,100,1,148,14,25,168,2,192,220,7,253,62,24,241,55,67,61,143,22,64,159,57,155,127,244,243,118,29,207,127,100,177,115,47,34,0,183,243,121,78,6,62,
49,140,242,163,72,0,189,138,149,129,122,0,100,57,235,73,80,68,0,195,136,1,100,163,44,109,5,240,74,147,2,50,117,10,248,247,149,69,13,21,43,192,170,134,138,21,16,33,0,40,0,10,128,2,160,0,40,0,10,128,2,160,
0,40,0,10,128,2,160,0,40,0,10,128,2,160,0,40,0,10,128,2,160,0,40,0,10,128,2,160,128,110,0,252,121,108,172,159,146,81,64,203,205,209,71,201,40,32,66,0,80,0,20,0,0,233,0,136,118,163,164,87,0,59,54,74,90,
148,2,0,110,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,144,16,128,87,238,245,90,52,0,239,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,209,0,248,246,216,162,142,86,63,238,63,199,132,122,30,61,0,171,122,185,6,192,195,31,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,136,17,192,99,247,250,78,50,0,255,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,75,245,30,119,90,131,12,133,66,161,80,40,20,10,
133,66,161,80,40,20,10,133,66,161,80,40,20,10,229,88,207,12,53,236,246,146,47,12,151,124,218,237,37,191,49,92,242,199,249,143,8,111,253,240,85,71,134,75,126,232,246,146,79,12,151,252,8,0,173,0,134,80,
0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,80,0,20,0,5,64,1,0,192,174,151,49,1,248,31,199,213,223,14,69,179,17,
113,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PointInfoControl::ungroupobjectsicon_png = (const char*) resource_PointInfoControl_ungroupobjectsicon_png;
const int PointInfoControl::ungroupobjectsicon_pngSize = 2514;

// JUCER_RESOURCE: trashbinicon_png, 3030, "../Resources/trash-bin-icon.png"
static const unsigned char resource_PointInfoControl_trashbinicon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,186,0,0,2,1,4,3,0,0,0,205,60,103,157,0,0,0,1,115,82,71,66,0,174,206,28,233,
0,0,0,4,103,65,77,65,0,0,177,143,11,252,97,5,0,0,0,24,80,76,84,69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,86,55,28,42,0,0,0,7,116,82,78,83,0,29,68,106,145,188,232,9,180,75,202,0,0,11,73,73,68,
65,84,120,218,237,157,207,147,84,69,18,199,171,94,15,177,215,50,22,246,220,33,200,121,130,21,247,58,160,198,94,119,68,131,187,232,122,223,141,133,191,1,254,3,69,98,207,43,171,112,214,16,57,75,4,193,89,
116,236,179,160,212,221,153,46,99,24,102,166,39,152,238,87,63,242,87,37,89,28,28,177,124,253,62,239,251,205,204,202,156,233,158,193,129,173,215,160,22,220,45,13,80,215,57,255,201,63,161,214,181,247,167,
178,232,78,126,124,49,192,61,242,211,151,47,74,162,59,121,5,144,109,119,157,223,148,67,55,185,226,160,215,217,13,49,116,239,59,248,245,86,16,66,247,151,41,2,157,255,80,6,157,223,116,24,235,196,84,4,221,
169,128,66,231,54,69,208,93,192,129,115,39,36,208,77,166,72,116,126,67,0,221,73,135,181,206,9,160,91,71,163,91,227,167,243,120,116,126,202,78,55,113,120,107,67,53,221,107,236,116,83,68,186,137,106,58,
207,78,23,16,233,6,213,218,53,63,186,129,219,60,162,233,156,209,177,45,163,51,58,163,51,58,163,51,58,163,51,58,163,51,58,163,227,167,219,185,125,251,155,186,63,247,59,160,155,111,109,61,172,251,243,200,
156,105,116,70,103,116,70,103,116,70,103,116,70,103,116,70,103,116,70,103,116,70,103,116,70,103,116,70,103,116,227,116,175,255,181,118,189,124,197,234,75,189,252,147,158,167,107,47,117,110,129,110,184,
252,193,59,149,235,237,151,238,104,82,123,169,119,94,254,113,204,51,181,151,122,247,63,235,251,116,195,199,83,125,158,252,251,250,11,186,119,131,198,152,219,165,26,156,59,177,174,51,163,108,62,167,123,
79,105,194,60,21,220,128,247,110,9,238,229,55,220,128,248,110,9,238,117,218,13,110,67,45,221,196,13,126,170,150,206,79,7,205,71,177,117,213,116,186,181,243,170,123,4,63,4,205,120,214,189,26,157,209,25,
157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,209,25,157,
209,25,93,1,93,84,77,151,20,195,37,213,116,207,134,185,98,186,153,114,186,244,72,45,220,124,54,56,189,116,59,110,112,79,212,210,61,116,131,155,107,21,111,126,127,247,172,242,173,82,186,199,207,79,98,219,
119,117,70,221,221,189,115,230,15,26,189,57,255,252,197,41,58,125,125,79,29,220,175,55,227,126,143,144,30,220,184,121,123,111,245,237,210,237,23,20,95,124,250,121,92,232,128,210,111,91,123,107,214,247,
177,249,5,197,207,49,163,191,187,222,195,90,212,35,141,116,175,125,183,12,69,116,93,188,107,52,20,76,30,250,166,139,218,230,42,71,126,17,88,84,167,157,43,112,166,105,103,116,230,76,211,174,108,141,86,
132,216,115,220,185,18,58,117,206,204,127,46,221,107,231,59,211,49,169,115,102,40,162,235,205,153,161,90,187,222,156,169,175,154,235,214,206,156,249,170,100,149,158,199,70,133,116,161,47,237,156,106,186,
168,173,34,172,60,68,247,175,157,234,106,174,91,59,115,166,57,83,135,118,81,115,220,117,214,156,151,210,245,117,140,246,13,206,236,171,5,74,78,155,51,67,33,93,95,206,12,13,218,245,222,156,247,94,205,117,
107,103,206,124,117,178,74,191,99,163,98,186,208,147,118,78,53,93,212,86,17,86,31,162,187,215,78,117,53,215,173,157,57,83,74,179,41,81,187,51,151,175,93,189,148,37,242,153,79,174,102,239,188,118,245,34,
138,118,133,99,163,179,151,166,206,159,249,40,99,231,91,151,66,230,206,191,237,238,60,255,111,4,237,10,155,243,83,155,207,255,49,185,50,190,243,66,238,206,179,123,191,249,108,237,67,4,186,162,56,25,246,
239,96,109,3,108,231,100,115,255,121,172,55,198,241,136,118,163,116,111,28,124,245,102,246,206,243,35,59,15,175,244,118,219,33,186,53,103,250,67,29,38,171,31,180,255,199,225,107,174,22,111,225,63,175,
141,253,250,186,80,76,87,162,221,218,194,229,47,172,222,185,240,245,185,236,157,27,172,116,175,47,185,171,213,59,39,43,119,46,62,165,63,179,86,243,69,55,14,83,248,157,107,156,218,29,253,181,142,211,236,
157,171,66,116,226,114,119,86,101,149,2,186,33,251,78,142,238,156,130,236,28,63,109,180,213,187,33,123,115,237,206,0,237,204,130,177,145,207,190,103,95,242,208,234,154,148,10,186,146,22,200,131,236,12,
37,215,44,119,38,202,200,15,69,173,177,161,209,104,69,192,209,142,168,189,3,117,38,204,10,128,70,106,171,230,220,115,107,74,237,48,156,25,25,233,18,186,51,229,76,141,76,59,126,237,162,230,184,43,24,27,
113,107,87,67,87,173,71,96,213,174,194,153,244,231,143,218,71,148,176,171,121,224,124,18,169,7,237,124,137,51,113,233,184,181,163,172,230,218,180,147,254,3,129,160,113,71,94,17,106,180,115,156,218,85,
63,206,92,186,234,19,8,188,118,9,222,153,5,199,104,211,14,81,187,145,161,209,120,69,16,173,157,171,160,171,30,61,80,107,231,107,226,14,44,97,99,107,231,73,181,243,212,113,71,74,199,170,29,186,51,61,231,
147,232,66,59,200,81,244,241,218,229,143,141,120,79,209,85,218,209,215,42,136,138,16,107,180,243,13,175,142,125,222,241,205,206,244,130,181,27,221,57,80,189,58,134,118,161,138,78,142,118,188,116,216,218,
53,230,100,225,57,51,170,214,206,33,104,39,39,238,48,180,227,204,20,248,89,165,186,202,132,30,156,201,121,140,14,164,206,164,214,174,246,73,196,46,180,171,237,65,48,232,192,43,2,108,138,30,16,131,6,91,
59,143,173,29,120,251,10,91,128,96,227,174,11,237,80,194,30,57,238,98,135,218,97,56,147,113,108,228,33,199,42,194,115,38,14,93,31,159,100,225,1,156,233,11,156,25,72,179,202,184,202,178,223,111,158,218,
14,209,192,218,209,86,4,124,58,232,147,70,73,206,172,173,8,124,213,220,169,214,206,17,104,199,23,119,129,64,59,25,39,124,172,156,153,216,170,57,236,105,103,156,174,164,57,239,67,59,186,184,135,138,187,
136,162,157,148,21,59,116,102,126,107,142,83,205,25,179,138,119,196,206,36,213,174,195,156,73,161,157,236,92,65,231,76,206,138,80,157,85,106,223,164,22,122,208,174,139,106,94,79,87,219,2,177,105,231,80,
232,128,181,131,29,26,65,231,76,202,83,91,168,166,227,26,104,150,188,199,137,130,46,178,105,87,95,17,232,14,75,178,181,19,189,68,87,132,132,148,85,132,56,19,137,46,117,160,93,189,51,243,143,209,61,106,
39,163,154,231,199,93,84,167,157,35,160,195,28,61,164,214,151,18,214,155,251,202,173,52,206,20,86,249,129,157,9,155,149,18,146,118,181,241,236,153,76,28,181,105,215,210,35,100,143,141,216,62,48,141,196,
153,50,42,66,172,215,142,112,176,2,92,90,155,233,48,181,139,173,34,15,168,238,193,212,46,52,208,233,214,78,126,220,133,134,184,19,145,51,35,187,51,217,180,131,170,230,108,163,7,52,237,152,22,81,206,76,
149,206,148,53,140,206,161,163,187,101,47,205,153,73,130,137,35,145,118,116,89,133,198,153,17,207,153,173,45,2,120,206,36,211,174,137,78,188,118,44,57,147,39,189,226,57,19,79,187,228,248,157,137,167,93,
66,115,166,147,30,119,109,206,172,188,105,50,237,224,232,36,126,231,220,55,57,51,155,14,114,24,237,129,69,30,4,11,146,90,15,209,194,180,163,234,128,36,196,29,162,118,44,213,28,184,53,7,215,206,83,105,
71,228,204,212,163,118,2,78,249,136,113,151,132,107,215,230,204,202,99,52,89,122,37,114,38,158,118,217,35,177,216,163,118,81,156,51,69,181,230,224,206,244,68,89,165,145,78,128,118,84,206,148,87,17,32,
157,73,53,88,201,207,153,84,206,76,78,238,146,165,157,163,210,46,59,166,1,181,131,30,137,101,210,121,113,218,181,58,147,161,57,175,212,46,53,198,93,193,35,38,210,46,52,210,85,142,141,164,52,135,226,227,
206,33,106,71,24,252,199,63,196,228,36,56,179,203,106,206,254,109,146,68,229,204,130,172,18,104,178,74,43,93,226,118,102,194,116,102,246,49,154,165,34,144,229,76,172,154,149,29,119,81,157,118,100,206,
228,208,46,175,69,144,85,205,161,7,15,146,181,35,235,205,137,86,160,115,102,229,96,133,70,59,46,58,26,237,96,157,41,109,212,222,158,51,99,251,235,162,229,98,88,58,207,224,76,220,156,89,215,224,209,107,
151,80,43,2,92,206,12,85,59,19,161,118,142,68,59,58,103,114,196,93,32,115,38,86,106,136,34,156,217,127,206,228,188,69,124,103,146,100,21,79,233,204,84,231,76,73,135,182,76,58,146,123,38,213,142,161,57,
175,138,187,72,169,29,73,69,32,116,102,196,113,102,123,139,32,184,154,39,92,58,193,218,1,247,230,166,93,95,113,199,156,51,19,161,51,245,229,76,222,177,81,34,164,163,56,70,211,230,204,92,58,250,247,57,
101,14,141,68,125,31,33,127,20,29,0,232,228,106,71,233,204,62,181,163,172,76,40,131,7,4,237,40,42,2,180,51,41,180,11,164,218,57,86,237,176,233,42,199,70,64,218,69,100,103,214,29,163,161,180,139,132,206,
148,27,119,250,180,203,221,7,79,71,125,220,142,180,206,12,232,206,244,61,106,71,155,51,169,181,115,170,181,203,206,22,164,116,9,65,17,252,14,8,185,24,49,107,7,82,114,48,180,131,167,243,130,180,131,113,
102,221,232,129,86,187,132,93,17,112,180,3,56,68,203,213,14,36,17,15,240,214,135,209,46,161,107,231,72,181,147,234,76,24,237,56,171,57,245,74,170,179,10,200,96,96,200,253,63,241,135,98,190,74,59,7,66,
183,234,146,125,102,21,206,236,160,77,187,186,92,27,209,233,136,181,243,48,116,124,209,4,211,0,193,104,151,136,227,142,182,154,19,127,3,207,115,198,93,237,3,232,195,153,169,146,53,194,231,225,1,34,232,
51,232,18,32,29,84,206,140,53,182,59,246,214,231,165,116,51,244,122,151,123,140,62,122,235,207,198,183,28,123,235,59,185,116,30,200,153,185,77,194,163,165,255,114,160,214,108,116,203,145,7,48,207,116,
102,34,232,239,198,233,142,252,109,58,86,152,249,108,169,144,149,135,104,32,237,126,93,248,122,251,248,45,91,227,91,238,47,217,206,221,155,111,47,60,134,159,150,108,89,248,250,225,241,91,158,46,33,229,
214,206,221,57,124,185,187,75,242,234,225,223,207,151,220,250,206,161,123,127,143,146,232,158,28,236,124,188,108,203,99,55,186,229,219,131,175,190,18,53,53,74,183,246,159,255,157,165,101,227,191,251,30,
189,187,212,224,251,162,254,48,115,146,180,115,219,123,79,123,254,255,229,91,126,249,110,111,203,173,229,91,190,223,243,230,211,59,142,66,187,130,177,209,227,91,51,231,126,188,185,234,161,127,255,101,
220,221,178,34,164,210,215,247,156,75,15,110,1,141,85,10,232,198,214,211,255,221,184,254,229,234,163,219,143,159,126,54,178,37,61,184,254,217,141,123,142,134,174,172,67,201,120,22,207,32,182,4,4,103,10,
252,208,232,209,115,62,156,51,5,182,230,160,206,236,141,78,190,51,73,170,185,105,103,116,124,206,84,167,29,199,91,201,233,234,157,200,143,119,245,96,206,148,78,151,212,85,132,0,70,215,249,7,15,155,118,
194,180,51,103,154,51,181,107,151,84,199,93,82,237,76,137,199,232,128,226,76,137,45,80,212,150,51,61,28,157,64,237,116,211,233,174,230,230,76,115,166,204,122,7,168,157,244,177,17,32,157,151,167,157,211,
76,151,0,227,174,187,67,116,135,218,233,174,230,186,181,51,103,154,51,245,107,151,84,199,93,82,237,76,121,199,232,128,228,76,121,45,80,132,212,174,183,6,168,63,237,116,211,233,174,230,175,144,118,186,
227,14,212,153,234,178,202,145,11,252,233,26,255,250,23,168,118,162,135,98,81,51,93,2,141,59,125,116,177,103,124,211,206,232,204,153,166,29,177,118,73,181,118,186,157,153,204,153,66,143,153,125,59,179,
157,110,46,152,110,102,116,171,215,35,177,112,243,168,153,110,7,224,36,246,84,44,221,22,0,221,206,76,40,92,186,15,113,138,254,74,40,221,147,8,65,247,187,204,200,59,248,84,140,198,106,254,141,68,111,206,
111,3,245,119,243,47,238,61,19,182,126,91,253,121,25,251,235,15,46,179,139,172,100,45,159,179,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PointInfoControl::trashbinicon_png = (const char*) resource_PointInfoControl_trashbinicon_png;
const int PointInfoControl::trashbinicon_pngSize = 3030;


//[EndFile] You can add extra defines here...
//[/EndFile]
