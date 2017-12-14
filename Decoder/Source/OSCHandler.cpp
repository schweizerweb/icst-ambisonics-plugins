/*
  ==============================================================================

    OSCHandler.cpp
    Created: 18 Mar 2017 5:07:47pm
    Author:  christian.schweizer

  ==============================================================================
*/

#include "OSCHandler.h"

OSCHandler::OSCHandler(Array<AmbiPoint>* pAmbiPointArr)
{
	pAmbiPointArray = pAmbiPointArr;
}

bool OSCHandler::start(int portNb)
{
	disconnect();
	bool ok = connect(portNb);

	if (!ok)
		return false;

	addListener(this);

	return true;
}

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	if (message.size() > 0)
	{
		int channel = message[0].getInt32();
		float a = message[1].getFloat32();
		float e = message[2].getFloat32();
		float d = message[3].getFloat32();
		int channelCheck = message[4].getInt32();

		if (channel == channelCheck)
		{
			String channelString = String(channel);
			int index = -1;

			for (int i = 0; i < pAmbiPointArray->size(); i++)
			{
				if (((*pAmbiPointArray)[i]).getName() == channelString)
				{
					index = i;
					break;
				}
			}

			if (index == -1)
			{
				pAmbiPointArray->add(AmbiPoint(Point3D<double>(0.0, 0.0, 0.0), channelString, channel));
				index = pAmbiPointArray->size() - 1;
			}

			AmbiPoint* ambiPt = &pAmbiPointArray->getReference(index);
			ambiPt->getPoint()->setDistance(d);
			ambiPt->getPoint()->setAzimuth((int(a)%360) / 180.0 * PI);
			ambiPt->getPoint()->setElevation(e / 180.0 * PI);
		}
	}
}

void OSCHandler::stop()
{
	disconnect();
}
