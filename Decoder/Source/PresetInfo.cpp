/*
  ==============================================================================

    PresetInfo.cpp
    Created: 30 Jul 2017 5:12:01pm
    Author:  chris

  ==============================================================================
*/

#include "PresetInfo.h"

PresetInfo::PresetInfo()
{
	ambiSettings = new AmbiSettings();
}

bool PresetInfo::LoadFromFile(File file)
{
	points.clear();
	
	if (!file.existsAsFile())
		return false;

	ScopedPointer<XmlDocument> xmlDoc = new XmlDocument(file);
	ScopedPointer<XmlElement> root = xmlDoc->getDocumentElement();
	if (root->getTagName() != XML_TAG_PRESET_ROOT)
		return false;

	name = root->getStringAttribute(XML_ATTRIBUTE_PRESET_NAME);

	// general
	XmlElement* xmlGeneral = root->getChildByName(XML_TAG_PRESET_GENERAL);
	if (xmlGeneral == nullptr)
		return false;

	// ambisonics settings
	ambiSettings = new AmbiSettings();
	XmlElement* xmlDistanceScaler = xmlGeneral->getChildByName(XML_TAG_PRESET_DISTANCESCALER);
	if (xmlDistanceScaler != nullptr)
	{
		ambiSettings->setDistanceScaler(xmlDistanceScaler->getDoubleAttribute(XML_VALUE));
	}
	XmlElement* xmlFlipDirection = xmlGeneral->getChildByName(XML_TAG_PRESET_FLIPDIRECTION);
	if (xmlFlipDirection != nullptr)
	{
		ambiSettings->setDirectionFlip(xmlFlipDirection->getBoolAttribute(XML_VALUE));
	}
	XmlElement* xmlAmbiChannelWeight = xmlGeneral->getChildByName(XML_TAG_PRESET_AMBICHANNELWEIGHT);
	int index = 0;
	if (xmlAmbiChannelWeight != nullptr)
	{
		String attributeName;
		while (xmlAmbiChannelWeight->hasAttribute(attributeName = "Order" + String(index)))
		{
			ambiSettings->getAmbiOrderWeightPointer()[index] = xmlAmbiChannelWeight->getDoubleAttribute(attributeName);
			index++;
		}
	}

	// points
	XmlElement* xmlPoints = root->getChildByName(XML_TAG_PRESET_POINTS);
	if (xmlPoints == nullptr)
		return false;

	XmlElement* xmlPoint = xmlPoints->getChildByName(XML_TAG_PRESET_POINT);
	while (xmlPoint != nullptr)
	{
		points.add(new AmbiPoint(
			Point3D<double>(
				xmlPoint->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_X),
				xmlPoint->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Y),
				xmlPoint->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_Z)),
			xmlPoint->getStringAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME),
			xmlPoint->getIntAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR),
			xmlPoint->getDoubleAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, 1.0)
		));
		xmlPoint = xmlPoint->getNextElement();
	}

	return true;
}

bool PresetInfo::SaveToFile(File file)
{
	ScopedPointer<XmlElement> xmlRoot = new XmlElement(XML_TAG_PRESET_ROOT);
	xmlRoot->setAttribute(XML_ATTRIBUTE_PRESET_NAME, name);
	
	// general
	XmlElement* xmlGeneral = new XmlElement(XML_TAG_PRESET_GENERAL);
	
	XmlElement* xmlDistanceScaler = new XmlElement(XML_TAG_PRESET_DISTANCESCALER);
	xmlDistanceScaler->setAttribute(XML_VALUE, ambiSettings->getDistanceScaler());
	xmlGeneral->addChildElement(xmlDistanceScaler);
	
	XmlElement* xmlFlipDirection = new XmlElement(XML_TAG_PRESET_FLIPDIRECTION);
	xmlFlipDirection->setAttribute(XML_VALUE, ambiSettings->getDirectionFlip());
	xmlGeneral->addChildElement(xmlFlipDirection);

	XmlElement* xmlAmbiChannelWeight = new XmlElement(XML_TAG_PRESET_AMBICHANNELWEIGHT);
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
	{
		xmlAmbiChannelWeight->setAttribute("Order" + String(i), ambiSettings->getAmbiOrderWeightPointer()[i]);
	}
	xmlGeneral->addChildElement(xmlAmbiChannelWeight);
	xmlRoot->addChildElement(xmlGeneral);

	// points
	XmlElement* xmlPoints = new XmlElement(XML_TAG_PRESET_POINTS);
	for (AmbiPoint* pt : points)
	{
		XmlElement* xmlPt = new XmlElement(XML_TAG_PRESET_POINT);
		xmlPt->setAttribute(XML_ATTRIBUTE_PRESET_POINT_X, pt->getPoint()->getX());
		xmlPt->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Y, pt->getPoint()->getY());
		xmlPt->setAttribute(XML_ATTRIBUTE_PRESET_POINT_Z, pt->getPoint()->getZ());
		xmlPt->setAttribute(XML_ATTRIBUTE_PRESET_POINT_NAME, pt->getName());
		xmlPt->setAttribute(XML_ATTRIBUTE_PRESET_POINT_COLOR, pt->getColorIndex());
		xmlPt->setAttribute(XML_ATTRIBUTE_PRESET_POINT_GAIN, pt->getGain());
		xmlPoints->addChildElement(xmlPt);
	}
	xmlRoot->addChildElement(xmlPoints);

	String xmlDocStr = xmlRoot->createDocument("");
	
	bool ret = file.replaceWithText(xmlDocStr.toWideCharPointer());
	if (!ret)
		return false;

	xmlRoot->deleteAllChildElements();

	return true;
}

OwnedArray<AmbiPoint>* PresetInfo::getPoints()
{
	return &points;
}

String PresetInfo::getName() const
{
	return name;
}

void PresetInfo::setName(String newName)
{
	name = newName;
}

AmbiSettings* PresetInfo::getAmbiSettings()
{
	return ambiSettings;
}
