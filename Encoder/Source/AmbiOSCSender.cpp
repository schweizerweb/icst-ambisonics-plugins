/*
  ==============================================================================

    AmbiOSCSender.cpp
    Created: 28 Dec 2017 1:09:23pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiOSCSender.h"
#include "../../Common/OSCHandler.h"

AmbiOSCSender::AmbiOSCSender(OwnedArray<AmbiPoint>* ambiPoints): pPoints(ambiPoints), sendIntervalMs(50)
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
			OSCArgument(pPoints->getUnchecked(iPoint)->getId()),
			OSCArgument(pPoints->getUnchecked(iPoint)->getName()),
			OSCArgument(float(pPoints->getUnchecked(iPoint)->getPoint()->getAzimuth())),
			OSCArgument(float(pPoints->getUnchecked(iPoint)->getPoint()->getElevation())),
			OSCArgument(float(pPoints->getUnchecked(iPoint)->getPoint()->getDistance())),
			OSCArgument(pPoints->getUnchecked(iPoint)->getRms(true)),
			OSCArgument(int32(pPoints->getUnchecked(iPoint)->getColor().getARGB())));
		oscSender->send(message);
	}

	startTimer(sendIntervalMs);
}
