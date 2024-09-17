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



#include "AmbiDataSet.h"

AmbiDataSet::AmbiDataSet(ScalingInfo* pScaling, bool _groupModeFlag) : pScalingInfo(pScaling), groupModeFlag(_groupModeFlag)
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
		ambiPt->getRawPoint()->setXYZ(x, y, z);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelAED(int channel, double a, double e, double d) const
{
	AmbiPoint* ambiPt = get(channel);
	if(ambiPt != nullptr)
	{
        ambiPt->getRawPoint()->setAed(a, e, d);
		return true;
	}

	return false;
}

float AmbiDataSet::getMaxDistance() const
{
    float maxDistance = 0.0f;
    for (int i = 0; i < size(); i++)
    {
        if(get(i) != nullptr)
        {
            Vector3D<double> pt = getAbsSourcePoint(i);
            maxDistance = jmax(maxDistance, (float)pt.length());
        }
    }
    
    return maxDistance;
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
        ambiPt->getRawPoint()->setAed(a, e, d);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelNameXYZ(String channelName, double x, double y, double z) const
{
	AmbiPoint* ambiPt = getPointByName(channelName);

	if (ambiPt != nullptr)
	{
		ambiPt->getRawPoint()->setXYZ(x, y, z);
		return true;
	}

	return false;
}

bool AmbiDataSet::setChannelNameGain(String channelName, double gain) const
{
    AmbiPoint* ambiPt = getPointByName(channelName);

    if (ambiPt != nullptr)
    {
        ambiPt->setGain(gain, true);
        return true;
    }

    return false;
}

bool AmbiDataSet::setChannelXY(int channel, double x, double y) const
{
	AmbiPoint* pt = get(channel);
	
	if(pt != nullptr)
    {
		pt->getRawPoint()->setXY(x, y);
        return true;
    }
    
    return false;
}

bool AmbiDataSet::setChannelYZ(int channel, double y, double z) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
    {
		pt->getRawPoint()->setYZ(y, z);
        return true;
    }
    
    return false;
}

bool AmbiDataSet::setChannelName(int channel, String name) const
{
	AmbiPoint* pt = get(channel);

    if (pt != nullptr)
    {
        pt->setName(name);
        return true;
    }

    return false;
}

void AmbiDataSet::setChannelColor(int channel, Colour colour) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
		pt->setColor(colour);
}

void AmbiDataSet::setEnabled(int channel, bool enable) const
{
    AmbiPoint* pt = get(channel);

    if (pt != nullptr)
    {
        pt->setEnabled(enable);
        if(!enable)
        {
            for(AmbiGroup* g : groups)
            {
                g->removePointFromGroup(pt);
            }
            
            // reset
            pt->setSolo(false);
            pt->setMute(false, true);
            pt->setGain(1.0, true);
            pt->getRawPoint()->setXYZ(0.0, 0.0, 0.0);
        }
    }
}

void AmbiDataSet::setX(int channel, double x, bool notify) const
{
	AmbiPoint* pt = get(channel);

    if(!notify)
    {
        x = pScalingInfo->decompress(x);
    }

	if (pt != nullptr)
	{
    	pt->getRawPoint()->setX(x, notify);
    }
}

void AmbiDataSet::setY(int channel, double y, bool notify) const
{
	AmbiPoint* pt = get(channel);

    if(!notify)
    {
        y = pScalingInfo->decompress(y);
    }

	if (pt != nullptr)
	{
    	pt->getRawPoint()->setY(y, notify);
    }
}

void AmbiDataSet::setZ(int channel, double z, bool notify) const
{
	AmbiPoint* pt = get(channel);

    if(!notify)
    {
        z = pScalingInfo->decompress(z);
    }

	if (pt != nullptr)
	{
    	pt->getRawPoint()->setZ(z, notify);
    }
}

void AmbiDataSet::setGroupX(int channel, double x, bool notify) const
{
    AmbiPoint* pt = getGroup(channel);

    if(!notify)
    {
        x = pScalingInfo->decompress(x);
    }

    if (pt != nullptr)
    {
        pt->getRawPoint()->setX(x, notify);
    }
}

void AmbiDataSet::setGroupY(int channel, double y, bool notify) const
{
    AmbiPoint* pt = getGroup(channel);

    if(!notify)
    {
        y = pScalingInfo->decompress(y);
    }
    
    if (pt != nullptr)
    {
        pt->getRawPoint()->setY(y, notify);
    }
}

void AmbiDataSet::setGroupZ(int channel, double z, bool notify) const
{
    AmbiPoint* pt = getGroup(channel);

    if(!notify)
    {
        z = pScalingInfo->decompress(z);
    }

    if (pt != nullptr)
    {
        pt->getRawPoint()->setZ(z, notify);
    }
}

void AmbiDataSet::setAzimuth(int channel, double azimuth) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
	{	
        pt->getRawPoint()->setAzimuth(azimuth);
    }
}

