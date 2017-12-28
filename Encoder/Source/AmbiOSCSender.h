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
	AmbiOSCSender(Array<AmbiPoint>* ambiPoints);
	~AmbiOSCSender();

	bool start(String targetHost, int port, int intervalMs);
	void stop();

private:
	void timerCallback() override;

private:
	Array<AmbiPoint>* pPoints;
	ScopedPointer<OSCSender> oscSender;
	int sendIntervalMs;
};
