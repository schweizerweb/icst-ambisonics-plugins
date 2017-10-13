/*
  ==============================================================================

    MultiSliderControl.cpp
    Created: 13 Oct 2017 3:08:23pm
    Author:  chris

  ==============================================================================
*/

#include "../Decoder/JuceLibraryCode/JuceHeader.h"
#include "MultiSliderControl.h"

//==============================================================================
MultiSliderControl::MultiSliderControl(int numberOfSliders, double* values, OwnedArray<String>* columnNames)
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
		s->setValue(values[i]);
		s->setName(String(i));
		s->setRange(0.0, 1.5, 0.01);
		addAndMakeVisible(s);
		sliders.add(s);

		addAndMakeVisible(infoLabel = new Label("infoLabel", "Ambisonics\r\nchannel\r\nweighting"));
		infoLabel->setJustificationType(Justification::centred);
	}
	pSliderValueArray = values;
}

MultiSliderControl::~MultiSliderControl()
{
}

void MultiSliderControl::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void MultiSliderControl::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	int compWidth = getWidth() / (sliders.size() + 1);
	int labelHeight = 30;
	int sliderHeight = getHeight() - labelHeight;
	infoLabel->setBounds(0, 0, compWidth, getHeight());
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
