/*
  ==============================================================================

    AmbiSpeakerSet.cpp
    Created: 30 Oct 2019 12:36:17pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiSpeakerSet.h"

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


void AmbiSpeakerSet::setFilterInfo(int channel, FilterInfo info) const
{
	AmbiSpeaker* pt = elements[channel];

	if (pt != nullptr)
		pt->getFilterInfo()->copyFrom(&info);
}


double AmbiSpeakerSet::getMaxNormalizedDistance() const
{
	double maxDist = 0.0;
	for (AmbiSpeaker* pt : elements)
	{
		if (pt != nullptr)
			maxDist = jmax(maxDist, pt->getPoint()->getDistance());
	}

	return maxDist;
}

void AmbiSpeakerSet::addNew(String id, Point3D<double> point, String name, Colour color)
{
	elements.add(new AmbiSpeaker(id, point, name, color));
}
