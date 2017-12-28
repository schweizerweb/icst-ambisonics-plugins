/*
  ==============================================================================

    AmbiOSCSender.cpp
    Created: 28 Dec 2017 1:09:23pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiOSCSender.h"
#include "../../Common/OSCHandler.h"

AmbiOSCSender::AmbiOSCSender(Array<AmbiPoint>* ambiPoints): pPoints(ambiPoints), sendIntervalMs(50)
{
	oscSender = new OSCSender();
}

AmbiOSCSender::~AmbiOSCSender()
{
	stop();
	oscSender = nullptr;
}

bool AmbiOSCSender::start(String targetHost, int port, int intervalMs)
{
	stop();
	
	bool ret = oscSender->connect(targetHost, port);
	if (ret)
	{
		sendIntervalMs = intervalMs;
		startTimer(sendIntervalMs);
	}
	return ret;
}

void AmbiOSCSender::stop()
{
	stopTimer();
	oscSender->disconnect();
}

void AmbiOSCSender::timerCallback()
{
	stopTimer();
	for (int iPoint = 0; iPoint < pPoints->size(); iPoint++)
	{
		OSCMessage message = OSCMessage(
			OSCAddressPattern(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS),
			OSCArgument(pPoints->getReference(iPoint).getId()),
			OSCArgument(pPoints->getReference(iPoint).getName()),
			OSCArgument(float(pPoints->getReference(iPoint).getPoint()->getAzimuth())),
			OSCArgument(float(pPoints->getReference(iPoint).getPoint()->getElevation())),
			OSCArgument(float(pPoints->getReference(iPoint).getPoint()->getDistance())),
			OSCArgument(pPoints->getReference(iPoint).getRms(true)),
			OSCArgument(pPoints->getReference(iPoint).getColorIndex()));
		oscSender->send(message);
	}

	startTimer(sendIntervalMs);
}
