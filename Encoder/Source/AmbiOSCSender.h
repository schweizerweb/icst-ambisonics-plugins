/*
  ==============================================================================

    AmbiOSCSender.h
    Created: 28 Dec 2017 1:09:23pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiSourceSet.h"

class AmbiOSCSender : public Timer
{
public:
	AmbiOSCSender(AmbiSourceSet* ambiPoints);
	~AmbiOSCSender() override;

	bool start(String targetHost, int port, int intervalMs);
	void stop();

private:
	void timerCallback() override;

private:
	AmbiSourceSet* pPoints;
	OSCSender* oscSender;
	int sendIntervalMs;
};
