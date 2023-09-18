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



#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/ColorEditorCustomComponent.h"
#include "../../Common/GroupPointsSelectionComponent.h"
#include "../../Common/ScalingInfo.h"
#include "../../Common/ColorDefinition.h"

#define COLUMN_ID_GROUP_NB		101
#define COLUMN_ID_GROUP_NAME	102
#define	COLUMN_ID_GROUP_X		107
#define	COLUMN_ID_GROUP_Y		108
#define	COLUMN_ID_GROUP_Z		109
#define	COLUMN_ID_GROUP_A		110
#define	COLUMN_ID_GROUP_E		111
#define	COLUMN_ID_GROUP_D		112
#define COLUMN_ID_GROUP_STRETCH 120
#define COLUMN_ID_GROUP_ROTATION    121
#define COLUMN_ID_GROUP_COLOR	113
#define COLUMN_ID_GROUP_POINTS	114


class GroupTableListModel : public TableListBoxModel, public TableColumnCallback, public ChangeListener
{
public:
	GroupTableListModel(AmbiSourceSet* _pSources, PointSelection* _pPointSelection, Component* _pParentComponent, ScalingInfo* _pScaling): pSources(_pSources), pPointSelection(_pPointSelection), pParentComponent(_pParentComponent), pTableListBox(nullptr), pScalingInfo(_pScaling)
	{
	}

	~GroupTableListModel() override {}

