/*
  ==============================================================================

    MultiSliderControl.h
    Created: 13 Oct 2017 3:08:23pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include "../Decoder/JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MultiSliderControl    : public Component, public SliderListener
{
public:
    MultiSliderControl(int numberOfSliders, double* values, OwnedArray<String>* columnNames, double minVal, double maxVal, double interval);
    ~MultiSliderControl();
	
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
