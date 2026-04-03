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



#include "RadarOptions.h"
#include "TrackColors.h"
#include "Constants.h"
#include "ColorDefinition.h"
#include "SvgHelper.h"

#include "PointInfoControl.h"


#if JUCE_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif

PointInfoControl::PointInfoControl (AmbiDataSet* _pEditablePoints, PointSelection* _pPointSelection, RadarOptions* _pRadarOptions)
    : pEditablePoints(_pEditablePoints), pPointSelection(_pPointSelection), pRadarOptions(_pRadarOptions)
{
    textName.reset (new juce::TextEditor ("textName"));
    addAndMakeVisible (textName.get());
    textName->setMultiLine (false);
    textName->setReturnKeyStartsNewLine (false);
    textName->setReadOnly (true);
    textName->setScrollbarsShown (true);
    textName->setCaretVisible (false);
    textName->setPopupMenuEnabled (true);
    textName->setText (juce::String());
    textName->setTextToShowWhenEmpty("Name", Colours::grey);

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

    textCH.reset (new juce::TextEditor ("labelCH"));
    addAndMakeVisible (textCH.get());
    textCH->setReadOnly(true);
    textCH->setMultiLine (false);
    textCH->setReturnKeyStartsNewLine (false);
    textCH->setScrollbarsShown (true);
    textCH->setCaretVisible (true);
    textCH->setPopupMenuEnabled (true);
    textCH->setText (juce::String());

    btnUngroup.reset (new DrawableButton ("btnUngroup", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnUngroup.get());
    btnUngroup->setButtonText (TRANS("Ungroup"));
    btnUngroup->addListener (this);
    btnUngroup->setTooltip("Ungroup sources (remove group point)");
    SvgHelper::loadSVGIcon(btnUngroup.get(), BinaryData::ungroup_icon_svg, BinaryData::ungroup_icon_svgSize, "");
    
    btnGroup.reset (new DrawableButton ("btnGroup", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnGroup.get());
    btnGroup->setButtonText (TRANS("Group"));
    btnGroup->addListener (this);
    btnGroup->setTooltip("Group selected sources");
    SvgHelper::loadSVGIcon(btnGroup.get(), BinaryData::group_icon_svg, BinaryData::group_icon_svgSize, "");
    
    btnDelete.reset (new DrawableButton ("btnDelete", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnDelete.get());
    btnDelete->setButtonText (TRANS("Delete"));
    btnDelete->addListener (this);
    btnDelete->setTooltip("Delete");
    SvgHelper::loadSVGIcon(btnDelete.get(), BinaryData::delete_icon_svg, BinaryData::delete_icon_svgSize, "");
    
    btnSphere.reset (new ColorBorderDrawableButton ("btnSphere", pRadarOptions->zoomSettings->fullSphere));
    addAndMakeVisible (btnSphere.get());
    btnSphere->setButtonText (TRANS("Sphere"));
    btnSphere->addListener (this);
    btnSphere->setTooltip("Toggle full/half sphere");
    SvgHelper::loadSVGIcon(btnSphere.get(), BinaryData::sphere_icon_svg, BinaryData::sphere_icon_svgSize, "");

    btnPolarCartesian.reset (new ColorBorderDrawableButton ("btnPolarCartesian", pRadarOptions->zoomSettings->showAed));
    addAndMakeVisible (btnPolarCartesian.get());
    btnPolarCartesian->setButtonText (TRANS("PolarCartesian"));
    btnPolarCartesian->addListener (this);
    btnPolarCartesian->setTooltip("Toggle XYZ/AED");
    SvgHelper::loadSVGIcon(btnPolarCartesian.get(), BinaryData::angle_icon_svg, BinaryData::angle_icon_svgSize, "");

    textCH->setJustification(Justification::centredLeft);
    textName->setJustification(Justification::centredLeft);
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

    setSize (390, 100);

	updateSelectedPoint();
	pPointSelection->addChangeListener(this);
}

PointInfoControl::~PointInfoControl()
{
    pPointSelection->removeChangeListener(this);

    textName = nullptr;
    textX = nullptr;
    textY = nullptr;
    textZ = nullptr;
    textA = nullptr;
    textE = nullptr;
    textD = nullptr;
    textCH = nullptr;
    btnUngroup = nullptr;
    btnGroup = nullptr;
    btnDelete = nullptr;
    btnSphere = nullptr;
    btnPolarCartesian = nullptr;
}

void PointInfoControl::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void PointInfoControl::resized()
{
    btnDelete->setBounds (2, 2, 28, 28);
    btnGroup->setBounds (34, 2, 28, 28);
    btnUngroup->setBounds (66, 2, 28, 28);
    
    int y = 4;
    int remainingWidth = jmin(400, getWidth() - 162);
    
    if(getHeight() <= controlSize::narrow)
    {
        btnSphere->setBounds(98, 2, 28, 28);
        btnPolarCartesian->setBounds(130, 2, 28, 28);
    }
    else
    {
        btnSphere->setBounds(getWidth() - 66, 2, 28, 28);
        btnPolarCartesian->setBounds(getWidth() - 34, 2, 28, 28);
        y = 34;
        remainingWidth = getWidth();
    }
    
    textCH->setBounds(getWidth()-remainingWidth, y, 38, 24);
    remainingWidth -= 40;
    textName->setBounds (getWidth()-remainingWidth, y, remainingWidth / 4 - 2, 24);
    remainingWidth -= remainingWidth / 4;
    textX->setBounds (getWidth()-remainingWidth, y, remainingWidth / 3 - 2, 24);
    remainingWidth -= remainingWidth / 3;
    textY->setBounds (getWidth()-remainingWidth, y, remainingWidth / 2 - 2, 24);
    remainingWidth -= remainingWidth / 2;
    textZ->setBounds (getWidth()-remainingWidth, y, remainingWidth - 4, 24);
    
    textA->setBounds (textX->getX(), textX->getY(), textX->getWidth(), textX->getHeight());
    textE->setBounds (textY->getX(), textY->getY(), textY->getWidth(), textY->getHeight());
    textD->setBounds (textZ->getX(), textZ->getY(), textZ->getWidth(), textZ->getHeight());
}

void PointInfoControl::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnUngroup.get())
    {
        if (pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            int indexToRemove = pPointSelection->getMainSelectedPointIndex();
            pPointSelection->unselectPoint();
            pEditablePoints->removeGroup(indexToRemove);
        }
    }
    else if (buttonThatWasClicked == btnGroup.get())
    {
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
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
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
    }
    else if (buttonThatWasClicked == btnPolarCartesian.get())
    {
        if(pRadarOptions != nullptr && pRadarOptions->zoomSettings != nullptr)
        {
            pRadarOptions->zoomSettings->setShowAed(!pRadarOptions->zoomSettings->showAed);
            controlVisibility();
        }
    }
    else if(buttonThatWasClicked == btnSphere.get())
    {
        if(pRadarOptions != nullptr && pRadarOptions->zoomSettings != nullptr)
        {
            pRadarOptions->zoomSettings->setFullSphere(!pRadarOptions->zoomSettings->fullSphere);
        }
    }
}



void PointInfoControl::updateSelectedPoint(String exceptField)
{
	disableListeners();

	int selection = pPointSelection->getMainSelectedPointIndex();
	if (pPointSelection->getSelectionMode() != PointSelection::None && selection >= 0 && selection < pEditablePoints->size())
	{
		AmbiPoint* point = pPointSelection->getSelectionMode() == PointSelection::Group ? pEditablePoints->getGroup(selection) : pEditablePoints->get(selection);

		setFieldsEnabled(true);
		textName->setText(point->getName());
        textCH->setText("CH " + String(selection + 1));
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
		textName->setText("");
        textCH->setText("");
		textX->setText("");
		textY->setText("");
		textZ->setText("");
		textA->setText("");
		textE->setText("");
		textD->setText("");
	}

    bool enGroup = pRadarOptions->allowGroup && pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getSelectedIndices().size() > 1
    && ( pRadarOptions->audioParams == nullptr || pEditablePoints->activeGroupCount() < pRadarOptions->audioParams->groupParams.size());
    bool enUngroup = pRadarOptions->allowGroup && pPointSelection->getSelectionMode() == PointSelection::Group && pPointSelection->getSelectedIndices().size() == 1;
    bool enDelete = pRadarOptions->allowDelete && pPointSelection->getSelectedIndices().size() > 0;
	btnGroup->setEnabled(enGroup);
	btnUngroup->setEnabled(enUngroup);
    btnDelete->setEnabled(enDelete);
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
    textCH->setEnabled(enable);
    textName->setEnabled(enable);
    textX->setEnabled(enable);
    textY->setEnabled(enable);
    textZ->setEnabled(enable);
    textA->setEnabled(enable);
    textE->setEnabled(enable);
    textD->setEnabled(enable);
    controlVisibility();
}

void PointInfoControl::controlVisibility() const
{
    bool aed = pRadarOptions->zoomSettings->showAed;
    textX->setVisible(!aed);
    textY->setVisible(!aed);
    textZ->setVisible(!aed);
    textA->setVisible(aed);
    textE->setVisible(aed);
    textD->setVisible(aed);
}

int PointInfoControl::getRequiredHeight(int width) const
{
    return width > 500 ? controlSize::narrow : controlSize::wide;
}
