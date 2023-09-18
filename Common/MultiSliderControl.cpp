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



#include "JuceHeader.h"
#include "MultiSliderControl.h"

//==============================================================================
MultiSliderControl::MultiSliderControl(int numberOfSliders, double* values, OwnedArray<String>* columnNames, double minVal, double maxVal, double interval)
{
    // initially make all sliders visible
    visibleSliderCount = numberOfSliders;
    
	// create sliders
	for(int i = 0; i < numberOfSliders; i++)
	{
		Label* l = new Label(String(i), (columnNames != nullptr && i < columnNames->size()) ? *columnNames->getUnchecked(i) : String(i));
		l->setJustificationType(Justification::centred);
		addAndMakeVisible(l);
		labels.add(l);

		Slider* s = new Slider(Slider::LinearVertical, Slider::TextBoxBelow);
		s->addListener(this);
		s->setName(String(i));
		s->setRange(minVal, maxVal, interval);
		addAndMakeVisible(s);
		sliders.add(s);
	}

	pSliderValueArray = values;
	updateValues();
}

MultiSliderControl::~MultiSliderControl()
{
}

void MultiSliderControl::setVisibleSliderCount(int count)
{
    visibleSliderCount = jmin(count, sliders.size());
    resized();
}

void MultiSliderControl::updateValues() const
{
	for(int i = 0; i < sliders.size(); i++)
		sliders[i]->setValue(pSliderValueArray[i]);
}

void MultiSliderControl::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void MultiSliderControl::resized()
{
    int compWidth = getWidth() / (visibleSliderCount);
	int labelHeight = 30;
	int sliderHeight = getHeight() - labelHeight - 8;
	
	for(int i = 0; i < labels.size(); i++)
	{
        if(i < visibleSliderCount)
        {
            labels[i]->setBounds(i * compWidth, 0, compWidth, labelHeight);
            labels[i]->setVisible(true);
        }
        else
        {
            labels[i]->setVisible(false);
        }
	}
	
	for(int i = 0; i < sliders.size(); i++)
	{
        if(i < visibleSliderCount)
        {
            sliders[i]->setBounds(i * compWidth, labelHeight, compWidth, sliderHeight);
            sliders[i]->setVisible(true);
        }
        else
        {
            sliders[i]->setVisible(false);
        }
	}
}

void MultiSliderControl::sliderValueChanged(Slider* slider)
{
	pSliderValueArray[slider->getName().getIntValue()] = slider->getValue();
}
