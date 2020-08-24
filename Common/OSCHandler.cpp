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

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	if(!handleSpecific(message))
	{
		reportError("Invalid OSC pattern received: " + message.getAddressPattern().toString(), &message);
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

bool OSCHandler::checkGain(double gain, String *errorString) const
{
    if(gain < Constants::GainDbMin || gain > Constants::GainDbMax)
    {
        *errorString = "OSC-Message Gain out of range: " + String(gain);
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
