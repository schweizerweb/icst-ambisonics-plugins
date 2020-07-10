/*
  ==============================================================================

    EditableTextCustomComponent.h
    Created: 9 Oct 2017 2:04:27pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "TableColumnCallback.h"

class EditableTextCustomComponent : public Label
{
public:
	EditableTextCustomComponent(TableColumnCallback& td) : owner(td)
	{
		setEditable(false, true, false);
	}

	void mouseDown(const MouseEvent& event) override
	{
		owner.getTable()->selectRowsBasedOnModifierKeys(row, event.mods, false);
		Label::mouseDown(event);
	}

	void textWasEdited() override
	{
		owner.setTableText(columnId, row, getText());
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		setText(owner.getTableText(columnId, row), dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row);
        setEditable(false, enabled, false);
        setColour(textColourId, enabled ? Colours::white : Colours::darkgrey);
	}

	void paint(Graphics& g) override
	{
		Label::paint(g);
	}

private:
	TableColumnCallback& owner;
	int row, columnId;
	Colour textColour;
};
