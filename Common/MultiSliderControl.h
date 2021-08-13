/*
  ==============================================================================

    MultiSliderControl.h
    Created: 13 Oct 2017 3:08:23pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

//==============================================================================
/*
*/
class MultiSliderControl    : public Component, public Slider::Listener
{
public:
	MultiSliderControl();
    ~MultiSliderControl();

	void init(int numberOfSliders, double* values, OwnedArray<String>* columnNames, double minVal, double maxVal, double interval);
	void updateValues() const;

private:
	void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;
	
private:
	OwnedArray<Slider>	sliders;
	OwnedArray<Label>	labels;
	double* pSliderValueArray;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiSliderControl)
};
