/*
  ==============================================================================

    AmbiDataSet.h
    Created: 12 Oct 2018 9:35:22pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"
#include "AmbiGroup.h"

class AmbiDataSet
{
public:
	AmbiDataSet();
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
	void setChannelXY(int channel, double x, double y) const;
	void setChannelYZ(int channel, double y, double z) const;
	void setChannelName(int channel, String name) const;
	void setChannelColor(int channel, Colour colour) const;
	void setX(int channel, double x, bool notify = true) const;
	void setY(int channel, double y, bool notify = true) const;
	void setZ(int channel, double z, bool notify = true) const;
	void setAzimuth(int channel, double azimuth) const;
	void setElevation(int channel, double elevation) const;
	void setDistance(int channel, double distance) const;
	void setGain(int channel, double gain, bool notify = true) const;
	String getNewUniqueName() const;

	int groupCount() const;
	AmbiGroup* getGroup(int index) const;
	AmbiGroup* addGroup(String id, Point3D<double> point, String name, Colour color);
	void moveGroupXyz(int groupIndex, double dx, double dy, double dz, bool moveSubElements) const;
	void removeGroup(int groupIndex);
	void setGroupXyz(int groupIndex, double newX, double newY, double newZ, bool moveSubElements) const;
	void setGroupAed(int groupIndex, double a, double e, double d, bool moveSubElements) const;
	void setGroupName(int groupIndex, String name) const;
	void stretchGroup(int groupIndex, double stretchValue);
    void rotateGroup(int groupIndex, double angleRad);
    
    bool setGroupAed(String groupName, double a, double e, double d, bool moveSubElements);
	bool setGroupXyz(String groupName, double x, double y, double z, bool moveSubElements) const;
    bool stretchGroup(String groupName, double stretchValue);
    bool rotateGroup(String groupName, double angleRad);

private:
	bool nameExists(String name) const;

protected:
	CriticalSection cs;
	OwnedArray<AmbiGroup> groups;
};
