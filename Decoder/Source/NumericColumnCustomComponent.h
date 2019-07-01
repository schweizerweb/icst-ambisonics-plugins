/*
  ==============================================================================

    NumericColumnCustomComponent.h
    Created: 1 Jul 2019 11:12:19am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"

class NumericColumnCustomComponent : public Label
{
public:
	NumericColumnCustomComponent(SpeakerSettingsComponent& td) : owner(td)
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
			if (range != nullptr)
			{
				dbl = jmax(range->min, dbl);
				dbl = jmin(range->max, dbl);
				owner.setValue(columnId, row, dbl);
			}
		}
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		range = new SliderRange(owner.getSliderRange(columnId));
		setText(String(owner.getValue(columnId, row), 3), dontSendNotification);
	}

	void paint(Graphics& g) override
	{
		auto& lf = getLookAndFeel();
		if (!dynamic_cast<LookAndFeel_V4*> (&lf))
			lf.setColour(textColourId, Colours::black);

		Label::paint(g);
	}

private:
	SpeakerSettingsComponent& owner;
	ScopedPointer<SliderRange> range;
	int row, columnId;
};