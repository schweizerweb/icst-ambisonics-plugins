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
