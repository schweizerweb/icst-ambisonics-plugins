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



#include "AmbiSpeakerSet.h"
#include "TrackColors.h"

AmbiSpeaker* AmbiSpeakerSet::get(int index) const
{
	return elements[index];
}

int AmbiSpeakerSet::size() const
{
	return elements.size();
}

void AmbiSpeakerSet::clear()
{
	const ScopedLock lock(cs);
	while (elements.size() > 0)
	{
		remove(0);
	}
}

void AmbiSpeakerSet::add(AmbiSpeaker* pt)
{
	const ScopedLock lock(cs);
	elements.add(pt);
}

void AmbiSpeakerSet::remove(int index)
{
	const ScopedLock lock(cs);
	AmbiSpeaker* removedPt = elements.removeAndReturn(index);

	if (removedPt != nullptr)
		removedElements.add(removedPt);
}

void AmbiSpeakerSet::swap(int a, int b)
{
	if (elements.size() > a && elements.size() > b)
		elements.swap(a, b);
}


void AmbiSpeakerSet::cleanup(int keepNbOfElements)
{
	const ScopedLock lock(cs);

	while (removedElements.size() > keepNbOfElements)
	{
		AmbiSpeaker* pt = removedElements.removeAndReturn(0);
		if (pt != nullptr)
			delete pt;
	}
}


void AmbiSpeakerSet::setFilterBypass(int channel, bool byPass) const
{
    AmbiSpeaker* pt = elements[channel];

    if (pt != nullptr)
        pt->setFilterBypass(byPass);
}

double AmbiSpeakerSet::getMaxNormalizedDistance() const
{
	double maxDist = 0.0;
	for (AmbiSpeaker* pt : elements)
	{
		if (pt != nullptr)
			maxDist = jmax(maxDist, pt->getRawPoint()->getDistance());
	}

	return maxDist;
}

void AmbiSpeakerSet::addNew(String id, Point3D<double> point, String name, Colour color)
{
	elements.add(new AmbiSpeaker(id, point, name, color));
}

void AmbiSpeakerSet::loadFromXml(XmlElement *xmlElement)
{
    clear();
    
    XmlElement* xmlSources = xmlElement->getChildByName(XML_TAG_PRESET_POINTS);
    if(xmlSources != nullptr)
    {
        XmlElement* xmlPoint = xmlSources->getChildByName(XML_TAG_PRESET_POINT);
        while (xmlPoint != nullptr)
        {
            elements.add(new AmbiSpeaker(xmlPoint));
            xmlPoint = xmlPoint->getNextElement();
        }
    }
    
    // groups
    groups.clear();
    XmlElement* groupsElement = xmlElement->getChildByName(XML_TAG_GROUPS);
    if(groupsElement != nullptr)
    {
        // create base type pointer array
        Array<AmbiPoint*> ambiPointPointers;
        for(int i = 0; i < elements.size(); i++)
            ambiPointPointers.add(elements[i]);
        
        int index = 0;
        XmlElement* xmlGroup = groupsElement->getChildByName(XML_TAG_GROUP);
        while (xmlGroup != nullptr)
        {
            groups.add(new AmbiGroup(xmlGroup, &ambiPointPointers, AudioParameterSet(), pScalingInfo));
            
            xmlGroup = xmlGroup->getNextElement();
            index++;
        }
    }
}

void AmbiSpeakerSet::writeToXmlElement(XmlElement *xml) const
{
    XmlElement* xmlSpeakers = new XmlElement(XML_TAG_PRESET_POINTS);
    for (AmbiPoint* pt : elements)
    {
        xmlSpeakers->addChildElement(pt->getAsXmlElement(XML_TAG_PRESET_POINT));
    }
    xml->addChildElement(xmlSpeakers);
    
    // groups
    XmlElement* groupsElement = new XmlElement(XML_TAG_GROUPS);
    for(int i = 0; i < groupCount(); i++)
    {
        AmbiGroup* g = getGroup(i);
        if (g != nullptr)
            groupsElement->addChildElement(g->getAsXmlElement(XML_TAG_GROUP));
    }

    xml->addChildElement(groupsElement);
}
