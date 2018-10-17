/*
  ==============================================================================

    AmbiDataSet.cpp
    Created: 12 Oct 2018 9:35:22pm
    Author:  chris

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
	elements.clear();
}

void AmbiDataSet::add(AmbiPoint* pt)
{
	elements.add(pt);
}

void AmbiDataSet::remove(int index)
{
	const ScopedLock lock(cs);
	if (elements.size() > index)
		elements.remove(index);
}

void AmbiDataSet::remove(String id)
{
	const ScopedLock lock(cs); 
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->getId() == id)
		{
			elements.remove(i);
			break;
		}
	}
}

void AmbiDataSet::swap(int a, int b)
{
	if (elements.size() > a && elements.size() > b)
		elements.swap(a, b);
}

AmbiPoint* AmbiDataSet::get(const int index, bool copyImage) const
{
	if (copyImage)
	{
		const ScopedLock lock(cs);
		AmbiPoint* pt = elements[index];
		if (pt != nullptr)
			return new AmbiPoint(pt, true);
	}
	else
	{
		AmbiPoint* pt = elements[index];
		if (pt != nullptr)
			return new AmbiPoint(pt);
	}

	return nullptr;
}

bool AmbiDataSet::setChannelXYZ(int channel, double x, double y, double z) const
{
	if (elements.size() >= channel && channel > 0)
	{
		AmbiPoint* ambiPt = elements.getUnchecked(channel - 1); // 1-based in message, 0-based in plugin
		ambiPt->getPoint()->setXYZ(x, y, z);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelAED(int channel, double a, double e, double d) const
{
	if (elements.size() >= channel && channel > 0)
	{
		AmbiPoint* ambiPt = elements.getUnchecked(channel - 1); // 1-based in message, 0-based in plugin
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth(a);
		ambiPt->getPoint()->setElevation(e);
		return true;
	}

	return false;
}

void AmbiDataSet::setChannelXYZExt(String id, String name, double x, double y, double z, float rms, Colour color)
{
	int index = -1;
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements.getUnchecked(i)->getId() == id)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		elements.add(new AmbiPoint(id, Point3D<double>(0.0, 0.0, 0.0), name, color));
		index = elements.size() - 1;
	}

	AmbiPoint* ambiPt = elements.getUnchecked(index);
	ambiPt->setName(name);
	ambiPt->getPoint()->setXYZ(x, y, z);
	ambiPt->setRms(rms);
	ambiPt->setColor(color);
	ambiPt->setAlive(Time::currentTimeMillis());
}

bool AmbiDataSet::setChannelNameAED(String channelName, double a, double e, double d) const
{
	AmbiPoint* ambiPt = nullptr;

	for (int i = 0; i < elements.size(); i++)
	{
		if ((elements.getUnchecked(i))->getName() == channelName)
		{
			ambiPt = elements.getUnchecked(i);
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
	CHANNELCHECK;

	elements[channel]->getPoint()->setXY(x, y);
}

void AmbiDataSet::setChannelYZ(int channel, double y, double z) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setYZ(y, z);
}

void AmbiDataSet::setChannelName(int channel, String name) const
{
	CHANNELCHECK;

	elements[channel]->setName(name);
}

void AmbiDataSet::setChannelColor(int channel, Colour colour) const
{
	CHANNELCHECK;

	elements[channel]->setColor(colour);
}

void AmbiDataSet::setX(int channel, double x, bool notify) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setX(x, notify);
}

void AmbiDataSet::setY(int channel, double y, bool notify) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setY(y, notify);
}

void AmbiDataSet::setZ(int channel, double z, bool notify) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setZ(z, notify);
}

void AmbiDataSet::setAzimuth(int channel, double azimuth) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setAzimuth(azimuth);
}

void AmbiDataSet::setElevation(int channel, double elevation) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setElevation(elevation);
}

void AmbiDataSet::setDistance(int channel, double distance) const
{
	CHANNELCHECK;

	elements[channel]->getPoint()->setDistance(distance);
}

void AmbiDataSet::setGain(int channel, double gain) const
{
	CHANNELCHECK;

	elements[channel]->setGain(gain);
}

double AmbiDataSet::getMaxNormalizedDistance() const
{
	double maxDist = 0.0;
	for (int i = 0; i < elements.size(); i++)
	{
		maxDist = jmax(maxDist, elements[i]->getPoint()->getDistance());
	}

	return maxDist;
}

void AmbiDataSet::setRms(int channel, float rms, bool onlyIfGreater) const
{
	CHANNELCHECK;

	elements[channel]->setRms(rms, onlyIfGreater);
}
