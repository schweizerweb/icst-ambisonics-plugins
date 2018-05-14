/*
  ==============================================================================

    OSCHandler.cpp
    Created: 18 Mar 2017 5:07:47pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "OSCHandler.h"

OSCHandler::OSCHandler(OwnedArray<AmbiPoint>* pAmbiPointArr)
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

void OSCHandler::HandleMusescoreSSMNStyle(const OSCMessage& message) const
{
	if (message.size() < 5)
		return;

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
			if (((*pAmbiPointArray)[i])->getName() == channelString)
			{
				index = i;
				break;
			}
		}

		if (index == -1)
		{
			ScopedPointer<Uuid> newId = new Uuid();
			pAmbiPointArray->add(new AmbiPoint(newId->toString(), Point3D<double>(0.0, 0.0, 0.0), channelString, channel));
			index = pAmbiPointArray->size() - 1;
		}

		AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(index);
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth((int(a)%360) / 180.0 * PI);
		ambiPt->getPoint()->setElevation(e / 180.0 * PI);
	}
}

void OSCHandler::HandleOwnInternalStyle(const OSCMessage& message) const
{
	if (message.size() < 7)
		return;

	String id = message[0].getString();
	String name = message[1].getString();
	float a = message[2].getFloat32();
	float e = message[3].getFloat32();
	float d = message[4].getFloat32();
	float rms = message[5].getFloat32();
	int colorIndex = message[6].getInt32();

	int index = -1;
	for (int i = 0; i < pAmbiPointArray->size(); i++)
	{
		if ((*pAmbiPointArray)[i]->getId() == id)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		pAmbiPointArray->add(new AmbiPoint(id, Point3D<double>(0.0, 0.0, 0.0), name, colorIndex));
		index = pAmbiPointArray->size() - 1;
	}

	AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(index);
	ambiPt->setName(name);
	ambiPt->getPoint()->setAzimuth(a);
	ambiPt->getPoint()->setElevation(e);
	ambiPt->getPoint()->setDistance(d);
	ambiPt->setRms(rms);
	ambiPt->setColorIndex(colorIndex);
	ambiPt->setAlive(Time::currentTimeMillis());
}

void OSCHandler::HandleOwnExternStyle(const OSCMessage& message) const
{
	if (message.size() < 4)
		return;

	String channelString = message[0].getString();
	float a = message[1].getFloat32();
	float e = message[2].getFloat32();
	float d = message[3].getFloat32();

	for (int i = 0; i < pAmbiPointArray->size(); i++)
	{
		if (((*pAmbiPointArray)[i])->getName() == channelString)
		{
			AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(i);
			ambiPt->getPoint()->setDistance(d);
			ambiPt->getPoint()->setAzimuth((int(a) % 360) / 180.0 * PI);
			ambiPt->getPoint()->setElevation(e / 180.0 * PI);
			break;
		}
	}
}

void OSCHandler::HandleOwnExternIntStyle(const OSCMessage& message) const
{
	if (message.size() < 4)
		return;

	int channel = message[0].getInt32();
	float a = message[1].getFloat32();
	float e = message[2].getFloat32();
	float d = message[3].getFloat32();

	channel--;	// channel comes 1-based, has to be used here as 0-based

	if (pAmbiPointArray->size() > channel)
	{
		AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(channel);
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth((int(a) % 360) / 180.0 * PI);
		ambiPt->getPoint()->setElevation(e / 180.0 * PI);
	}
}

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	OSCAddressPattern pattern = message.getAddressPattern();
	if (pattern.matches(OSCAddress(OSC_ADDRESS_MUSESCORE_SSMN)))
	{
		HandleMusescoreSSMNStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS)))
	{
		HandleOwnInternalStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_EXTERN)))
	{
		HandleOwnExternStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_INT_EXTERN)))
	{
		HandleOwnExternIntStyle(message);
	}
}

void OSCHandler::stop()
{
	disconnect();
}
