/*
  ==============================================================================

    AmbiOSCSenderExt.cpp
    Created: 2 Jul 2019 1:15:15pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiOSCSenderExt.h"
#include "../../Common/OSCHandler.h"

AmbiOSCSenderExt::AmbiOSCSenderExt(AmbiDataSet* ambiPoints): pPoints(ambiPoints)
{
	oscSender = new OSCSender();
}

AmbiOSCSenderExt::~AmbiOSCSenderExt()
{
	stop();
	oscSender = nullptr;
}

bool AmbiOSCSenderExt::start(String targetHost, int port)
{
	stop();
	
	bool ret = oscSender->connect(targetHost, port);
	if (ret)
	{
		startTimerHz(5);
	}
	return ret;
}

void AmbiOSCSenderExt::stop()
{
	stopTimer();
	oscSender->disconnect();
}

void AmbiOSCSenderExt::timerCallback()
{
	// create history elements if required
	while (pPoints->size() > history.size())
		history.add(new PointHistoryEntry());
		
	for (int i = 0; i < pPoints->size(); i++)
	{
		AmbiPoint* pt = pPoints->get(i);
		if (pt != nullptr)
		{
			if (history[i]->update(pt))
			{
				OSCMessage message = OSCMessage(
					OSCAddressPattern(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_EXTERN),
					OSCArgument(pt->getName()),
					OSCArgument(float(pt->getPoint()->getAzimuth())),
					OSCArgument(float(pt->getPoint()->getElevation())),
					OSCArgument(float(pt->getPoint()->getDistance())));
				oscSender->send(message);
			}
		}
	}
}
