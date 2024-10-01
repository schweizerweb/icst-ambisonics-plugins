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
#include "ColorSelectionComponent.h"

class ColorEditorCustomComponent : public Label, public ChangeListener, public Button::Listener
{
public:
	ColorEditorCustomComponent(TableColumnCallback& td, bool _groupFlag = false) : owner(td), row(0), columnId(0), groupFlag(_groupFlag)
	{
	}

	void mouseDown(const MouseEvent& event) override
	{
        if(isEnabled())
        {
            auto tbl = owner.getTable();
            if (tbl != nullptr)
            {
                tbl->selectRowsBasedOnModifierKeys(row, event.mods, false);
            }

            CallOutBox::launchAsynchronously(std::make_unique<ColorSelectionComponent>(color, this, this, groupFlag), getScreenBounds(), nullptr);
        }
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		color = Colour(uint32(owner.getValue(columnId, row)));
        bool enabled = owner.getEnabled(columnId, row);
        setEnabled(enabled);
        setColour(textColourId, enabled ? Colours::white : Colours::darkgrey);
	}

	void paint(Graphics& g) override
	{
        if(isEnabled())
            g.fillAll(color);
        else
            g.fillAll(color.withAlpha(0.3f));
	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		ColourSelector* selector = dynamic_cast<ColourSelector*> (source);
		if (selector != nullptr)
		{
			uint32 c = uint32(selector->getCurrentColour().getARGB());
			owner.setValue(columnId, row, c);
		}
	}

	void buttonClicked(Button *) override
    {
        owner.setValue(columnId, row, -1);
    }
    
private:
	TableColumnCallback& owner;
	int row, columnId;
	Colour color;
    bool groupFlag;
};
