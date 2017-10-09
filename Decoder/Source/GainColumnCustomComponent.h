/*
  ==============================================================================

    GainColumnCustomComponent.h
    Created: 9 Oct 2017 2:25:32pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"

class GainColumnCustomComponent : public Component,
	private SliderListener
{
public:
	GainColumnCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
		addAndMakeVisible(slider);
		slider.setSliderStyle(Slider::LinearBar);
		slider.setRange(0.0, 1.2, 0.001);
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
		slider.setValue(owner.getGain(row), dontSendNotification);
	}

	void sliderValueChanged(Slider* slider) override
	{
		owner.setGain(row, slider->getValue());
	}

	
private:
	SpeakerSettingsComponent& owner;
	Slider slider;
	int row, columnId;
};