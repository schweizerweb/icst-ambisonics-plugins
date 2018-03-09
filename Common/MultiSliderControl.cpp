/*
  ==============================================================================

    MultiSliderControl.cpp
    Created: 13 Oct 2017 3:08:23pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "JuceHeader.h"
#include "MultiSliderControl.h"

//==============================================================================
MultiSliderControl::MultiSliderControl(int numberOfSliders, double* values, OwnedArray<String>* columnNames, double minVal, double maxVal, double interval)
{
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
    int compWidth = getWidth() / (sliders.size() + 1);
	int labelHeight = 30;
	int sliderHeight = getHeight() - labelHeight;
	
	for(int i = 0; i < labels.size(); i++)
	{
		labels[i]->setBounds((i+1) * compWidth, 0, compWidth, labelHeight);
	}
	
	for(int i = 0; i < sliders.size(); i++)
	{
		sliders[i]->setBounds((i+1) * compWidth, labelHeight, compWidth, sliderHeight);
	}
}

void MultiSliderControl::sliderValueChanged(Slider* slider)
{
	pSliderValueArray[slider->getName().getIntValue()] = slider->getValue();
}