void AmbiDataSet::setElevation(int channel, double elevation) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
    {
		pt->getRawPoint()->setElevation(elevation);
    }
}

void AmbiDataSet::setDistance(int channel, double distance) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
    {
		pt->getRawPoint()->setDistance(distance);
    }
}

bool AmbiDataSet::setGain(int channel, double gain, bool notify) const
{
	AmbiPoint* pt = get(channel);

	if (pt != nullptr)
    {
        pt->setGain(gain, notify);
        return true;
    }
    
    return false;
}

bool AmbiDataSet::setMute(int channel, bool mute, bool notify) const
{
    AmbiPoint* pt = get(channel);

    if (pt != nullptr)
    {
        pt->setMute(mute, notify);
        return true;
    }
    
    return false;
}

bool AmbiDataSet::setSolo(int channel, bool solo) const
{
    AmbiPoint* pt = get(channel);

    if (pt != nullptr)
    {
        pt->setSolo(solo);
        return true;
    }
    
    return false;
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

int AmbiDataSet::getEnabledCount() const
{
	int count = 0;
	for(int i = 0; i < size(); i++)
	{
		if (get(i) != nullptr && get(i)->getEnabled())
        {
			count++;
        }
	}

	return count;
}

int AmbiDataSet::groupCount() const
{
	return groups.size();
}

int AmbiDataSet::activeGroupCount() const
{
    int cnt = 0;
    for(auto& g : groups)
    {
        if(g->getEnabled())
        {
            cnt++;
        }
    }
    
    return cnt;
}

AmbiGroup* AmbiDataSet::getGroup(int index) const
{
	return groups[index];
}

AmbiGroup* AmbiDataSet::getActiveGroup(int index, int* pRealIndex) const
{
    for(int i = 0; i < groups.size(); i++)
    {
        if(groups[i]->getEnabled())
        {
            if(index == 0)
            {
                if(pRealIndex != nullptr)
                {
                    *pRealIndex = i;
                }

                return groups[i];
            }
            
            index--;
        }
    }
    
    return nullptr;
}

void AmbiDataSet::moveGroupXyz(int groupIndex, double dx, double dy, double dz, bool moveSubElements) const
{
	const ScopedLock lock(cs);

	AmbiGroup* group = groups[groupIndex];
	if (group != nullptr)
    {
		group->moveXYZ(dx, dy, dz, moveSubElements, groupModeFlag);
    }
}

void AmbiDataSet::removeGroup(int groupIndex)
{
    const ScopedLock lock(cs);
    
    // keep absolute positions
    std::map<int, Vector3D<double>> origPos;
    for(int i = 0; i < size(); i++)
    {
        auto p = get(i);
        if(p != nullptr && p->getGroup() == groups[groupIndex])
        {
            origPos[i] = getAbsSourcePoint(i);
        }
    }
    
    groups[groupIndex]->removeAllPoints();
    groups[groupIndex]->setEnabled(false);
    
    for(auto& p : origPos)
    {
        setAbsSourcePoint(p.first, p.second);
    }
}

void AmbiDataSet::addPointToGroup(int groupIndex, int pointIndex)
{
    auto origPos = getAbsSourcePoint(pointIndex);
    getGroup(groupIndex)->addPointToGroup(get(pointIndex));
    setAbsSourcePoint(pointIndex, origPos);
}

void AmbiDataSet::removePointFromGroup(int groupIndex, int pointIndex)
{
    auto origPos = getAbsSourcePoint(pointIndex);
    getGroup(groupIndex)->removePointFromGroup(get(pointIndex));
    setAbsSourcePoint(pointIndex, origPos);
}

bool AmbiDataSet::setGroupXyz(int groupIndex, double newX, double newY, double newZ, bool moveSubElements) const
{
	const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->setXYZ(newX, newY, newZ, moveSubElements, groupModeFlag);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::setGroupAed(int groupIndex, double newA, double newE, double newD, bool moveSubElements) const
{
	const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->setAED(newA, newE, newD, moveSubElements, groupModeFlag);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::stretchGroup(int groupIndex, double stretchValue)
{
    const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->stretch(stretchValue, groupModeFlag);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::stretchGroup(String groupName, double stretchValue)
{
    bool found = false;
    for (int i = 0; i < groups.size(); i++)
    {
        if (getGroup(i)->getName() == groupName)
        {
            stretchGroup(i, stretchValue);
            found = true;
        }
    }

    return found;
}

bool AmbiDataSet::rotateGroup(int groupIndex, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis)
{
    const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->rotate(angleAroundXAxis, angleAroundYAxis, angleAroundZAxis, groupModeFlag);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::rotateGroup(String groupName, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis)
{
    bool found = false;
    for (int i = 0; i < groups.size(); i++)
    {
        if (getGroup(i)->getName() == groupName)
        {
            rotateGroup(i, angleAroundXAxis, angleAroundYAxis, angleAroundZAxis);
            found = true;
        }
    }

    return found;
}

bool AmbiDataSet::rotateGroupAroundOrigin(int groupIndex, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements)
{
    const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->rotateAroundOrigin(angleAroundXAxis, angleAroundYAxis, angleAroundZAxis, moveSubElements, groupModeFlag);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::rotateGroupAroundOrigin(String groupName, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements)
{
    bool found = false;
    for (int i = 0; i < groups.size(); i++)
    {
        if (getGroup(i)->getName() == groupName)
        {
            rotateGroupAroundOrigin(i, angleAroundXAxis, angleAroundYAxis, angleAroundZAxis, moveSubElements);
            found = true;
        }
    }

    return found;
}

void AmbiDataSet::setGroupName(int groupIndex, String name) const
{
	const ScopedLock lock(cs);

	AmbiGroup* group = groups[groupIndex];
	if (group != nullptr)
    {
		group->setName(name);
    }
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

int AmbiDataSet::addGroup(String id, Vector3D<double> point, String name, Colour color)
{
    // returns the index of the group
    int index = -1;
    
    // find existing disabled groups first
    for(int i = 0; i < groups.size(); i++)
    {
        if(!groups[i]->getEnabled())
        {
            index = i;
            groups[i]->setEnabled(true);
            groups[i]->setStretch(1.0);
            groups[i]->setRotation(Quaternion<double>(0.0, 0.0, 0.0, 1.0));
            break;
        }
    }
    
    // otherwise create a new one
    if(index == -1)
    {
        AmbiGroup* group = new AmbiGroup(id, Point3D<double>(point.x, point.y, point.z, AudioParameterSet()), name, color, pScalingInfo);
        groups.add(group);
        index = groups.size() - 1;
    }

    groups[index]->setXYZ(point.x, point.y, point.z, false, groupModeFlag);
    
	return index;
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
        {
			return true;
        }
	}

	return false;
}

void AmbiDataSet::swapGroup(int a, int b)
{
    if (groups.size() > a && groups.size() > b)
    {
        // keep audio parameter for index
        Point3D<double>* pointA = groups.getUnchecked(a)->getRawPoint();
        Point3D<double>* pointB = groups.getUnchecked(b)->getRawPoint();
        
        AudioParameterSet setA = pointA->getAudioParameterSet();
        pointA->setAudioParameterSet(pointB->getAudioParameterSet());
        pointB->setAudioParameterSet(setA);
        groups.swap(a, b);
    }
}

bool AmbiDataSet::setGroupRotation(int groupIndex, Quaternion<double> rotation, bool notify)
{
    const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->setRotation(rotation, notify);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::setGroupRotation(String groupName, Quaternion<double> rotation, bool notify)
{
    bool found = false;
    for (int i = 0; i < groups.size(); i++)
    {
        if (getGroup(i)->getName() == groupName)
        {
            setGroupRotation(i, rotation, notify);
            found = true;
        }
    }

    return found;}

bool AmbiDataSet::setGroupStretch(int groupIndex, double stretchFactor, bool notify)
{
    const ScopedLock lock(cs);

    if(groupIndex >= 0 && groupIndex < groupCount())
    {
        AmbiGroup* group = groups[groupIndex];
        if (group != nullptr)
        {
            group->setStretch(stretchFactor, notify);
            return true;
        }
    }
    
    return false;
}

bool AmbiDataSet::setGroupStretch(String groupName, double stretchFactor, bool notify)
{
    bool found = false;
    for (int i = 0; i < groups.size(); i++)
    {
        if (getGroup(i)->getName() == groupName)
        {
            setGroupStretch(i, stretchFactor, notify);
            found = true;
        }
    }

    return found;}

bool AmbiDataSet::getGroupModeFlag() const
{
    return groupModeFlag;
}

Vector3D<double> AmbiDataSet::getAbsSourcePoint(int index) const
{
    const ScopedLock lock(cs);

    if(groupModeFlag)
    {
        auto p = get(index);
        if(p==nullptr)
            return Vector3D<double>();
        Vector3D<double> pt = p->getVector3D();
        auto grpPoint = get(index)->getGroup();
        if(grpPoint != nullptr)
        {
            grpPoint->applyTransform(&pt);
            pt += grpPoint->getVector3D();
        }

        return pt;
    }
    else
    {
        return get(index)->getVector3D();
    }
}

void AmbiDataSet::setAbsSourcePoint(int index, Vector3D<double> absPoint)
{
    if(groupModeFlag)
    {
        auto grpPoint = get(index)->getGroup();
        if(grpPoint != nullptr)
        {
            absPoint -= grpPoint->getVector3D();
            grpPoint->applyInverseTransform(&absPoint);
        }
    }
    
    get(index)->getRawPoint()->setXYZ(absPoint.x, absPoint.y, absPoint.z);
}