	int getNumRows() override {
		return pSources->activeGroupCount();
	}

	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
	{
        if (rowIsSelected)
        {
            Colour baseColor = COLOR_DEFINITION_SELECTED_ROW;
            if (pPointSelection->getMainSelectedPointIndex() == rowNumber)
                g.fillAll(baseColor);
            else
                g.fillAll(baseColor.withAlpha(0.4f));
        }
        else if (rowNumber % 2)
        {
            const Colour alternateColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId)
                .interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), COLOR_DEFINITION_ALTERNATE_INTENSITY));

            g.fillAll(alternateColour);
        }
	}

	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override 
	{
        int grpIndex;
		AmbiGroup* pt = pSources->getActiveGroup(rowNumber, &grpIndex);
		if (pt == nullptr)
			return;

		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId));
		String text;
		switch (columnId)
		{
		case COLUMN_ID_GROUP_NB: text = String(grpIndex + 1); break;
		case COLUMN_ID_GROUP_NAME: text = pt->getName(); break;
        case COLUMN_ID_GROUP_POINTS: text = String(pt->groupPointCount()); break;
		default: text = "";
		}
		g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);

		if(columnId == COLUMN_ID_GROUP_POINTS)
		{
			g.setColour(pParentComponent->getLookAndFeel().findColour(HyperlinkButton::textColourId));
			g.drawText("Edit...", 20, 0, width - 22, height, Justification::centredRight, true);
		}

		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}

	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override
	{
		if (columnId == COLUMN_ID_GROUP_X
			|| columnId == COLUMN_ID_GROUP_Y
			|| columnId == COLUMN_ID_GROUP_Z
			|| columnId == COLUMN_ID_GROUP_A
			|| columnId == COLUMN_ID_GROUP_E
			|| columnId == COLUMN_ID_GROUP_D
            || columnId == COLUMN_ID_GROUP_STRETCH)
		{
			NumericColumnCustomComponent* numericBox = static_cast<NumericColumnCustomComponent*> (existingComponentToUpdate);
			if (numericBox == nullptr)
				numericBox = new NumericColumnCustomComponent(*this);

			numericBox->setRowAndColumn(rowNumber, columnId);
            numericBox->setJustificationType(Justification::right);
			return numericBox;
		}
		else if (columnId == COLUMN_ID_GROUP_NAME
            || columnId == COLUMN_ID_GROUP_ROTATION)
		{
			EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
			if (textLabel == nullptr)
				textLabel = new EditableTextCustomComponent(*this);

			textLabel->setRowAndColumn(rowNumber, columnId);
			return textLabel;
		}
		else if (columnId == COLUMN_ID_GROUP_COLOR)
		{
			ColorEditorCustomComponent* colorBox = static_cast<ColorEditorCustomComponent*> (existingComponentToUpdate);
			if (colorBox == nullptr)
				colorBox = new ColorEditorCustomComponent(*this, true);

			colorBox->setRowAndColumn(rowNumber, columnId);
			return colorBox;
		}

		return nullptr;
	}

	void selectedRowsChanged(int lastRowSelected) override
	{
        pPointSelection->unselectPoint();

        bool first = true;
        if (lastRowSelected >= 0 && lastRowSelected < pSources->size())
        {
            auto set = pTableListBox->getSelectedRows();
            for (auto r : set.getRanges())
            {
                for (int i = r.getStart(); i < r.getEnd(); i++)
                {
                    if (i != lastRowSelected)
                    {
                        pPointSelection->selectGroup(i, !first);
                        first = false;
                    }
                }
            }

            pPointSelection->selectGroup(lastRowSelected, !first);
        }
	}

	double getValue(int columnId, int rowNumber) override 
	{
		// group table handling
		AmbiGroup* pt = pSources->getActiveGroup(rowNumber);
		if (pt == nullptr)
			return 0.0;

		switch (columnId)
		{
		case COLUMN_ID_GROUP_X: return pt->getRawPoint()->getX();
		case COLUMN_ID_GROUP_Y: return pt->getRawPoint()->getY();
		case COLUMN_ID_GROUP_Z: return pt->getRawPoint()->getZ();
		case COLUMN_ID_GROUP_A: return Constants::RadToGrad(pt->getRawPoint()->getAzimuth());
		case COLUMN_ID_GROUP_E: return Constants::RadToGrad(pt->getRawPoint()->getElevation());
		case COLUMN_ID_GROUP_D: return pt->getRawPoint()->getDistance();
        case COLUMN_ID_GROUP_STRETCH: return pt->getStretch();
		case COLUMN_ID_GROUP_COLOR: return pt->getColor().getARGB();
		default: return 0.0;
		}
	}

    bool getEnabled(const int /*columnId*/, const int /*rowNumber*/) override
    {
        return true;
    }
    
	void setValue(int columnId, int rowNumber, double newValue) override
	{
		switch (columnId)
		{
		case COLUMN_ID_GROUP_X: pSources->getActiveGroup(rowNumber)->getRawPoint()->setX(newValue); break;
		case COLUMN_ID_GROUP_Y: pSources->getActiveGroup(rowNumber)->getRawPoint()->setY(newValue); break;
		case COLUMN_ID_GROUP_Z: pSources->getActiveGroup(rowNumber)->getRawPoint()->setZ(newValue); break;
		case COLUMN_ID_GROUP_A: pSources->getActiveGroup(rowNumber)->getRawPoint()->setAzimuth(Constants::GradToRad(newValue)); break;
		case COLUMN_ID_GROUP_E: pSources->getActiveGroup(rowNumber)->getRawPoint()->setElevation(Constants::GradToRad(newValue)); break;
		case COLUMN_ID_GROUP_D: pSources->getActiveGroup(rowNumber)->getRawPoint()->setDistance(newValue); break;
        case COLUMN_ID_GROUP_STRETCH:
            pSources->getActiveGroup(rowNumber)->setStretch(newValue);
            break;
		case COLUMN_ID_GROUP_COLOR:
            if(newValue < 0) // code for setting children
            {
                pSources->getActiveGroup(rowNumber)->setChildrenColor();
                pPointSelection->notifyChange();
            }
            else
            {
                pSources->getActiveGroup(rowNumber)->setColor(Colour(uint32(newValue)));
            }
            break;
		default: throw;
		}

		getTable()->updateContent();
		getTable()->repaint();
	}
	
	SliderRange getSliderRange(int columnId) override 
	{
		switch (columnId)
		{
		case COLUMN_ID_GROUP_X:
		case COLUMN_ID_GROUP_Y:
		case COLUMN_ID_GROUP_Z:
			return SliderRange(pScalingInfo->CartesianMin(), pScalingInfo->CartesianMax(), 0.001);

		case COLUMN_ID_GROUP_D:
			return SliderRange(Constants::DistanceMin, pScalingInfo->DistanceMax(), 0.001);

		case COLUMN_ID_GROUP_A:
			return SliderRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);

		case COLUMN_ID_GROUP_E:
			return SliderRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);
                
        case COLUMN_ID_GROUP_STRETCH:
            return SliderRange(Constants::StretchMin, Constants::StretchMax, 0.01);
                
		default: return SliderRange(0.0, 1.0, 0.001);
		}
	}

	TableListBox* getTable() override
	{
		return pTableListBox;
	}

	String getTableText(const int columnId, const int rowNumber) override
	{
		AmbiGroup* pt = pSources->getActiveGroup(rowNumber);
		if (pt == nullptr)
			return "";

		switch (columnId)
		{
		case COLUMN_ID_GROUP_NAME: return pt->getName();
        case COLUMN_ID_GROUP_ROTATION:
            {
                auto rot = pt->getRotation();
                String text;
                text << String(rot.vector.x, 2) << "; " << String(rot.vector.y, 2) << "; " << String(rot.vector.z, 2) << "; " << String(rot.scalar, 2);
                return text;
            }
                
		default: return "";
		}
	}

	void setTableText(const int columnId, const int rowNumber, const String& newText) override
	{
		switch (columnId)
		{
		case COLUMN_ID_GROUP_NAME: pSources->setGroupName(rowNumber, newText); break;
        case COLUMN_ID_GROUP_ROTATION:
            {
                StringArray tokens;
                tokens.addTokens (newText, ";", "");

                if(tokens.size() == 4)
                {
                    pSources->setGroupRotation(rowNumber, Quaternion<double>(
                                                tokens[0].getDoubleValue(),
                                                tokens[1].getDoubleValue(),
                                                tokens[2].getDoubleValue(),
                                                tokens[3].getDoubleValue()));
                }
                break;
            }
		default: throw;
		}
	}

	void initTable(TableListBox* tableListBox)
	{
		pTableListBox = tableListBox;
		tableListBox->setModel(this);
		tableListBox->getHeader().addColumn("CH", COLUMN_ID_GROUP_NB, 30);
		tableListBox->getHeader().addColumn("Name", COLUMN_ID_GROUP_NAME, 100);
		tableListBox->getHeader().addColumn("X", COLUMN_ID_GROUP_X, 50);
		tableListBox->getHeader().addColumn("Y", COLUMN_ID_GROUP_Y, 50);
		tableListBox->getHeader().addColumn("Z", COLUMN_ID_GROUP_Z, 50);
		tableListBox->getHeader().addColumn("A", COLUMN_ID_GROUP_A, 50);
		tableListBox->getHeader().addColumn("E", COLUMN_ID_GROUP_E, 50);
		tableListBox->getHeader().addColumn("D", COLUMN_ID_GROUP_D, 50);
        tableListBox->getHeader().addColumn("Stretch", COLUMN_ID_GROUP_STRETCH, 60);
        tableListBox->getHeader().addColumn("Rotation", COLUMN_ID_GROUP_ROTATION, 120);
		tableListBox->getHeader().addColumn("# Points", COLUMN_ID_GROUP_POINTS, 60);
		tableListBox->getHeader().addColumn("Color", COLUMN_ID_GROUP_COLOR, 60);
		tableListBox->getHeader().setStretchToFitActive(true);
		tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
        tableListBox->setMultipleSelectionEnabled(true);
	}

	void cellClicked(int rowNumber, int columnId, const MouseEvent&) override
	{
		if(columnId == COLUMN_ID_GROUP_POINTS)
		{
			CallOutBox::launchAsynchronously(std::make_unique<GroupPointsSelectionComponent>(pSources, rowNumber, this), pTableListBox->getCellPosition(columnId, rowNumber, true).translated(pTableListBox->getScreenX(), pTableListBox->getScreenY()), nullptr);
		}
	}

	void changeListenerCallback(ChangeBroadcaster* /*source*/) override
	{
		getTable()->updateContent();
		getTable()->repaint();
	}

private:
	AmbiSourceSet* pSources;
	PointSelection* pPointSelection;
	Component* pParentComponent;
	TableListBox* pTableListBox;
    ScalingInfo* pScalingInfo;
};
