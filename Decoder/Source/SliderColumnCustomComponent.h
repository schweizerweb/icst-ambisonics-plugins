/*
  ==============================================================================

    GainColumnCustomComponent.h
    Created: 9 Oct 2017 2:25:32pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"

class SliderColumnCustomComponent : public Component,
	private SliderListener
{
public:
	SliderColumnCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
		addAndMakeVisible(slider);
		slider.setSliderStyle(Slider::LinearBar);
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
	}

	void sliderValueChanged(Slider* changedSlider) override
	{
		owner.setValue(columnId, row, changedSlider->getValue());
	}

	
private:
	SpeakerSettingsComponent& owner;
	Slider slider;
	int row, columnId;
};