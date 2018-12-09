/*
  ==============================================================================

    AmbiDataSet.cpp
    Created: 12 Oct 2018 9:35:22pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiDataSet.h"

AmbiDataSet::AmbiDataSet()
{
}

AmbiDataSet::~AmbiDataSet()
{
}

int AmbiDataSet::size() const
{
	return elements.size();
}

void AmbiDataSet::clear()
{
	const ScopedLock lock(cs);
	while(elements.size() > 0)
	{
		remove(0);
	}
}

void AmbiDataSet::add(AmbiPoint* pt)
{
	const ScopedLock lock(cs);
	elements.add(pt);
}

void AmbiDataSet::remove(int index)
{
	const ScopedLock lock(cs);
	AmbiPoint* removedPt = elements.removeAndReturn(index);

	if (removedPt != nullptr)
		removedElements.add(removedPt);
}

void AmbiDataSet::swap(int a, int b)
{
	if (elements.size() > a && elements.size() > b)
		elements.swap(a, b);
}


AmbiPoint* AmbiDataSet::get(const int index) const
{
	return elements[index];
}

AmbiPoint* AmbiDataSet::get(int index, int64 referenceTime, int timeoutMs)
{
	const ScopedLock lock(cs);
	AmbiPoint* pt = elements[index];
	if (pt != nullptr)
	{
		if (pt->checkAlive(referenceTime, timeoutMs))
			return pt;

		remove(index);
	}

	return nullptr;
}

bool AmbiDataSet::setChannelXYZ(int channel, double x, double y, double z) const
{
	AmbiPoint* ambiPt = elements[channel];
	if(ambiPt != nullptr)
	{
		ambiPt->getPoint()->setXYZ(x, y, z);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelAED(int channel, double a, double e, double d) const
{
	AmbiPoint* ambiPt = elements[channel];
	if(ambiPt != nullptr)
	{
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth(a);
		ambiPt->getPoint()->setElevation(e);
		return true;
	}

	return false;
}

void AmbiDataSet::setChannelXYZExt(String id, String name, double x, double y, double z, float rms, Colour color)
{
	AmbiPoint* matchingPt = nullptr;

	for (AmbiPoint* pt : elements)
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
		for (AmbiPoint* pt : removedElements)
		{
			if (pt != nullptr && pt->getId() == id)
			{
				matchingPt = pt;
				break;
			}
		}
		if(matchingPt != nullptr)
		{
			elements.add(matchingPt);
			removedElements.removeObject(matchingPt, false);
		}
		else
			matchingPt = elements.add(new AmbiPoint(id, Point3D<double>(0.0, 0.0, 0.0), name, color));
	}

	matchingPt->setName(name);
	matchingPt->getPoint()->setXYZ(x, y, z);
	matchingPt->setRms(rms);
	matchingPt->setColor(color);
	matchingPt->setAlive(Time::currentTimeMillis());
}

bool AmbiDataSet::setChannelNameAED(String channelName, double a, double e, double d) const
{
	AmbiPoint* ambiPt = nullptr;

	for (AmbiPoint* pt : elements)
	{
		if (pt != nullptr && pt->getName() == channelName)
		{
			ambiPt = pt;
			break;
		}
	}

	if (ambiPt != nullptr)
	{
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth(a);
		ambiPt->getPoint()->setElevation(e);
		return true;
	}

	return false;
}

void AmbiDataSet::setChannelXY(int channel, double x, double y) const
{
	AmbiPoint* pt = elements[channel];
	
	if(pt != nullptr)
		pt->getPoint()->setXY(x, y);
}

void AmbiDataSet::setChannelYZ(int channel, double y, double z) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setYZ(y, z);
}

void AmbiDataSet::setChannelName(int channel, String name) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->setName(name);
}

void AmbiDataSet::setChannelColor(int channel, Colour colour) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->setColor(colour);
}

void AmbiDataSet::setX(int channel, double x, bool notify) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setX(x, notify);
}

void AmbiDataSet::setY(int channel, double y, bool notify) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setY(y, notify);
}

void AmbiDataSet::setZ(int channel, double z, bool notify) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setZ(z, notify);
}

void AmbiDataSet::setAzimuth(int channel, double azimuth) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setAzimuth(azimuth);
}

void AmbiDataSet::setElevation(int channel, double elevation) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setElevation(elevation);
}

void AmbiDataSet::setDistance(int channel, double distance) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->getPoint()->setDistance(distance);
}

void AmbiDataSet::setGain(int channel, double gain) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->setGain(gain);
}

double AmbiDataSet::getMaxNormalizedDistance() const
{
	double maxDist = 0.0;
	for (AmbiPoint* pt : elements)
	{
		if(pt != nullptr)
			maxDist = jmax(maxDist, pt->getPoint()->getDistance());
	}

	return maxDist;
}

void AmbiDataSet::cleanup(int keepNbOfElements)
{
	const ScopedLock lock(cs);

	while (removedElements.size() > keepNbOfElements)
	{
		AmbiPoint* pt = removedElements.removeAndReturn(0);
		if (pt != nullptr)
			delete pt;
	}
}

void AmbiDataSet::setRms(int channel, float rms, bool onlyIfGreater) const
{
	AmbiPoint* pt = elements[channel];

	if (pt != nullptr)
		pt->setRms(rms, onlyIfGreater);
}
