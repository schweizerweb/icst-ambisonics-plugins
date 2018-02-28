/*
  ==============================================================================

    AmbiPoint.h
    Created: 3 Sep 2016 11:29:07am
    Author:  christian.schweizer

  ==============================================================================
*/

#ifndef AMBIPOINT_H_INCLUDED
#define AMBIPOINT_H_INCLUDED

#include "Point3D.h"

#define XML_ATTRIBUTE_PRESET_POINT_X "X"
#define XML_ATTRIBUTE_PRESET_POINT_Y "Y"
#define XML_ATTRIBUTE_PRESET_POINT_Z "Z"
#define XML_ATTRIBUTE_PRESET_POINT_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_COLOR "Color"
#define XML_ATTRIBUTE_PRESET_POINT_GAIN "Gain"

class AmbiPoint
{
public:
	AmbiPoint(): colorIndex(0), gain(1.0)
	{
	}

	AmbiPoint(Point3D<double> point, String name, int colorIndex = 0, double gain = 1.0) :
		point(point),
		colorIndex(colorIndex), 
		name(name),
		gain(gain)
	{
	}

	AmbiPoint(XmlElement* element) :
	point(Point3D<double>(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_X),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Y),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Z))),
	colorIndex(element->getIntAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR)),
	name(element->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME)),
	gain(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, 1.0))
	{
	}

	Point3D<double>* getPoint()
	{
		return &point;
	}

	int getColorIndex() const
	{
		return colorIndex;
	}

	String getName() const
	{
		return name;
	}

	void setName(String newName)
	{
		name = newName;
	}

	double getGain() const
	{
		return gain;
	}

	XmlElement* getAsXmlElement(String tagName)
	{
		XmlElement* element =  new XmlElement(tagName);
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_X, getPoint()->getX());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Y, getPoint()->getY());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Z, getPoint()->getZ());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME, getName());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR, getColorIndex());
		element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, getGain());
		return element;
	}

	void setGain(double newGain)
	{
		gain = newGain;
	}

private:
	Point3D<double> point;
	int colorIndex;
	String name;
	double gain;
};



#endif  // AMBIPOINT_H_INCLUDED
