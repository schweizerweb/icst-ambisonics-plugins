/*
  ==============================================================================

    AmbiDataSet.h
    Created: 12 Oct 2018 9:35:22pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"

#define CHANNELCHECK if (channel < 0 || channel >= elements.size()) return

class AmbiDataSet
{
public:
	AmbiDataSet();
	~AmbiDataSet();

	int size() const;
	void clear();
	void add(AmbiPoint* pt);
	void remove(int index);
	void remove(String id);
	void swap(int a, int b);
	AmbiPoint* get(const int index, bool copyImage = false) const;
	
	bool setChannelXYZ(int channel, double x, double y, double z) const;
	bool setChannelAED(int channel, double a, double e, double d) const;
	void setChannelXYZExt(String id, String name, double x, double y, double z, float rms, Colour color);
	bool setChannelNameAED(String channelName, double a, double e, double d) const;
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
	void setGain(int channel, double gain) const;
	void setRms(int channel, float rms, bool onlyIfGreater) const;
	double getMaxNormalizedDistance() const;

private:
	OwnedArray<AmbiPoint> elements;
	CriticalSection cs;
};
