/*
  =============================================================================

    CustomOscInputTableListModel.
    Created: 17 Jan 2022 10:24:05p
    Author:  Schweizer Christia

  =============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"

#define COLUMN_ID_ENABLE		201
#define	COLUMN_ID_PATH			204
#define COLUMN_ID_COMMAND       205
#define ACTION_MESSAGE_DATA_CHANGED "data"
#define ACTION_MESSAGE_SEL_CHANGED "sel"


class CustomOscInputTableListModel : public TableListBoxModel, public TableColumnCallback, public ActionBroadcaster
{
public:
	CustomOscInputTableListModel(EncoderSettings* pSettings, Component* pParentComponent, ActionListener* pActionListener): pSettings(pSettings), pParentComponent(pParentComponent), pTableListBox(nullptr)
	{
		addActionListener(pActionListener);
	}

	~CustomOscInputTableListModel() override
	{
		removeAllActionListeners();
	}

	int getNumRows() override {
		return pSettings->customOscInput.size();
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

	void paintCell(Graphics& g, int /*rowNumber*/, int /*columnId*/, int width, int height, bool /*rowIsSelected*/) override 
	{
		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}

	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override
	{
		if (columnId == COLUMN_ID_PATH
            || columnId == COLUMN_ID_COMMAND)
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
		sendActionMessage(ACTION_MESSAGE_SEL_CHANGED);
	}

	double getValue(int columnId, int rowNumber) override 
	{
		CustomOscInput* t = pSettings->customOscInput[rowNumber];
		if (t == nullptr)
			return 0.0;

		switch (columnId)
		{
		case COLUMN_ID_ENABLE: return t->enabledFlag;
		default: return 0.0;
		}
	}

	void setValue(int columnId, int rowNumber, double newValue) override
	{
        switch (columnId)
        {
            case COLUMN_ID_ENABLE: pSettings->customOscInput[rowNumber]->enabledFlag = newValue != 0.0; break;
            default: ;
        }
        
		getTable()->updateContent();
		getTable()->repaint();

		sendActionMessage(ACTION_MESSAGE_DATA_CHANGED);
	}
	
	SliderRange getSliderRange(int columnId) override 
	{
		switch (columnId)
		{
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
		case COLUMN_ID_PATH: return pSettings->customOscInput[rowNumber]->oscString; break;
		case COLUMN_ID_COMMAND: return pSettings->customOscInput[rowNumber]->commandString; break;
        default: return "";
		}
	}

	void setTableText(const int columnId, const int rowNumber, const String& newText) override
	{
        switch (columnId)
        {
            case COLUMN_ID_PATH: pSettings->customOscInput[rowNumber]->oscString = newText; break;
            case COLUMN_ID_COMMAND: pSettings->customOscInput[rowNumber]->commandString = newText; break;
            default: ;
        }
        
		sendActionMessage(ACTION_MESSAGE_DATA_CHANGED);
	}

	void initTable(TableListBox* tableListBox)
	{
		pTableListBox = tableListBox;
		tableListBox->setModel(this);
		tableListBox->getHeader().addColumn("Enable", COLUMN_ID_ENABLE, 20);
		tableListBox->getHeader().addColumn("OSC-Message", COLUMN_ID_PATH, 450);
        // TODO: add javascript column
        // tableListBox->getHeader().addColumn("OSC-Command", COLUMN_ID_COMMAND, 150);
		tableListBox->getHeader().setStretchToFitActive(true);
		tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
	}

    bool getEnabled(const int /*columnId*/, const int /*rowNumber*/) override
	{
	    return true;
	}
    
    int getCustomIndex(int selectedIndex)
    {
        return selectedIndex;
    }

private:
    
	EncoderSettings* pSettings;
	Component* pParentComponent;
	TableListBox* pTableListBox;
};
