/*
  ==============================================================================

    GroupTableListModel.h
    Created: 9 Nov 2019 8:41:04am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/ColorEditorCustomComponent.h"
#include "GroupPointsSelectionComponent.h"
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
#define COLUMN_ID_GROUP_COLOR	113
#define COLUMN_ID_GROUP_POINTS	114


class GroupTableListModel : public TableListBoxModel, public TableColumnCallback, public ChangeListener
{
public:
	GroupTableListModel(AmbiSourceSet* pSources, PointSelection* pPointSelection, Component* pParentComponent, ScalingInfo* pScaling): pSources(pSources), pPointSelection(pPointSelection), pParentComponent(pParentComponent), pTableListBox(nullptr), pScalingInfo(pScaling)
	{
	}

	~GroupTableListModel() override {}

	int getNumRows() override {
		return pSources->groupCount();
	}

	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
	{
		const Colour alternateColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId)
			.interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), COLOR_DEFINITION_ALTERNATE_INTENSITY));
		if (rowIsSelected)
			g.fillAll(COLOR_DEFINITION_SELECTED_ROW);
		else if (rowNumber % 2)
			g.fillAll(alternateColour);
	}

	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override 
	{
		AmbiGroup* pt = pSources->getGroup(rowNumber);
		if (pt == nullptr)
			return;

		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId));
		String text;
		switch (columnId)
		{
		case COLUMN_ID_GROUP_NB: text = String(rowNumber + 1); break;
		case COLUMN_ID_GROUP_NAME: text = pt->getName(); break;
		case COLUMN_ID_GROUP_POINTS: text = String(pt->groupPoints.size()); break;
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
			|| columnId == COLUMN_ID_GROUP_D)
		{
			NumericColumnCustomComponent* numericBox = static_cast<NumericColumnCustomComponent*> (existingComponentToUpdate);
			if (numericBox == nullptr)
				numericBox = new NumericColumnCustomComponent(*this);

			numericBox->setRowAndColumn(rowNumber, columnId);
			return numericBox;
		}
		else if (columnId == COLUMN_ID_GROUP_NAME)
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
		if (lastRowSelected >= 0 && lastRowSelected < pSources->size())
			pPointSelection->selectGroup(lastRowSelected, false);
	}

	double getValue(int columnId, int rowNumber) override 
	{
		// group table handling
		AmbiGroup* pt = pSources->getGroup(rowNumber);
		if (pt == nullptr)
			return 0.0;

		switch (columnId)
		{
		case COLUMN_ID_GROUP_X: return pt->getPoint()->getX();
		case COLUMN_ID_GROUP_Y: return pt->getPoint()->getY();
		case COLUMN_ID_GROUP_Z: return pt->getPoint()->getZ();
		case COLUMN_ID_GROUP_A: return Constants::RadToGrad(pt->getPoint()->getAzimuth());
		case COLUMN_ID_GROUP_E: return Constants::RadToGrad(pt->getPoint()->getElevation());
		case COLUMN_ID_GROUP_D: return pt->getPoint()->getDistance();
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
		case COLUMN_ID_GROUP_X: pSources->getGroup(rowNumber)->getPoint()->setX(newValue); break;
		case COLUMN_ID_GROUP_Y: pSources->getGroup(rowNumber)->getPoint()->setY(newValue); break;
		case COLUMN_ID_GROUP_Z: pSources->getGroup(rowNumber)->getPoint()->setZ(newValue); break;
		case COLUMN_ID_GROUP_A: pSources->getGroup(rowNumber)->getPoint()->setAzimuth(Constants::GradToRad(newValue)); break;
		case COLUMN_ID_GROUP_E: pSources->getGroup(rowNumber)->getPoint()->setElevation(Constants::GradToRad(newValue)); break;
		case COLUMN_ID_GROUP_D: pSources->getGroup(rowNumber)->getPoint()->setDistance(newValue); break;
		case COLUMN_ID_GROUP_COLOR:
            if(newValue < 0) // code for setting children
            {
                pSources->getGroup(rowNumber)->setChildrenColor();
                pPointSelection->notifyChange();
            }
            else
            {
                pSources->getGroup(rowNumber)->setColor(Colour(uint32(newValue)));
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
		default: return SliderRange(0.0, 1.0, 0.001);
		}
	}

	TableListBox* getTable() override
	{
		return pTableListBox;
	}

	String getTableText(const int columnId, const int rowNumber) override
	{
		AmbiPoint* pt = pSources->getGroup(rowNumber);
		if (pt == nullptr)
			return "";

		switch (columnId)
		{
		case COLUMN_ID_GROUP_NAME: return pt->getName();
		default: return "";
		}
	}

	void setTableText(const int columnId, const int rowNumber, const String& newText) override
	{
		switch (columnId)
		{
		case COLUMN_ID_GROUP_NAME: pSources->setGroupName(rowNumber, newText); break;
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
		tableListBox->getHeader().addColumn("# Points", COLUMN_ID_GROUP_POINTS, 60);
		tableListBox->getHeader().addColumn("Color", COLUMN_ID_GROUP_COLOR, 60);
		tableListBox->getHeader().setStretchToFitActive(true);
		tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
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
