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

class CheckBoxCustomComponent : public ToggleButton, public ToggleButton::Listener
{
public:
    CheckBoxCustomComponent(TableColumnCallback& td) : owner(td)
    {
        addListener(this);
    }

    void mouseDown(const MouseEvent& event) override
    {
        owner.getTable()->selectRowsBasedOnModifierKeys(row, event.mods, false);
        ToggleButton::mouseDown(event);
    }

    void buttonClicked(Button*) override
    {
        owner.setValue(columnId, row, getToggleState()?1:0);
    }

    void setRowAndColumn(const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        setToggleState(!exactlyEqual(owner.getValue(columnId, row), 0.0), dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row);
        setEnabled(enabled);
    }

    void paint(Graphics& g) override
    {
        auto& lf = getLookAndFeel();
        if (!dynamic_cast<LookAndFeel_V4*> (&lf))
            lf.setColour(textColourId, Colours::black);

        ToggleButton::paint(g);
    }

private:
    TableColumnCallback& owner;
    int row, columnId;
    Colour textColour;
};

