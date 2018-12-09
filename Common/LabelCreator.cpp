/*
  ==============================================================================

    LabelCreator.cpp
    Created: 15 Oct 2018 6:00:01am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "LabelCreator.h"
Image* LabelCreator::createNewLabel(String label, Colour newColor, float fontSize)
{
	const MessageManagerLock lock;
	
	ScopedPointer<Font> labelFont = new Font(fontSize);
	int width = labelFont->getStringWidth(label);
	if (width <= 0)
		return new Image();

	Image* img = new Image(Image::ARGB, width, int(fontSize+0.5), true);
	Graphics g(*img);
	g.setColour(newColor);
	g.setFont(fontSize);
	g.drawSingleLineText(label, 0, int(fontSize+0.5));
	return img;
}

