/*
  ==============================================================================

    CustomOscTableListModel.h
    Created: 24 Jan 2021 9:53:48pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"

#define COLUMN_ID_ENABLE		201
#define COLUMN_ID_HOST			202
#define	COLUMN_ID_PORT			203
#define	COLUMN_ID_PATH			204


class CustomOscTableListModel : public TableListBoxModel, public TableColumnCallback, public ChangeBroadcaster
{
public:
	CustomOscTableListModel(EncoderSettings* pSettings, Component* pParentComponent, ChangeListener* pChangeListener): pSettings(pSettings), pParentComponent(pParentComponent), pTableListBox(nullptr)
	{
		addChangeListener(pChangeListener);
	}

	~CustomOscTableListModel() override
	{
		removeAllChangeListeners();
	}

	int getNumRows() override {
		return pSettings->customOscTargets.size();
	}

	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
	{
		const Colour alternateColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId)
			.interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
		if (rowIsSelected)
			g.fillAll(Colours::lightblue);
		else if (rowNumber % 2)
			g.fillAll(alternateColour);
	}

	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override 
	{
		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}

	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override
	{
		if (columnId == COLUMN_ID_PORT)
		{
			SliderColumnCustomComponent* sliderBox = static_cast<SliderColumnCustomComponent*> (existingComponentToUpdate);
			if (sliderBox == nullptr)
				sliderBox = new SliderColumnCustomComponent(*this);

			sliderBox->setRowAndColumn(rowNumber, columnId);
			return sliderBox;
		}
		else if (columnId == COLUMN_ID_HOST
			|| columnId == COLUMN_ID_PATH)
		{
			EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
			if (textLabel == nullptr)
				textLabel = new EditableTextCustomComponent(*this);

			textLabel->setRowAndColumn(rowNumber, columnId);
			return textLabel;
		}
		else if (columnId == COLUMN_ID_ENABLE)
		{
			CheckBoxCustomComponent* checkBox = static_cast<CheckBoxCustomComponent*> (existingComponentToUpdate);
			if (checkBox == nullptr)
				checkBox = new CheckBoxCustomComponent(*this);

			checkBox->setRowAndColumn(rowNumber, columnId);
			return checkBox;
		}
		
		return nullptr;
	}

	void selectedRowsChanged(int /*lastRowSelected*/) override
	{
	}

	double getValue(int columnId, int rowNumber) override 
	{
		// group table handling
		CustomOscTarget* t = pSettings->customOscTargets[rowNumber];
		if (t == nullptr)
			return 0.0;

		switch (columnId)
		{
		case COLUMN_ID_PORT: return t->targetPort;
		case COLUMN_ID_ENABLE: return t->enabledFlag;
		default: return 0.0;
		}
	}

    bool getEnabled(const int columnId, const int rowNumber) override
    {
		if (columnId == COLUMN_ID_HOST 
			|| columnId == COLUMN_ID_PORT
			|| columnId == COLUMN_ID_PATH)
			return pSettings->customOscTargets[rowNumber]->enabledFlag;

        return true;
    }
    
	void setValue(int columnId, int rowNumber, double newValue) override
	{
		switch (columnId)
		{
		case COLUMN_ID_PORT: pSettings->customOscTargets[rowNumber]->targetPort = (int)newValue; break;
		case COLUMN_ID_ENABLE: pSettings->customOscTargets[rowNumber]->enabledFlag = newValue != 0.0; break;
		default: ;
		}

		getTable()->updateContent();
		getTable()->repaint();

		sendChangeMessage();
	}
	
	SliderRange getSliderRange(int columnId) override 
	{
		switch (columnId)
		{
		case COLUMN_ID_PORT:
			return SliderRange(0, 65535, 1);
		default: return SliderRange(0.0, 1.0, 0.001);
		}
	}

	TableListBox* getTable() override
	{
		return pTableListBox;
	}

	String getTableText(const int columnId, const int rowNumber) override
	{
		switch (columnId)
		{
		case COLUMN_ID_HOST: return pSettings->customOscTargets[rowNumber]->targetHost; break;
		case COLUMN_ID_PATH: return pSettings->customOscTargets[rowNumber]->oscString; break;
		default: return "";
		}
	}

	void setTableText(const int columnId, const int rowNumber, const String& newText) override
	{
		switch (columnId)
		{
		case COLUMN_ID_HOST: pSettings->customOscTargets[rowNumber]->targetHost = newText; break;
		case COLUMN_ID_PATH: pSettings->customOscTargets[rowNumber]->oscString = newText; break;
		default: ;
		}

		sendChangeMessage();
	}

	void initTable(TableListBox* tableListBox)
	{
		pTableListBox = tableListBox;
		tableListBox->setModel(this);
		tableListBox->getHeader().addColumn("Enable", COLUMN_ID_ENABLE, 30);
		tableListBox->getHeader().addColumn("Host", COLUMN_ID_HOST, 100);
		tableListBox->getHeader().addColumn("Port", COLUMN_ID_PORT, 50);
		tableListBox->getHeader().addColumn("Command", COLUMN_ID_PATH, 200);
		tableListBox->getHeader().setStretchToFitActive(true);
		tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
	}

	void cellClicked(int rowNumber, int columnId, const MouseEvent&) override
	{
		//if(columnId == COLUMN_ID_GROUP_POINTS)
		{
		}

		int i = rowNumber;
	}

private:
	EncoderSettings* pSettings;
	Component* pParentComponent;
	TableListBox* pTableListBox;
};
