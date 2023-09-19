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

class SliderColumnCustomComponent : public Component,
	private Slider::Listener
{
public:
	SliderColumnCustomComponent(TableColumnCallback& td) : owner(td)
	{
		addAndMakeVisible(slider);
		slider.setSliderStyle(Slider::IncDecButtons);
		slider.addListener(this);
		slider.setWantsKeyboardFocus(false);
	}

	void resized() override
	{
		slider.setBoundsInset(BorderSize<int>(2));
	}

	void setRowAndColumn(int newRow, int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		SliderRange range = owner.getSliderRange(columnId);
		slider.setRange(range.min, range.max, range.interval);
		slider.setValue(owner.getValue(columnId, row), dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row);
        slider.setEnabled(enabled);
        setColour(Slider::textBoxTextColourId, enabled ? Colours::white : Colours::darkgrey);
        setColour(Slider::thumbColourId, enabled ? Colours::white : Colours::darkgrey);
	}

	void sliderValueChanged(Slider* changedSlider) override
	{
		owner.setValue(columnId, row, changedSlider->getValue());
	}

	
private:
	TableColumnCallback& owner;
	Slider slider;
	int row, columnId;
};
