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
	AmbiSource* removedPt = elements.removeAndReturn(index);

	if (removedPt != nullptr)
		removedElements.add(removedPt);
}

void AmbiSourceSet::swap(int a, int b)
{
	if (elements.size() > a && elements.size() > b)
		elements.swap(a, b);
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
