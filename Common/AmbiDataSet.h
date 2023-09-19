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



#pragma once
#include "AmbiPoint.h"
#include "AmbiGroup.h"

#define DEFAULT_GROUP_MODE_FLAG     false
#define XML_TAG_GROUPS "Groups"
#define XML_TAG_GROUP "Group"

class AmbiDataSet
{
public:
	AmbiDataSet(ScalingInfo* pScaling);
	virtual ~AmbiDataSet();

	virtual int size() const = 0;
	virtual void clear() = 0;
	virtual void cleanup(int keepNbOfElements = 0) = 0;
	virtual void remove(int index) = 0;
	virtual void swap(int a, int b) = 0;
	virtual AmbiPoint* get(int index) const = 0;
	virtual void addNew(String id, Point3D<double> point, String name, Colour color) = 0;

	AmbiPoint* get(int index, int64 referenceTime, int timeoutMs);
	bool setChannelXYZ(int channel, double x, double y, double z) const;
	bool setChannelAED(int channel, double a, double e, double d) const;
	AmbiPoint* getPointByName(String string) const;
	bool setChannelNameAED(String channelName, double a, double e, double d) const;
	bool setChannelNameXYZ(String channelName, double x, double y, double z) const;
    bool setChannelNameGain(String channelName, double gain) const;
	bool setChannelXY(int channel, double x, double y) const;
	bool setChannelYZ(int channel, double y, double z) const;
	void setChannelName(int channel, String name) const;
	void setChannelColor(int channel, Colour colour) const;
    void setEnabled(int channel, bool enable) const;
	void setX(int channel, double x, bool notify = true) const;
	void setY(int channel, double y, bool notify = true) const;
	void setZ(int channel, double z, bool notify = true) const;
    void setGroupX(int channel, double x, bool notify = true) const;
    void setGroupY(int channel, double y, bool notify = true) const;
    void setGroupZ(int channel, double z, bool notify = true) const;
	void setAzimuth(int channel, double azimuth) const;
	void setElevation(int channel, double elevation) const;
	void setDistance(int channel, double distance) const;
	bool setGain(int channel, double gain, bool notify = true) const;
    bool setMute(int channel, bool mute, bool notify = true) const;
	bool setSolo(int channel, bool solo) const;
    String getNewUniqueName() const;
	int getEnabledCount() const;
    float getMaxDistance() const;
    
	int groupCount() const;
    int activeGroupCount() const;
	AmbiGroup* getGroup(int index) const;
    AmbiGroup* getActiveGroup(int index, int* pRealIndex = nullptr) const;
	int addGroup(String id, Vector3D<double> point, String name, Colour color);
	void moveGroupXyz(int groupIndex, double dx, double dy, double dz, bool moveSubElements) const;
	void removeGroup(int groupIndex);
    void addPointToGroup(int groupIndex, int pointIndex);
    void removePointFromGroup(int groupIndex, int pointIndex);
	bool setGroupXyz(int groupIndex, double newX, double newY, double newZ, bool moveSubElements) const;
	bool setGroupAed(int groupIndex, double a, double e, double d, bool moveSubElements) const;
	void setGroupName(int groupIndex, String name) const;
	bool stretchGroup(int groupIndex, double stretchValue);
    bool rotateGroup(int groupIndex, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis);
    bool rotateGroupAroundOrigin(int groupIndex, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements);
    
    bool setGroupAed(String groupName, double a, double e, double d, bool moveSubElements);
	bool setGroupXyz(String groupName, double x, double y, double z, bool moveSubElements) const;
    bool stretchGroup(String groupName, double stretchValue);
    bool rotateGroup(String groupName, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis);
    bool rotateGroupAroundOrigin(String groupName, double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements);
    void swapGroup(int a, int b);
    
    bool setGroupRotation(int groupIndex, Quaternion<double> rotation, bool notify = true);
    bool setGroupStretch(int groupIndex, double stretchFactor, bool notify = true);
    bool setGroupRotation(String groupName, Quaternion<double> rotation, bool notify = true);
    bool setGroupStretch(String groupName, double stretchFactor, bool notify = true);
    
    Vector3D<double> getAbsSourcePoint(int index) const;
    void setAbsSourcePoint(int index, Vector3D<double> absPoint);
    
    bool getGroupModeFlag() const;
    void setGroupModeFlag(bool en);
    
private:
	bool nameExists(String name) const;

protected:
	CriticalSection cs;
	OwnedArray<AmbiGroup> groups;
    ScalingInfo* pScalingInfo;
    
    bool groupModeFlag;
};
