/*
  ==============================================================================

    GainColumnCustomComponent.h
    Created: 9 Oct 2017 2:25:32pm
    Author:  Christian Schweizer

  ==============================================================================
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
