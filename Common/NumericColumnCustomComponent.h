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
#include "TableColumnCallback.h"
#include "SliderRange.h"


class NumericColumnCustomComponent : public Label
{
public:
	NumericColumnCustomComponent(TableColumnCallback& td) : owner(td)
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
		if(!getText().containsOnly("-0123456789."))
		{
			setRowAndColumn(row, columnId);
		}
		else 
		{
			double dbl = getText().getDoubleValue();
			dbl = jmax(range.min, dbl);
			dbl = jmin(range.max, dbl);
			owner.setValue(columnId, row, dbl);
		}
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		range = SliderRange(owner.getSliderRange(columnId));
		setText(String(owner.getValue(columnId, row), 3), dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row);
        setEditable(false, enabled, false);
        setColour(textColourId, enabled ? Colours::white : Colours::darkgrey);
	}

	void paint(Graphics& g) override
	{
		auto& lf = getLookAndFeel();
		if (!dynamic_cast<LookAndFeel_V4*> (&lf))
			lf.setColour(textColourId, Colours::black);

		Label::paint(g);
	}

private:
	TableColumnCallback& owner;
	SliderRange range;
	int row, columnId;
};
