/*
  ==============================================================================

    NumericColumnCustomComponent.h
    Created: 1 Jul 2019 11:12:19am
    Author:  Christian Schweizer

  ==============================================================================
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
