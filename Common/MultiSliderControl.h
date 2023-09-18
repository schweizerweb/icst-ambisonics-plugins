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

//==============================================================================
/*
*/
class MultiSliderControl    : public Component, public Slider::Listener
{
public:
    MultiSliderControl(int numberOfSliders, double* values, OwnedArray<String>* columnNames, double minVal, double maxVal, double interval);
    ~MultiSliderControl() override;
	
	void updateValues() const;
    void setVisibleSliderCount(int count);

private:
	void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;
	
private:
	OwnedArray<Slider>	sliders;
	OwnedArray<Label>	labels;
	double* pSliderValueArray;
    int visibleSliderCount;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiSliderControl)
};
