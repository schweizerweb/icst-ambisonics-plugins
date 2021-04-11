/*
  ==============================================================================

    AmbiOSCSenderExt.h
    Created: 2 Jul 2019 1:15:15pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiDataSet.h"
#include "EncoderSettings.h"
#include "OSCSenderInstance.h"
#include "../../Common/StatusMessageHandler.h"
#include "../../Common/ScalingInfo.h"

#define COMPARISON_DELTA 0.00001

class PointHistoryEntry
{
public:
	bool update(AmbiPoint* pAmbiPoint)
	{
		bool changed = false;

		if(pAmbiPoint->getName() != name)
		{
			name = pAmbiPoint->getName();
			changed = true;
		}
		if(!pAmbiPoint->getPoint()->equals(point, COMPARISON_DELTA))
		{
			point = *pAmbiPoint->getPoint();
			changed = true;
		}
		if(pAmbiPoint->getGain() != gain)
		{
			gain = pAmbiPoint->getGain();
			changed = true;
		}
		if(pAmbiPoint->getColor() != color)
		{
			color = pAmbiPoint->getColor();
			changed = true;
		}
		return changed;
	}

	String name; 
	Point3D<double> point;
	Colour color;
	double gain;
};

class AmbiOSCSenderExt : public Timer
{
public:
	AmbiOSCSenderExt(AmbiDataSet* ambiPoints, StatusMessageHandler* pStatusMessageHandler, ScalingInfo* pScaling);
    virtual ~AmbiOSCSenderExt();

    OSCSenderInstance* getOrCreateInstance(int index);
	bool start(EncoderSettings* pSettings, String* pMessage);
	void stop();

private:
	void timerCallback() override;

private:
	CriticalSection cs;
	AmbiDataSet* pPoints;
	StatusMessageHandler* pStatusMessageHandler;
	OwnedArray<PointHistoryEntry> history;
	OwnedArray<OSCSenderInstance> oscSender;
    ScalingInfo* pScalingInfo;
};
