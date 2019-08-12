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
		if(*pAmbiPoint->getPoint() != point)
		{
			point = *pAmbiPoint->getPoint();
			changed = true;
		}
		return changed;
	}

	String name; 
	Point3D<double> point;
};

class AmbiOSCSenderExt : public Timer
{
public:
	AmbiOSCSenderExt(AmbiDataSet* ambiPoints);
	~AmbiOSCSenderExt();

	bool start(String targetHost, int port);
	void stop();

private:
	void timerCallback() override;

private:

	AmbiDataSet* pPoints;
	OwnedArray<PointHistoryEntry> history;
	ScopedPointer<OSCSender> oscSender;
};
