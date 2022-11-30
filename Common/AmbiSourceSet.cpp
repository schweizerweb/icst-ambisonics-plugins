/*
  ==============================================================================

    AmbiSourceSet.cpp
    Created: 30 Oct 2019 12:36:29pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiSourceSet.h"

AmbiSource* AmbiSourceSet::get(int index) const
{
	return elements[index];
}

int AmbiSourceSet::size() const
{
	return elements.size();
}

void AmbiSourceSet::clear()
{
	const ScopedLock lock(cs);
	while (elements.size() > 0)
	{
		remove(0);
	}
}

void AmbiSourceSet::add(AmbiSource* pt)
{
	const ScopedLock lock(cs);
	elements.add(pt);
}

void AmbiSourceSet::remove(int index)
{
	const ScopedLock lock(cs);

	// rearrange audio parameter
	for (int i = elements.size() - 1; i > index; i--)
	{
		elements[i]->getPoint()->setAudioParameterSet(elements[i-1]->getPoint()->getAudioParameterSet());
	}
	
	AmbiSource* removedPt = elements.removeAndReturn(index);

	if (removedPt != nullptr)
		removedElements.add(removedPt);
    
    cleanup(100);
}

void AmbiSourceSet::swap(int a, int b)
{
	if (elements.size() > a && elements.size() > b)
	{
		// keep audio parameter for index
		Point3D<double>* pointA = elements.getUnchecked(a)->getPoint();
		Point3D<double>* pointB = elements.getUnchecked(b)->getPoint();
		
		AudioParameterSet setA = pointA->getAudioParameterSet();
		pointA->setAudioParameterSet(pointB->getAudioParameterSet());
		pointB->setAudioParameterSet(setA);
		elements.swap(a, b);
	}
}


void AmbiSourceSet::cleanup(int keepNbOfElements)
{
	const ScopedLock lock(cs);

	while (removedElements.size() > keepNbOfElements)
	{
		AmbiSource* pt = removedElements.removeAndReturn(0);
		if (pt != nullptr)
			delete pt;
	}
}

void AmbiSourceSet::setChannelXYZExt(String id, String name, double x, double y, double z, float rms, Colour color)
{
    const ScopedLock lock(cs);

	AmbiSource* matchingPt = nullptr;

	for (AmbiSource* pt : elements)
	{
		if (pt != nullptr && pt->getId() == id)
		{
			matchingPt = pt;
			break;
		}
	}

	if (matchingPt == nullptr)
	{
		// try to reactivate deleted point
		for (AmbiSource* pt : removedElements)
		{
			if (pt != nullptr && pt->getId() == id)
			{
				matchingPt = pt;
				break;
			}
		}
		if (matchingPt != nullptr)
		{
			elements.add(matchingPt);
			removedElements.removeObject(matchingPt, false);
		}
		else
			matchingPt = elements.add(new AmbiSource(id, Point3D<double>(0.0, 0.0, 0.0), name, color));
	}

	matchingPt->setName(name);
	matchingPt->getPoint()->setXYZ(x, y, z);
	matchingPt->setRms(rms);
	matchingPt->setColor(color);
	matchingPt->setAlive(Time::currentTimeMillis());
}


void AmbiSourceSet::setRms(int channel, float rms, bool onlyIfGreater) const
{
	AmbiSource* pt = elements[channel];

	if (pt != nullptr)
		pt->setRms(rms, onlyIfGreater);
}

void AmbiSourceSet::addNew(String id, Point3D<double> point, String name, Colour color)
{
	elements.add(new AmbiSource(id, point, name, color));
}

void AmbiSourceSet::loadFromXml(XmlElement* xmlElement, AudioParams* pAudioParams)
{
	XmlElement* sourcesElement = xmlElement->getChildByName(XML_TAG_SOURCES);
	clear();
	if (sourcesElement != nullptr)
	{
		int index = 0;
		XmlElement* xmlPoint = sourcesElement->getChildByName(XML_TAG_SOURCE);
		while (xmlPoint != nullptr)
		{
			if (pAudioParams != nullptr && pAudioParams->sourceParams.size() > index)
				add(new AmbiSource(xmlPoint, pAudioParams->sourceParams.getUnchecked(index)));
			else
				add(new AmbiSource(xmlPoint, AudioParameterSet()));

			xmlPoint = xmlPoint->getNextElement();
			index++;
		}
        if(pAudioParams != nullptr)
        {
            while (elements.size() < pAudioParams->sourceParams.size())
            {
				String indexString = String(index + 1);
                add(new AmbiSource(pAudioParams->sourceParams.getUnchecked(index), indexString));
                index++;
            }
        }
	}

	// groups
	XmlElement* groupsElement = xmlElement->getChildByName(XML_TAG_GROUPS);
	groups.clear();
	if(groupsElement != nullptr)
	{
		int index = 0;
		XmlElement* xmlGroup = groupsElement->getChildByName(XML_TAG_GROUP);
		while (xmlGroup != nullptr)
		{
            if(pAudioParams != nullptr && pAudioParams->groupParams.size() > index)
                groups.add(new AmbiGroup(xmlGroup, &elements, pAudioParams->groupParams.getUnchecked(index), pScalingInfo));
            else
                groups.add(new AmbiGroup(xmlGroup, &elements, AudioParameterSet(), pScalingInfo));
            
			xmlGroup = xmlGroup->getNextElement();
			index++;
		}
	}
}

void AmbiSourceSet::writeToXmlElement(XmlElement* xml) const
{
	// load sources
	XmlElement* sourcesElement = new XmlElement(XML_TAG_SOURCES);
	for (int i = 0; i < size(); i++)
	{
		AmbiPoint* pt = get(i);
		if (pt != nullptr)
			sourcesElement->addChildElement(pt->getAsXmlElement(XML_TAG_SOURCE));
	}

	xml->addChildElement(sourcesElement);

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

void AmbiSourceSet::resetIds()
{
    // set new IDs
    for (int i = 0; i < size(); i++)
        get(i)->resetId();
    for (AmbiGroup* g : groups)
        g->resetId();
}


bool AmbiSourceSet::anySolo() const
{
    for(auto& p : elements)
        if(p->getSolo())
            return true;
    
    return false;
}
