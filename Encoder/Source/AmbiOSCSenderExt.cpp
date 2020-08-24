/*
  ==============================================================================

    AmbiOSCSenderExt.cpp
    Created: 2 Jul 2019 1:15:15pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiOSCSenderExt.h"
#include "OSCHandlerEncoder.h"

AmbiOSCSenderExt::AmbiOSCSenderExt(AmbiDataSet* ambiPoints): pPoints(ambiPoints)
{
	oscSender = new OSCSender();
}

AmbiOSCSenderExt::~AmbiOSCSenderExt()
{
	stop();
	delete oscSender;
}

bool AmbiOSCSenderExt::start(String targetHost, int port)
{
	stop();
	
	bool ret = oscSender->connect(targetHost, port);
	if (ret)
	{
		startTimerHz(20);
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
				OSCMessage messageAed = OSCMessage(
					OSCAddressPattern(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED),
					OSCArgument(pt->getName()),
					OSCArgument(float(Constants::RadToGrad(double(pt->getPoint()->getAzimuth())))),
					OSCArgument(float(Constants::RadToGrad(double(pt->getPoint()->getElevation())))),
					OSCArgument(float(pt->getPoint()->getDistance())));
				oscSender->send(messageAed);

				OSCMessage messageXyz = OSCMessage(
					OSCAddressPattern(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ),
					OSCArgument(pt->getName()),
					OSCArgument(float(pt->getPoint()->getX())),
					OSCArgument(float(pt->getPoint()->getY())),
					OSCArgument(float(pt->getPoint()->getZ())));
				oscSender->send(messageXyz);
			}
		}
	}
}
