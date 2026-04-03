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
    textName.reset (new CoorLabel ("textName", "Name: ", ""));
    setupLabel(textName.get(), false);

    textX.reset (new CoorLabel("textX", "X: ", ""));
    setupLabel(textX.get());

    textY.reset (new CoorLabel("textY", "Y: ", ""));
    setupLabel(textY.get());

    textZ.reset (new CoorLabel("textZ", "Z: ", ""));
    setupLabel(textZ.get());

    textA.reset (new CoorLabel("textA", "A: ", juce::String::fromUTF8 (u8"\u00B0")));
    setupLabel(textA.get());

    textE.reset (new CoorLabel("textE", "E: ", juce::String::fromUTF8 (u8"\u00B0")));
    setupLabel(textE.get());

    textD.reset (new CoorLabel("textD", "D: ", ""));
    setupLabel(textD.get());

    textCH.reset (new CoorLabel ("labelCH", "CH", ""));
    addAndMakeVisible (textCH.get());
    textCH->setEditable(false);
    textCH->setText (juce::String(), dontSendNotification);

    btnUngroup.reset (new DrawableButton ("btnUngroup", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnUngroup.get());
    btnUngroup->setButtonText (TRANS("Ungroup"));
    btnUngroup->addListener (this);
    SvgHelper::loadSVGIcon(btnUngroup.get(), BinaryData::ungroup_icon_svg, BinaryData::ungroup_icon_svgSize, "");
    
    btnGroup.reset (new DrawableButton ("btnGroup", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnGroup.get());
    btnGroup->setButtonText (TRANS("Group"));
    btnGroup->addListener (this);
    SvgHelper::loadSVGIcon(btnGroup.get(), BinaryData::group_icon_svg, BinaryData::group_icon_svgSize, "");
    
    btnDelete.reset (new DrawableButton ("btnDelete", juce::DrawableButton::ImageOnButtonBackground));
    addAndMakeVisible (btnDelete.get());
    btnDelete->setButtonText (TRANS("Delete"));
    btnDelete->addListener (this);
    SvgHelper::loadSVGIcon(btnDelete.get(), BinaryData::delete_icon_svg, BinaryData::delete_icon_svgSize, "");
    
    btnSphere.reset (new ColorBorderDrawableButton ("btnSphere", pRadarOptions->zoomSettings->fullSphere));
    addAndMakeVisible (btnSphere.get());
    btnSphere->setButtonText (TRANS("Sphere"));
    btnSphere->addListener (this);
    SvgHelper::loadSVGIcon(btnSphere.get(), BinaryData::sphere_icon_svg, BinaryData::sphere_icon_svgSize, "Toggle full/half sphere");

    btnPolarCartesian.reset (new ColorBorderDrawableButton ("btnPolarCartesian", pRadarOptions->zoomSettings->showAed));
    addAndMakeVisible (btnPolarCartesian.get());
    btnPolarCartesian->setButtonText (TRANS("PolarCartesian"));
    btnPolarCartesian->addListener (this);
    SvgHelper::loadSVGIcon(btnPolarCartesian.get(), BinaryData::angle_icon_svg, BinaryData::angle_icon_svgSize, "Toggle XYZ/AED");

    textCH->setJustificationType(Justification::centredLeft);
    textName->setJustificationType(Justification::centredLeft);
    textX->setJustificationType(Justification::centredRight);
    textY->setJustificationType(Justification::centredRight);
    textZ->setJustificationType(Justification::centredRight);
    textA->setJustificationType(Justification::centredRight);
    textE->setJustificationType(Justification::centredRight);
    textD->setJustificationType(Justification::centredRight);

    setSize (390, 100);

	updateSelectedPoint();
	pPointSelection->addChangeListener(this);
}

void PointInfoControl::setupLabel(juce::Label* l, bool editable)
{
    addAndMakeVisible(l);
    l->setEditable(editable);
    l->setText("", dontSendNotification);
    
    if(editable)
    {
        l->onEditorShow = [this]
        {
            shiftWasDownWhenEditingStarted =
            juce::ModifierKeys::getCurrentModifiersRealtime().isShiftDown();
        };
    }
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
    int multiSelection = pPointSelection->getSelectedIndices().size() > 1;
	if (pPointSelection->getSelectionMode() != PointSelection::None && selection >= 0 && selection < pEditablePoints->size())
	{
        String commonText = multiSelection ? " (Click to change; Shift+Click to change all selected points)" : " (Click to change)";
        
		AmbiPoint* point = pPointSelection->getSelectionMode() == PointSelection::Group ? pEditablePoints->getGroup(selection) : pEditablePoints->get(selection);

		setFieldsEnabled(true);
		textName->setText(point->getName(), dontSendNotification);
        textName->setTooltip("Channel Name " + commonText);
        textCH->setText(String(selection + 1), dontSendNotification);
        textCH->setTooltip("Channel index (1-based)");
        if (exceptField != textX->getName()) textX->setText(String(point->getRawPoint()->getX(), 3), dontSendNotification);
        textX->setTooltip("X-Coordinate" + commonText);
		if (exceptField != textY->getName()) textY->setText(String(point->getRawPoint()->getY(), 3), dontSendNotification);
        textY->setTooltip("Y-Coordinate" + commonText);
		if (exceptField != textZ->getName()) textZ->setText(String(point->getRawPoint()->getZ(), 3), dontSendNotification);
        textZ->setTooltip("Z-Coordinate" + commonText);
		if (exceptField != textA->getName()) textA->setText(String(Constants::RadToGrad(point->getRawPoint()->getAzimuth()), 2), dontSendNotification);
        textA->setTooltip("Azimuth" + commonText);
		if (exceptField != textE->getName()) textE->setText(String(Constants::RadToGrad(point->getRawPoint()->getElevation()), 2), dontSendNotification);
        textE->setTooltip("Elevation" + commonText);
		if (exceptField != textD->getName()) textD->setText(String(point->getRawPoint()->getDistance(), 3), dontSendNotification);
        textD->setTooltip("Distance" + commonText);
	}
	else
	{
		setFieldsEnabled(false);
		textName->setText("", dontSendNotification);
        textName->setTooltip("");
        textCH->setText("", dontSendNotification);
        textCH->setTooltip("");
		textX->setText("", dontSendNotification);
        textX->setTooltip("");
		textY->setText("", dontSendNotification);
        textY->setTooltip("");
		textZ->setText("", dontSendNotification);
        textZ->setTooltip("");
		textA->setText("", dontSendNotification);
        textA->setTooltip("");
		textE->setText("", dontSendNotification);
        textE->setTooltip("");
		textD->setText("", dontSendNotification);
        textD->setTooltip("");
	}

    bool enGroup = pRadarOptions->allowGroup && pPointSelection->getSelectionMode() == PointSelection::Point && pPointSelection->getSelectedIndices().size() > 1
    && ( pRadarOptions->audioParams == nullptr || pEditablePoints->activeGroupCount() < pRadarOptions->audioParams->groupParams.size());
    bool enUngroup = pRadarOptions->allowGroup && pPointSelection->getSelectionMode() == PointSelection::Group && pPointSelection->getSelectedIndices().size() == 1;
    bool enDelete = pRadarOptions->allowDelete && pPointSelection->getSelectedIndices().size() > 0;
	btnGroup->setEnabled(enGroup);
    btnGroup->setTooltip(enGroup ? "Group selected sources" : "");
    btnUngroup->setEnabled(enUngroup);
    btnUngroup->setTooltip(enUngroup ? "Ungroup sources (remove group point)" : "");
    btnDelete->setEnabled(enDelete);
    btnDelete->setTooltip(enDelete ? "Delete selected point(s)/group(s)" : "");
    bool makeReadOnly = pPointSelection->getSelectionMode() == PointSelection::None;
	textX->setEditable(!makeReadOnly);
    textY->setEditable(!makeReadOnly);
    textZ->setEditable(!makeReadOnly);
    textA->setEditable(!makeReadOnly);
    textE->setEditable(!makeReadOnly);
    textD->setEditable(!makeReadOnly);
    if(pRadarOptions->checkNameFieldEditable)
    {
        makeReadOnly = pRadarOptions->dawParameter->updateTrackPropertiesWorking;
    }
    textName->setEditable(!makeReadOnly);
	enableListeners();
}

void PointInfoControl::changeListenerCallback(ChangeBroadcaster* source)
{
	if(source == pPointSelection)
		updateSelectedPoint();
}

void PointInfoControl::labelTextChanged(Label *source)
{
	Array<int> selectedIndices = shiftWasDownWhenEditingStarted ? pPointSelection->getSelectedIndices() : Array<int>( pPointSelection->getMainSelectedPointIndex());
    
	if(pPointSelection->getSelectionMode() == PointSelection::Group)
	{
		if (source->getName() == textName->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setGroupName(selection, textName->getText());
		}
        
        if (source->getName() == textX->getName())
        {
            for(int selection : selectedIndices)
                pEditablePoints->setGroupX(selection, textX->getText().getFloatValue());
        }
        if (source->getName() == textY->getName())
        {
            for(int selection : selectedIndices)
                pEditablePoints->setGroupY(selection, textY->getText().getFloatValue());
        }
        if (source->getName() == textZ->getName())
        {
            for(int selection : selectedIndices)
                pEditablePoints->setGroupZ(selection, textZ->getText().getFloatValue());
        }

        if (source->getName() == textA->getName())
        {
            for(int selection : selectedIndices)
                pEditablePoints->setGroupAed(selection, Constants::GradToRad(textA->getText().getFloatValue()), pEditablePoints->getGroup(selection)->getRawPoint()->getElevation(), pEditablePoints->getGroup(selection)->getRawPoint()->getDistance(), pEditablePoints->getGroupModeFlag());
        }
        if (source->getName() == textE->getName())
        {
            for(int selection : selectedIndices)
                pEditablePoints->setGroupAed(selection, pEditablePoints->getGroup(selection)->getRawPoint()->getAzimuth(), Constants::GradToRad(textE->getText().getFloatValue()), pEditablePoints->getGroup(selection)->getRawPoint()->getDistance(), pEditablePoints->getGroupModeFlag());
        }
        if (source->getName() == textD->getName())
        {
            for(int selection : selectedIndices)
                pEditablePoints->setGroupAed(selection, pEditablePoints->getGroup(selection)->getRawPoint()->getAzimuth(), pEditablePoints->getGroup(selection)->getRawPoint()->getElevation(), textD->getText().getFloatValue(), pEditablePoints->getGroupModeFlag());
        }
	}
	else
	{
		if (source->getName() == textName->getName())
		{
            for(int selection : selectedIndices)
            {
                pEditablePoints->setChannelName(selection, textName->getText());
                if (pRadarOptions->setTrackColorAccordingToName)
                {
                    pEditablePoints->setChannelColor(selection, TrackColors::getColor(textName->getText().initialSectionContainingOnly("0123456789").getIntValue()));
                }
            }
		}

		if (source->getName() == textX->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setX(selection, textX->getText().getFloatValue());
		}
		if (source->getName() == textY->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setY(selection, textY->getText().getFloatValue());
		}
		if (source->getName() == textZ->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setZ(selection, textZ->getText().getFloatValue());
		}

		if (source->getName() == textA->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setAzimuth(selection, Constants::GradToRad(textA->getText().getFloatValue()));
		}
		if (source->getName() == textE->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setElevation(selection, Constants::GradToRad(textE->getText().getFloatValue()));
		}
		if (source->getName() == textD->getName())
		{
            for(int selection : selectedIndices)
                pEditablePoints->setDistance(selection, textD->getText().getFloatValue());
		}
	}

	updateSelectedPoint(source->getName());
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
