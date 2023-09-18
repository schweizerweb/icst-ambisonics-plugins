/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
        Vector3D<double> absPt = pPoints->getAbsSourcePoint(i);
        if (pt != nullptr && pt->getEnabled())
		{
            OSCMessage message = OSCMessage(
				OSCAddressPattern(OSC_ADDRESS_AMBISONIC_PLUGINS_INTERNAL),
				OSCArgument(pt->getId()),
				OSCArgument(pt->getName()),
				OSCArgument(float(absPt.x)),
				OSCArgument(float(absPt.y)),
				OSCArgument(float(absPt.z)),
				OSCArgument(pt->getRms()),
				OSCArgument(int32(pt->getColor().getARGB())));
			oscSender->send(message);
			pPoints->setRms(i, 0.0f, false);
		}
	}

	startTimer(sendIntervalMs);
}
