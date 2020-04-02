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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)", &message);
		return;
	}

	int channel = message[0].getInt32();
	double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
	int channelCheck = message[4].getInt32();
    String errorString;
	if (!checkAed(a, e, d, &errorString))
    {
        reportError(errorString, &message);
        return;
    }
    
	if (channel == channelCheck)
	{
		if (pAmbiPoints->setChannelNameAED(String(channel), a, e, d))
		{
			reportSuccess(&message);

		}
		else	
		{
			reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
		}
	}
	else
	{
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(internal style)", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style AED)", &message);
		return;
	}

	String channelString = message[0].getString();
	double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
    String errorMessage;
	if(!checkAed(a, e, d, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
	if(pAmbiPoints->setChannelNameAED(channelString, a, e, d))
	{
		reportSuccess(&message);
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style XYZ)", &message);
		return;
	}

	String channelString = message[0].getString();
	double x = GetIntOrFloat(&message[1]);
	double y = GetIntOrFloat(&message[2]);
	double z = GetIntOrFloat(&message[3]);
    String errorMessage;
	if (!checkXyz(x, y, z, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
	if (pAmbiPoints->setChannelNameXYZ(channelString, x, y, z))
	{
		reportSuccess(&message);
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST index AED style)", &message);
		return;
	}

	int channel = message[0].getInt32();
	double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
    String errorMessage;
	if (!checkAed(a, e, d, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
	if(pAmbiPoints->setChannelAED(channel-1, a, e, d))
	{
		reportSuccess(&message);
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST index XYZ style)", &message);
		return;
	}

	int channel = message[0].getInt32();
	double x = GetIntOrFloat(&message[1]);
	double y = GetIntOrFloat(&message[2]);
	double z = GetIntOrFloat(&message[3]);
    String errorMessage;
	if (!checkXyz(x, y, z, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
	if(pAmbiPoints->setChannelXYZ(channel-1, x, y, z))
	{
		reportSuccess(&message);
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel-1) + ")", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style group AED)", &message);
		return;
	}

	String groupString = message[0].getString();
	double a = Constants::GradToRad(GetIntOrFloat(&message[1]));
	double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
	double d = GetIntOrFloat(&message[3]);
	bool movePoints = message[4].getInt32();
    String errorMessage;
	if (!checkAed(a, e, d, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
	if (pAmbiPoints->setGroupAed(groupString, a, e, d, movePoints))
	{
		reportSuccess(&message);
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
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
		reportError(ERROR_STRING_MALFORMATTED_OSC + "(ICST string style group XYZ)", &message);
		return;
	}

	String groupString = message[0].getString();
	double x = GetIntOrFloat(&message[1]);
	double y = GetIntOrFloat(&message[2]);
	double z = GetIntOrFloat(&message[3]);
	bool movePoints = message[4].getInt32();
    String errorMessage;
	if (!checkXyz(x, y, z, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
	if (pAmbiPoints->setGroupXyz(groupString, x, y, z, movePoints))
	{
		reportSuccess(&message);
	}
	else
	{
		reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
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
		reportError("Invalid OSC pattern received: " + pattern.toString(), &message);
	}
}

void OSCHandler::reportError(String message, const OSCMessage* pMsg) const
{
	if(pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage(message, oscMessageToString(pMsg) + "\r\n\t" + message, StatusMessageHandler::Error);
	}
}

void OSCHandler::reportSuccess(const OSCMessage* pMsg) const
{
    if (pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage("OSC", oscMessageToString(pMsg), StatusMessageHandler::Success);
	}
}

String OSCHandler::oscMessageToString(const OSCMessage *pMsg) const
{
    String detailString = pMsg->getAddressPattern().toString();
    for(int i = 0; i < pMsg->size(); i++)
    {
        if((*pMsg)[i].isString())
        {
            detailString += " \"" + ((*pMsg)[i].getString()) + "\"";
        }
        else if((*pMsg)[i].isInt32() || (*pMsg)[i].isFloat32())
        {
            detailString += " " + String(GetIntOrFloat(&((*pMsg)[i])));
        }
    }
    return detailString;
}

bool OSCHandler::checkAed(double a, double e, double d, String* errorString) const
{
	if(a < Constants::AzimuthRadMin || a > Constants::AzimuthRadMax)
	{
        *errorString = "OSC-Message Azimuth out of range: " + String(a);
		return false;
	}
	if(e < Constants::ElevationRadMin || e > Constants::ElevationRadMax)
	{
		*errorString = "OSC-Message Elevation out of range: " + String(e);
		return false;
	}
	if (d < Constants::DistanceMin)
	{
		*errorString = "OSC-Message Azimuth out of range: " + String(d);
		return false;
	}

	return true;
}

bool OSCHandler::checkXyz(double x, double y, double z, String* errorString) const
{
	if (x < Globals::CartesianMin() || x > Globals::CartesianMax())
	{
		*errorString = "OSC-Message X out of range: " + String(x);
		return false;
	}
	if (y < Globals::CartesianMin() || y > Globals::CartesianMax())
	{
		*errorString = "OSC-Message Y out of range: " + String(y);
		return false;
	}
	if (z < Globals::CartesianMin() || z > Globals::CartesianMax())
	{
		*errorString = "OSC-Message Z out of range: " + String(z);
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
