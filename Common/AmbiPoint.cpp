/*
  ==============================================================================

    AmbiPoint.cpp
    Created: 15 Oct 2018 5:46:32am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiPoint.h"
#include "LabelCreator.h"

AmbiPoint::AmbiPoint(AmbiPoint* other, bool copyImage): audioParams(other->audioParams), id(other->id), point(other->point), color(other->color), name(other->name), gain(other->gain), lastUpdate(other->lastUpdate), enabled(other->enabled), pGroup(other->pGroup)
{
	if (copyImage)
	{
		other->ensureLabelImage();
		if (other->labelImage != Image())
		{
			this->labelImage = Image(other->labelImage);
			this->labelImage.duplicateIfShared();
		}
	}
}

AmbiPoint::AmbiPoint(): color(Colour()), gain(1.0), pGroup(nullptr)
{
}

AmbiPoint::AmbiPoint(String id, Point3D<double> point, String name, Colour color, double gain, bool mute, bool solo):
    audioParams(point.getAudioParameterSet()),
    id(id),
	point(point),
	color(color),
	name(name),
	gain(gain),
    mute(mute),
    solo(solo),
    enabled(true),
    pGroup(nullptr)
{
}

AmbiPoint::AmbiPoint(XmlElement* element):
	id(element->getStringAttribute(XML_ATTRIBUTE_POINT_ID, Uuid().toString())),
	point(Point3D<double>(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_X),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_POINT_Y),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_POINT_Z))),
	color(loadColorAttribute(element)),
	name(element->getStringAttribute(XML_ATTRIBUTE_POINT_NAME)),
	gain(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_GAIN, 1.0)),
    mute(element->getBoolAttribute(XML_ATTRIBUTE_POINT_MUTE, false)),
    solo(false),
    enabled(element->getBoolAttribute(XML_ATTRIBUTE_POINT_ENABLED, true)),
    pGroup(nullptr)
{
}

AmbiPoint::AmbiPoint(XmlElement* element, AudioParameterSet audioParams):
    audioParams(audioParams),
    id(element->getStringAttribute(XML_ATTRIBUTE_POINT_ID, Uuid().toString())),
	point(Point3D<double>(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_X),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_POINT_Y),
	                      element->getDoubleAttribute(XML_ATTRIBUTE_POINT_Z),
	                      audioParams)),
	color(loadColorAttribute(element)),
	name(element->getStringAttribute(XML_ATTRIBUTE_POINT_NAME)),
	gain(element->getDoubleAttribute(XML_ATTRIBUTE_POINT_GAIN, 1.0)),
    mute(element->getBoolAttribute(XML_ATTRIBUTE_POINT_MUTE, false)),
    solo(false),
    enabled(element->getBoolAttribute(XML_ATTRIBUTE_POINT_ENABLED, true)),
    pGroup(nullptr)
{
}

AmbiPoint::AmbiPoint(AudioParameterSet audioParams) :
    audioParams(audioParams),
    id(Uuid().toString()),
    point(Point3D<double>(0.0, 0.0, 0.0, audioParams)),
    color(Colours::black),
    gain(1.0),
    mute(false),
    solo(false),
    enabled(false),
    pGroup(nullptr)
{
}

AmbiPoint::~AmbiPoint()
{
}

XmlElement* AmbiPoint::getBaseXmlElement(String tagName)
{
	XmlElement* element = new XmlElement(tagName);
	element->setAttribute(XML_ATTRIBUTE_POINT_ID, getId());
	element->setAttribute(XML_ATTRIBUTE_POINT_X, getRawPoint()->getX());
	element->setAttribute(XML_ATTRIBUTE_POINT_Y, getRawPoint()->getY());
	element->setAttribute(XML_ATTRIBUTE_POINT_Z, getRawPoint()->getZ());
	element->setAttribute(XML_ATTRIBUTE_POINT_NAME, getName());
	element->setAttribute(XML_ATTRIBUTE_POINT_COLOR_NEW, getColor().toString());
	element->setAttribute(XML_ATTRIBUTE_POINT_GAIN, getGain());
    element->setAttribute(XML_ATTRIBUTE_POINT_MUTE, getMute());
    element->setAttribute(XML_ATTRIBUTE_POINT_ENABLED, getEnabled());
	return element;
}

Point3D<double>* AmbiPoint::getRawPoint()
{
	return &point;
}

Vector3D<double> AmbiPoint::getVector3D()
{
    return Vector3D<double>(point.getX(), point.getY(), point.getZ());
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
		labelImage = LabelCreator::createNewLabel(newName, color, FONT_SIZE);
	}

	name = newName;
}

double AmbiPoint::getGain() const
{
	return gain;
}

void AmbiPoint::setGain(double newGain, bool notify)
{
	gain = newGain;
    
    if(notify)
        audioParams.notifyGain(gain);
}

bool AmbiPoint::getMute() const
{
    return allowMute() ? mute : false;
}

void AmbiPoint::setMute(bool newMute, bool notify)
{
    if(!allowMute())
        return;
    
    mute = newMute;
    if(newMute && solo)
        setSolo(false);
    
    if(notify)
        audioParams.notifyMute(mute);
}

bool AmbiPoint::getSolo() const
{
    return allowSolo() ? solo : false;
}

void AmbiPoint::setSolo(bool newSolo)
{
    if(!allowSolo())
        return;
    
    solo = newSolo;
    if(newSolo && mute)
        setMute(false, true);
}

String AmbiPoint::getId()
{
	if (id.isEmpty())
	{
		id = Uuid().toString();
	}

	return id;
}

void AmbiPoint::resetId()
{
	id = Uuid().toString();
}

void AmbiPoint::setColor(Colour newColor)
{
	newColor = newColor.withAlpha(1.0f);
	if (color != newColor)
	{
		labelImage = LabelCreator::createNewLabel(name, newColor, FONT_SIZE);
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
	return &labelImage;
}

void AmbiPoint::ensureLabelImage()
{
	if (labelImage == Image())
	{
		labelImage = LabelCreator::createNewLabel(name, color, FONT_SIZE);
		labelImage.duplicateIfShared();
	}
}

bool AmbiPoint::getEnabled()
{
    return enabled;
}

void AmbiPoint::setEnabled(bool enable)
{
    enabled = enable;
}

Colour AmbiPoint::loadColorAttribute(XmlElement* element)
{
	if (element->hasAttribute(XML_ATTRIBUTE_POINT_COLOR_NEW))
		return Colour::fromString(element->getStringAttribute(XML_ATTRIBUTE_POINT_COLOR_NEW));

    return Colour(uint32(element->getIntAttribute(XML_ATTRIBUTE_POINT_COLOR_OLD))).withAlpha(1.0f);
}

void AmbiPoint::setGroup(AmbiGroup* pG) {
    this->pGroup = pG;
}

AmbiGroup* AmbiPoint::getGroup() {
    return pGroup;
}


