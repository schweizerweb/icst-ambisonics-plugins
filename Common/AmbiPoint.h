/*
  ==============================================================================

    AmbiPoint.h
    Created: 3 Sep 2016 11:29:07am
    Author:  Christian Schweizer

  ==============================================================================
*/

#ifndef AMBIPOINT_H_INCLUDED
#define AMBIPOINT_H_INCLUDED

#include "Point3D.h"

#define XML_ATTRIBUTE_PRESET_POINT_ID "Uuid"
#define XML_ATTRIBUTE_PRESET_POINT_X "X"
#define XML_ATTRIBUTE_PRESET_POINT_Y "Y"
#define XML_ATTRIBUTE_PRESET_POINT_Z "Z"
#define XML_ATTRIBUTE_PRESET_POINT_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_COLOR "Color"
#define XML_ATTRIBUTE_PRESET_POINT_GAIN "Gain"
#define FONT_SIZE	20

class AmbiPoint
{
public:
	AmbiPoint(): color(Colour()), gain(1.0), rms(0.0f)
	{
	}

	AmbiPoint(String id, Point3D<double> point, String name, Colour color = Colour(), double gain = 1.0) :
		id(id),
		point(point),
		color(color),
		name(name),
		gain(gain), 
		rms(0.0f)
	{
	}

	AmbiPoint(Point3D<double> point, AudioParameterSet audioParams, String name, Colour color = Colour(), double gain = 1.0) :
		id(Uuid().toString()),
		point(Point3D<double>(point.getX(), point.getY(), point.getZ(), audioParams)),
		color(color),
		name(name),
		gain(gain),
		rms(0.0f)
	{
	}

	AmbiPoint(XmlElement* element) :
		id(element->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_ID, Uuid().toString())),
		point(Point3D<double>(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_X),
		                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Y),
		                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Z))),
		color(Colour(uint32(element->getIntAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR))).withAlpha(1.0f)),
		name(element->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME)),
		gain(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, 1.0)), 
		rms(0.0f)
	{
	}

	AmbiPoint(XmlElement* element, AudioParameterSet audioParams) :
		id(element->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_ID, Uuid().toString())),
		point(Point3D<double>(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_X),
		                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Y),
		                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Z),
		                      audioParams)),
		color(Colour(uint32(element->getIntAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR))).withAlpha(1.0f)),
		name(element->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME)),
		gain(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, 1.0)), 
		rms(0.0f)
	{
	}

	Point3D<double>* getPoint()
	{
		return &point;
	}

	Colour getColor() const
	{
		return color;
	}

	String getName() const
	{
		return name;
	}

	void setName(String newName)
	{
		if (name != newName)
		{
			Image* img = createNewLabel(newName, color);
			labelImage = img;
		}

		name = newName;
	}

	double getGain() const
	{
		return gain;
	}

	XmlElement* getAsXmlElement(String tagName)
	{
		XmlElement* element =  new XmlElement(tagName);
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_ID, getId());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_X, getPoint()->getX());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Y, getPoint()->getY());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Z, getPoint()->getZ());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME, getName());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR, int(getColor().getARGB()));
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, getGain());
		return element;
	}

	void setGain(double newGain)
	{
		gain = newGain;
	}

	void setRms(float newRmsLevel, bool onlyIfGreater = false)
	{
		if(!onlyIfGreater || newRmsLevel > rms)
			rms = newRmsLevel;
	}

	float getRms(bool reset = false)
	{
		float ret = rms;
		if (reset)
			rms = 0;
		return ret;
	}

	String getId()
	{
		if (id.isEmpty())
		{
			id = Uuid().toString();
		}

		return id;
	}

	void setColor(Colour newColor)
	{
		if (color != newColor)
		{
			Image* img = createNewLabel(name, newColor);
			labelImage = img;
		}

		color = newColor;
	}

	bool checkAlive(int64 referenceTime, int timeout) const
	{
		return (lastUpdate == 0 || referenceTime - lastUpdate < timeout);
	}

	void setAlive(int64 currentTimeMillis)
	{
		lastUpdate = currentTimeMillis;
	}

	Image* getLabelImage()
	{
		if (labelImage == nullptr)
			labelImage = createNewLabel(name, color);
		return new Image(*labelImage);
	}

private:
	Image* createNewLabel(String label, Colour color) const
	{
		const MessageManagerLock lock;
		int width = labelFont.getStringWidth(label);
		Image* img = new Image(Image::ARGB, width, FONT_SIZE, true);
		Graphics g(*img);
		g.setColour(color);
		g.setFont(labelFont);
		g.drawSingleLineText(label, 0, FONT_SIZE);
		return img;
	}
	const Font labelFont = Font(FONT_SIZE);

private:
	String id;
	Point3D<double> point;
	Colour color;
	String name;
	double gain;
	float rms;
	int64 lastUpdate = 0;
	ScopedPointer<Image> labelImage;
};



#endif  // AMBIPOINT_H_INCLUDED
