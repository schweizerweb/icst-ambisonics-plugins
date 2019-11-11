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

OSCHandler::OSCHandler(AmbiSourceSet* pAmbiPointArr, StatusMessageHandler* pStatusMessageHandler)
{
	pAmbiPoints = pAmbiPointArr;
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
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32())
		&& message[4].isInt32();
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)");
		return;
	}

	int channel = message[0].getInt32();
	double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
	int channelCheck = message[4].getInt32();
	if (!checkAed(a, e, d))
		return;

	if (channel == channelCheck)
	{
		if (pAmbiPoints->setChannelNameAED(String(channel), a, e, d))
		{
			reportSuccess();

		}
		else	
		{
			reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")");
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
	float x = message[2].getFloat32();
	float y = message[3].getFloat32();
	float z = message[4].getFloat32();
	float rms = message[5].getFloat32();
	Colour color = Colour(uint32(message[6].getInt32()));
	
	pAmbiPoints->setChannelXYZExt(id, name, x, y, z, rms, color);
}

void OSCHandler::handleOwnExternStyleAed(const OSCMessage& message) const
{
	bool valid =
		message.size() == 4
		&& message[0].isString()
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32());
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style AED)");
		return;
	}

	String channelString = message[0].getString();
	double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
	if(!checkAed(a, e, d))
		return;

	if(pAmbiPoints->setChannelNameAED(channelString, a, e, d))
	{
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")");
	}
}

void OSCHandler::handleOwnExternStyleXyz(const OSCMessage& message) const
{
	bool valid =
		message.size() == 4
		&& message[0].isString()
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32());
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style XYZ)");
		return;
	}

	String channelString = message[0].getString();
	double x = GetIntOrFloat(&message[1]);
	double y = GetIntOrFloat(&message[2]);
	double z = GetIntOrFloat(&message[3]);
	if (!checkXyz(x, y, z))
		return;

	if (pAmbiPoints->setChannelNameXYZ(channelString, x, y, z))
	{
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")");
	}
}

void OSCHandler::handleOwnExternStyleIndexAed(const OSCMessage& message) const
{
	bool valid =
		message.size() == 4
		&& message[0].isInt32()
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32());
	if(!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST index AED style)");
		return;
	}

	int channel = message[0].getInt32();
	double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
	if (!checkAed(a, e, d))
		return;

	if(pAmbiPoints->setChannelAED(channel-1, a, e, d))
	{
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")");
	}
}

void OSCHandler::handleOwnExternStyleIndexXyz(const OSCMessage& message) const
{
	bool valid =
		message.size() == 4
		&& message[0].isInt32()
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32());
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST index XYZ style)");
		return;
	}

	int channel = message[0].getInt32();
	double x = GetIntOrFloat(&message[1]);
	double y = GetIntOrFloat(&message[2]);
	double z = GetIntOrFloat(&message[3]);
	if (!checkXyz(x, y, z))
		return;

	if(pAmbiPoints->setChannelXYZ(channel-1, x, y, z))
	{
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel-1) + ")");
	}
}

void OSCHandler::handleOwnExternStyleGroupAed(const OSCMessage& message) const
{
	bool valid =
		message.size() == 5
		&& message[0].isString()
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32())
		&& message[4].isInt32();
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style group AED)");
		return;
	}

	String groupString = message[0].getString();
	double a = Constants::GradToRad(GetIntOrFloat(&message[1]));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
	bool movePoints = message[4].getInt32();
	if (!checkAed(a, e, d))
		return;

	if (pAmbiPoints->setGroupAed(groupString, a, e, d, movePoints))
	{
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")");
	}
}

void OSCHandler::handleOwnExternStyleGroupXyz(const OSCMessage& message) const
{
	bool valid =
		message.size() == 5
		&& message[0].isString()
		&& (message[1].isInt32() || message[1].isFloat32())
		&& (message[2].isInt32() || message[2].isFloat32())
		&& (message[3].isInt32() || message[3].isFloat32())
		&& message[4].isInt32();
	if (!valid)
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style group XYZ)");
		return;
	}

	String groupString = message[0].getString();
	double x = GetIntOrFloat(&message[1]);
	double y = GetIntOrFloat(&message[2]);
	double z = GetIntOrFloat(&message[3]);
	bool movePoints = message[4].getInt32();
	if (!checkXyz(x, y, z))
		return;

	if (pAmbiPoints->setGroupXyz(groupString, x, y, z, movePoints))
	{
		reportSuccess();
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")");
	}
}

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	OSCAddressPattern pattern = message.getAddressPattern();
	if (pattern.matches(OSCAddress(OSC_ADDRESS_MUSESCORE_SSMN)))
	{
		handleMusescoreSSMNStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_INTERNAL)))
	{
		handleOwnInternalStyle(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED)))
	{
		handleOwnExternStyleAed(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ)))
	{
		handleOwnExternStyleXyz(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED)))
	{
		handleOwnExternStyleGroupAed(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ)))
	{
		handleOwnExternStyleGroupXyz(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED)))
	{
		handleOwnExternStyleIndexAed(message);
	}
	else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ)))
	{
		handleOwnExternStyleIndexXyz(message);
	}
	else
	{
		reportError("Invalid OSC pattern received: " + pattern.toString());
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
	if (d < Constants::DistanceMin)
	{
		reportError("OSC-Message Azimuth out of range: " + String(d));
		return false;
	}

	return true;
}

bool OSCHandler::checkXyz(double x, double y, double z) const
{
	if (x < Constants::XMin || x > Constants::XMax)
	{
		reportError("OSC-Message X out of range: " + String(x));
		return false;
	}
	if (y < Constants::YMin || y > Constants::YMax)
	{
		reportError("OSC-Message Y out of range: " + String(y));
		return false;
	}
	if (z < Constants::ZMin || z > Constants::ZMax)
	{
		reportError("OSC-Message Z out of range: " + String(z));
		return false;
	}

	return true;
}

void OSCHandler::stop()
{
	disconnect();
}

double OSCHandler::GetIntOrFloat(const OSCArgument* pOscArgument)
{
	return pOscArgument->isInt32() ? double(pOscArgument->getInt32()) : pOscArgument->getFloat32();
}
