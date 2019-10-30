/*
  ==============================================================================

    AmbiOSCSender.cpp
    Created: 28 Dec 2017 1:09:23pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiOSCSender.h"
#include "../../Common/OSCHandler.h"

AmbiOSCSender::AmbiOSCSender(AmbiSourceSet* ambiPoints): pPoints(ambiPoints), sendIntervalMs(50)
{
	oscSender = new OSCSender();
}

AmbiOSCSender::~AmbiOSCSender()
{
	stop();
	delete oscSender;
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
	for (int i = 0; i < pPoints->size(); i++)
	{
		AmbiSource* pt = pPoints->get(i);
		if (pt != nullptr)
		{
			OSCMessage message = OSCMessage(
				OSCAddressPattern(OSC_ADDRESS_AMBISONIC_PLUGINS_INTERNAL),
				OSCArgument(pt->getId()),
				OSCArgument(pt->getName()),
				OSCArgument(float(pt->getPoint()->getX())),
				OSCArgument(float(pt->getPoint()->getY())),
				OSCArgument(float(pt->getPoint()->getZ())),
				OSCArgument(pt->getRms()),
				OSCArgument(int32(pt->getColor().getARGB())));
			oscSender->send(message);
			pPoints->setRms(i, 0.0f, false);
		}
	}

	startTimer(sendIntervalMs);
}
