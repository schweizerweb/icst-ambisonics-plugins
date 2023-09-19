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

	void setRowAndColumn(const int newRow, const int newColumn, const bool readOnly = false)
	{
		row = newRow;
		columnId = newColumn;
		setText(owner.getTableText(columnId, row), dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row) && !readOnly;
        setEditable(false, enabled, false);
        setColour(textColourId, readOnly ? Colours:: yellow : (enabled ? Colours::white : Colours::darkgrey));
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
