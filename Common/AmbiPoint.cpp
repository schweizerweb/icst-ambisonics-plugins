/*
  ==============================================================================

    AmbiPoint.cpp
    Created: 15 Oct 2018 5:46:32am
    Author:  chris

  ==============================================================================
*/

#include "AmbiPoint.h"
#include "LabelCreator.h"

AmbiPoint::AmbiPoint(AmbiPoint* other, bool copyImage): id(other->id), point(other->point), color(other->color), name(other->name), gain(other->gain), rms(other->rms), lastUpdate(other->lastUpdate)
{
	if (copyImage)
	{
		other->ensureLabelImage();
		if (other->labelImage != nullptr)
		{
			this->labelImage = new Image(*other->labelImage);
			this->labelImage->duplicateIfShared();
		}
	}
}

AmbiPoint::AmbiPoint(): color(Colour()), gain(1.0), rms(0.0f)
{
}

AmbiPoint::AmbiPoint(String id, Point3D<double> point, String name, Colour color, double gain):
	id(id),
	point(point),
	color(color),
	name(name),
	gain(gain),
	rms(0.0f)
{
}

AmbiPoint::AmbiPoint(Point3D<double> point, AudioParameterSet audioParams, String name, Colour color, double gain):
	id(Uuid().toString()),
	point(Point3D<double>(point.getX(), point.getY(), point.getZ(), audioParams)),
	color(color),
	name(name),
	gain(gain),
	rms(0.0f)
{
}

AmbiPoint::AmbiPoint(XmlElement* element):
	id(Uuid().toString()),
	point(Point3D<double>(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_X),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Y),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Z))),
	color(Colour(uint32(element->getIntAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR))).withAlpha(1.0f)),
	name(element->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME)),
	gain(element->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, 1.0)),
	rms(0.0f)
{
}

AmbiPoint::AmbiPoint(XmlElement* element, AudioParameterSet audioParams):
	id(Uuid().toString()),
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

AmbiPoint::~AmbiPoint()
{
}

Point3D<double>* AmbiPoint::getPoint()
{
	return &point;
}

Colour AmbiPoint::getColor() const
{
	return color;
}

String AmbiPoint::getName() const
{
	return name;
}

void AmbiPoint::setName(String newName)
{
	if (name != newName)
	{
		Image* img = LabelCreator::createNewLabel(newName, color, FONT_SIZE);
		labelImage = img;
	}

	name = newName;
}

double AmbiPoint::getGain() const
{
	return gain;
}

XmlElement* AmbiPoint::getAsXmlElement(String tagName)
{
	XmlElement* element = new XmlElement(tagName);
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_ID, getId());
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_X, getPoint()->getX());
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Y, getPoint()->getY());
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Z, getPoint()->getZ());
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME, getName());
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR, int(getColor().getARGB()));
	element->setAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, getGain());
	return element;
}

void AmbiPoint::setGain(double newGain)
{
	gain = newGain;
}

void AmbiPoint::setRms(float newRmsLevel, bool onlyIfGreater)
{
	if (!onlyIfGreater || newRmsLevel > rms)
		rms = newRmsLevel;
}

float AmbiPoint::getRms() const
{
	return rms;
}

String AmbiPoint::getId()
{
	if (id.isEmpty())
	{
		id = Uuid().toString();
	}

	return id;
}

void AmbiPoint::setColor(Colour newColor)
{
	if (color != newColor)
	{
		Image* img = LabelCreator::createNewLabel(name, newColor, FONT_SIZE);
		labelImage = img;
	}

	color = newColor;
}

bool AmbiPoint::checkAlive(int64 referenceTime, int timeout) const
{
	return (lastUpdate == 0 || referenceTime - lastUpdate < timeout);
}

void AmbiPoint::setAlive(int64 currentTimeMillis)
{
	lastUpdate = currentTimeMillis;
}

Image* AmbiPoint::getLabelImage()
{
	ensureLabelImage();
	return labelImage;
}

void AmbiPoint::ensureLabelImage()
{
	if (labelImage == nullptr)
	{
		labelImage = LabelCreator::createNewLabel(name, color, FONT_SIZE);
		labelImage->duplicateIfShared();
	}
}