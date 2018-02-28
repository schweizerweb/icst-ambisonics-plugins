/*
  ==============================================================================

    AmbiOSCSender.h
    Created: 28 Dec 2017 1:09:23pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"

class AmbiOSCSender : public Timer
{
public:
	AmbiOSCSender(OwnedArray<AmbiPoint>* ambiPoints);
	~AmbiOSCSender();

	bool start(String targetHost, int port, int intervalMs);
	void stop();

private:
	void timerCallback() override;

private:
	OwnedArray<AmbiPoint>* pPoints;
	ScopedPointer<OSCSender> oscSender;
	int sendIntervalMs;
};
