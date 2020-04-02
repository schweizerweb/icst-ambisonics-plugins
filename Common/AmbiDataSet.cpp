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

AmbiPoint* AmbiDataSet::get(int index, int64 referenceTime, int timeoutMs)
{
	const ScopedLock lock(cs);
	AmbiPoint* pt = get(index);
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
	AmbiPoint* ambiPt = get(channel);
	if(ambiPt != nullptr)
	{
		ambiPt->getPoint()->setXYZ(x, y, z);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelAED(int channel, double a, double e, double d) const
{
	AmbiPoint* ambiPt = get(channel);
	if(ambiPt != nullptr)
	{
        ambiPt->getPoint()->setAed(a, e, d);
		return true;
	}

	return false;
}

AmbiPoint* AmbiDataSet::getPointByName(String channelName) const
{
	AmbiPoint* ambiPt = nullptr;

	for (int i = 0; i < size(); i++)
	{
		AmbiPoint* pt = get(i);
		if (pt != nullptr && pt->getName() == channelName)
		{
			ambiPt = pt;
			break;
		}
	}

	return ambiPt;
}

bool AmbiDataSet::setChannelNameAED(String channelName, double a, double e, double d) const
{
	AmbiPoint* ambiPt = getPointByName(channelName);

	if (ambiPt != nullptr)
	{
        ambiPt->getPoint()->setAed(a, e, d);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelNameXYZ(String channelName, double x, double y, double z) const
{
	AmbiPoint* ambiPt = getPointByName(channelName);

	if (ambiPt != nullptr)
	{
		ambiPt->getPoint()->setXYZ(x, y, z);
		return true;
	}

	return false;
}

void AmbiDataSet::setChannelXY(int channel, double x, double y) const
{
	AmbiPoint* pt = get(channel);
	
	if(pt != nullptr)
		pt->getPoint()->setXY(x, y);
}

void AmbiDataSet::setChannelYZ(int channel, double y, double z) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setYZ(y, z);
}

void AmbiDataSet::setChannelName(int channel, String name) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->setName(name);
}

void AmbiDataSet::setChannelColor(int channel, Colour colour) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->setColor(colour);
}

void AmbiDataSet::setX(int channel, double x, bool notify) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setX(x, notify);
}

void AmbiDataSet::setY(int channel, double y, bool notify) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setY(y, notify);
}

void AmbiDataSet::setZ(int channel, double z, bool notify) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setZ(z, notify);
}

void AmbiDataSet::setAzimuth(int channel, double azimuth) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setAzimuth(azimuth);
}

void AmbiDataSet::setElevation(int channel, double elevation) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setElevation(elevation);
}

void AmbiDataSet::setDistance(int channel, double distance) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->getPoint()->setDistance(distance);
}

void AmbiDataSet::setGain(int channel, double gain, bool notify) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->setGain(gain, notify);
}

String AmbiDataSet::getNewUniqueName() const
{
	String nameBase = String(size() + 1);
	String name = nameBase;
	int iTrial = 1;

	while (nameExists(name))
	{
		name = nameBase + "-" + String(iTrial++);
	}

	return name;
}

int AmbiDataSet::groupCount() const
{
	return groups.size();
}

AmbiGroup* AmbiDataSet::getGroup(int index) const
{
	return groups[index];
}

void AmbiDataSet::moveGroupXyz(int groupIndex, double dx, double dy, double dz, bool moveSubElements) const
{
	const ScopedLock lock(cs);

	AmbiGroup* group = groups[groupIndex];
	if (group != nullptr)
		group->moveXYZ(dx, dy, dz, moveSubElements);
}

void AmbiDataSet::removeGroup(int groupIndex)
{
	groups.remove(groupIndex);
}

void AmbiDataSet::setGroupXyz(int groupIndex, double newX, double newY, double newZ, bool moveSubElements) const
{
	const ScopedLock lock(cs);

	AmbiGroup* group = groups[groupIndex];
	if (group != nullptr)
		group->setXYZ(newX, newY, newZ, moveSubElements);
}

void AmbiDataSet::setGroupAed(int groupIndex, double newA, double newE, double newD, bool moveSubElements) const
{
	const ScopedLock lock(cs);

	AmbiGroup* group = groups[groupIndex];
	if (group != nullptr)
		group->setAED(newA, newE, newD, moveSubElements);
}

void AmbiDataSet::setGroupName(int groupIndex, String name) const
{
	const ScopedLock lock(cs);

	AmbiGroup* group = groups[groupIndex];
	if (group != nullptr)
		group->setName(name);
}

bool AmbiDataSet::setGroupAed(String groupName, double a, double e, double d, bool moveSubElements)
{
	bool found = false;
	for (int i = 0; i < groups.size(); i++)
	{
		if (getGroup(i)->getName() == groupName)
		{
			setGroupAed(i, a, e, d, moveSubElements);
			found = true;
		}
	}

	return found;
}

AmbiGroup* AmbiDataSet::addGroup(String id, Point3D<double> point, String name, Colour color)
{
	AmbiGroup* group = new AmbiGroup(id, point, name, color);
	groups.add(group);

	return group;
}

bool AmbiDataSet::setGroupXyz(String groupName, double x, double y, double z, bool moveSubElements) const
{
	bool found = false;
	for (int i = 0; i < groups.size(); i++)
	{
		if (getGroup(i)->getName() == groupName)
		{
			setGroupXyz(i, x, y, z, moveSubElements);
			found = true;
		}
	}

	return found;
}

bool AmbiDataSet::nameExists(String name) const
{
	for (int i = 0; i < size(); i++)
	{
		if (get(i)->getName() == name)
			return true;
	}

	return false;
}
