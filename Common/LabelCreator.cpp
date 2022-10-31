/*
  ==============================================================================

    LabelCreator.cpp
    Created: 15 Oct 2018 6:00:01am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "LabelCreator.h"
Image LabelCreator::createNewLabel(String label, Colour newColor, float fontSize)
{
	const MessageManagerLock lock;
	
	Font labelFont(fontSize);
	int width = labelFont.getStringWidth(label);
	if (width <= 0)
		return Image();

	Image img(Image::ARGB, width, int(fontSize+0.5), true);
	Graphics g(img);
	g.setColour(newColor);
	g.setFont(fontSize);
	g.drawSingleLineText(label, 0, int(fontSize+0.5));
	return img;
}

DrawableImage* LabelCreator::createIcon(String str, Colour fillColor, Colour textColor, Colour borderColor, bool strikeThrough)
{
    Image img(Image::ARGB, 20, 20, true);
    img.clear(Rectangle<int>(20, 20), fillColor);

    Graphics g(img);
    g.setFont(16);
    g.setColour(textColor);
    g.drawText(str, 0, 0, 20, 20, Justification::centred);
    g.setColour(borderColor);
    g.drawRect(1, 1, 18, 18);
    
    if(strikeThrough) {
        g.drawLine(0, 0, 20, 20, 2.0f);
    }
    
    auto d = new DrawableImage();
    d->setImage(img);
    return d;
}

DrawableImage* LabelCreator::createColorField(Colour color)
{
    Image img(Image::PixelFormat::RGB, 20, 20, true);
    img.clear(Rectangle<int>(20, 20), color);
    auto d = new DrawableImage();
    d->setImage (img);
    return d;
}
