/*
  ==============================================================================

    OSCHandler.cpp
    Created: 18 Mar 2017 5:07:47pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "OSCHandler.h"
#include "TrackColors.h"
#include "Constants.h"

OSCHandler::OSCHandler(OwnedArray<AmbiPoint>* pAmbiPointArr, StatusMessageHandler* pStatusMessageHandler)
{
	pAmbiPointArray = pAmbiPointArr;
	this->pStatusMessageHandler = pStatusMessageHandler;
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

void OSCHandler::handleMusescoreSSMNStyle(const OSCMessage& message) const
{
	bool valid =
		message.size() == 5
		&& message[0].isInt32()
		&& message[1].isFloat32()
		&& message[2].isFloat32()
		&& message[3].isFloat32()
		&& message[4].isInt32();
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)");
		return;
	}

	int channel = message[0].getInt32();
	double a = Constants::GradToRad(int(message[1].getFloat32()) % 360);
	double e = Constants::GradToRad(message[2].getFloat32());
	double d = message[3].getFloat32();
	int channelCheck = message[4].getInt32();
	if (!checkAed(a, e, d))
		return;

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
			reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")");
		}
		else
		{
			AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(index);
			ambiPt->getPoint()->setDistance(d);
			ambiPt->getPoint()->setAzimuth(a);
			ambiPt->getPoint()->setElevation(e);
			reportSuccess();
		}
	}
	else
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)");
	}
}

void OSCHandler::handleOwnInternalStyle(const OSCMessage& message) const
{
	bool valid =
		message.size() == 7
		&& message[0].isString()
		&& message[1].isString()
		&& message[2].isFloat32()
		&& message[3].isFloat32()
		&& message[4].isFloat32()
		&& message[5].isFloat32()
		&& message[6].isInt32();
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(internal style)");
		return;
	}
	
	String id = message[0].getString();
	String name = message[1].getString();
	float a = message[2].getFloat32();
	float e = message[3].getFloat32();
	float d = message[4].getFloat32();
	float rms = message[5].getFloat32();
	Colour color = Colour(uint32(message[6].getInt32()));
	if (!checkAed(a, e, d))
		return;

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
		pAmbiPointArray->add(new AmbiPoint(id, Point3D<double>(0.0, 0.0, 0.0), name, color));
		index = pAmbiPointArray->size() - 1;
	}

	AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(index);
	ambiPt->setName(name);
	ambiPt->getPoint()->setAzimuth(a);
	ambiPt->getPoint()->setElevation(e);
	ambiPt->getPoint()->setDistance(d);
	ambiPt->setRms(rms);
	ambiPt->setColor(color);
	ambiPt->setAlive(Time::currentTimeMillis());
}

void OSCHandler::handleOwnExternStyle(const OSCMessage& message) const
{
	bool valid =
		message.size() == 4
		&& message[0].isString()
		&& message[1].isFloat32()
		&& message[2].isFloat32()
		&& message[3].isFloat32();
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ZHdK string style)");
		return;
	}

	String channelString = message[0].getString();
	double a = Constants::GradToRad(int(message[1].getFloat32()) % 360);
	double e = Constants::GradToRad(message[2].getFloat32());
	double d = message[3].getFloat32();
	if(!checkAed(a, e, d))
		return;

	AmbiPoint* ambiPt = nullptr;

	for (int i = 0; i < pAmbiPointArray->size(); i++)
	{
		if (((*pAmbiPointArray)[i])->getName() == channelString)
		{
			ambiPt = pAmbiPointArray->getUnchecked(i);
			break;
		}
	}

	if(ambiPt != nullptr)
	{
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth(a);
		ambiPt->getPoint()->setElevation(e);
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")");
	}
}

void OSCHandler::handleOwnExternIntStyle(const OSCMessage& message) const
{
	bool valid =
		message.size() == 4
		&& message[0].isInt32()
		&& message[1].isFloat32()
		&& message[2].isFloat32()
		&& message[3].isFloat32();
	if(!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ZHdK index style)");
		return;
	}

	int channel = message[0].getInt32();
	double a = Constants::GradToRad(int(message[1].getFloat32()) % 360);
	double e = Constants::GradToRad(message[2].getFloat32());
	double d = message[3].getFloat32();
	if (!checkAed(a, e, d))
		return;

	if (pAmbiPointArray->size() >= channel && channel > 0)
	{
		AmbiPoint* ambiPt = pAmbiPointArray->getUnchecked(channel-1); // 1-based in message, 0-based in plugin
		ambiPt->getPoint()->setDistance(d);
		ambiPt->getPoint()->setAzimuth(a);
		ambiPt->getPoint()->setElevation(e);
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")");
	}
}

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	OSCAddressPattern pattern = message.getAddressPattern();
	if (pattern.matches(OSCAddress(OSC_ADDRESS_MUSESCORE_SSMN)))
	{
		handleMusescoreSSMNStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS)))
	{
		handleOwnInternalStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_EXTERN)))
	{
		handleOwnExternStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_INT_EXTERN)))
	{
		handleOwnExternIntStyle(message);
	}
}

void OSCHandler::reportError(String message) const
{
	if(pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage(message, StatusMessageHandler::Error);
	}
}

void OSCHandler::reportSuccess() const
{
	if (pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage("OSC", StatusMessageHandler::Success);
	}
}

bool OSCHandler::checkAed(double a, double e, double d) const
{
	if(a < Constants::AzimuthRadMin || a > Constants::AzimuthRadMax)
	{
		reportError("OSC-Message Azimuth out of range: " + String(a));
		return false;
	}
	if(e < Constants::ElevationRadMin || e > Constants::ElevationRadMax)
	{
		reportError("OSC-Message Elevation out of range: " + String(e));
		return false;
	}
	if (d < Constants::DistanceMin || d > Constants::DistanceMax)
	{
		reportError("OSC-Message Azimuth out of range: " + String(d));
		return false;
	}

	return true;
}

void OSCHandler::stop()
{
	disconnect();
}
