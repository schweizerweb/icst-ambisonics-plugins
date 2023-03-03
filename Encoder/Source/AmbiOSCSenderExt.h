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

class PointHistoryEntry
{
public:
	bool update(Vector3D<double> absPos, AmbiPoint* pAmbiPoint)
	{
		bool changed = false;

		if(pAmbiPoint->getName() != name)
		{
			name = pAmbiPoint->getName();
			changed = true;
		}
		if(!(absPos - point).lengthIsBelowEpsilon())
		{
			point = absPos;
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
	Vector3D<double> point;
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
    int connectStandardSender(int* pIndex, StandardOscTarget* pTarget, String oscString, String description, String* pMessage);
    
private:
	CriticalSection cs;
	AmbiDataSet* pPoints;
	StatusMessageHandler* pStatusMessageHandler;
	OwnedArray<PointHistoryEntry> history;
	OwnedArray<OSCSenderInstance> oscSender;
    ScalingInfo* pScalingInfo;
    bool doContinuousUpdate;
};
