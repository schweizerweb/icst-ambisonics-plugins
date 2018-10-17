/*
  ==============================================================================

    AmbiOSCSender.h
    Created: 28 Dec 2017 1:09:23pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiDataSet.h"

class AmbiOSCSender : public Timer
{
public:
	AmbiOSCSender(AmbiDataSet* ambiPoints);
	~AmbiOSCSender();

	bool start(String targetHost, int port, int intervalMs);
	void stop();

private:
	void timerCallback() override;

private:
	AmbiDataSet* pPoints;
	ScopedPointer<OSCSender> oscSender;
	int sendIntervalMs;
};
