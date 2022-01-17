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

    ok = initSpecific();
    if (!ok)
        return false;
    
	addListener(this);

	return true;
}

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	if(!handleSpecific(message))
	{
		reportError("Unknown OSC message received: " + message.getAddressPattern().toString(), &message);
	}
}

void OSCHandler::reportError(String message, const OSCMessage* pMsg) const
{
	if(pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage(message, message + oscMessageToString(pMsg), StatusMessage::Error);
	}
}

void OSCHandler::reportSuccess(const OSCMessage* pMsg) const
{
    if (pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage("OSC", oscMessageToString(pMsg), StatusMessage::Success);
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

void OSCHandler::stop()
{
	disconnect();
}

double OSCHandler::GetIntOrFloat(const OSCArgument* pOscArgument)
{
	return pOscArgument->isInt32() ? double(pOscArgument->getInt32()) : pOscArgument->getFloat32();
}
