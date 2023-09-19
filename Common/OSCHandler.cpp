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



#include "OSCHandler.h"
#include "TrackColors.h"
#include "Constants.h"

OSCHandler::OSCHandler(AmbiSourceSet* pAmbiPointArr, StatusMessageHandler* _pStatusMessageHandler)
{
    reportErrorFlag = true;
    reportSuccessFlag = true;
	pAmbiPoints = pAmbiPointArr;
	this->pStatusMessageHandler = _pStatusMessageHandler;
}

void OSCHandler::setVerbosity(bool reportSuccess, bool reportError)
{
    reportErrorFlag = reportError;
    reportSuccessFlag = reportSuccess;
}

bool OSCHandler::start(int portNb)
{
	disconnect();
	bool ok = connect(portNb);
	if (!ok)
	{
    	return false;
    }

    ok = initSpecific();
    if (!ok)
    {
        return false;
    }

	addListener(this);

	return true;
}

void OSCHandler::oscMessageReceived(const OSCMessage & message)
{
	if(!handleSpecific(message))
	{
		reportError("Unknown OSC message received: ", &message);
	}
}

void OSCHandler::oscBundleReceived(const OSCBundle & bundle)
{
    for(auto & e : bundle)
    {
        if(e.isBundle())
        {
            oscBundleReceived(e.getBundle());
        }
        else
        {
            oscMessageReceived(e.getMessage());
        }
    }
}

void OSCHandler::reportError(String message, const OSCMessage* pMsg) const
{
	if(reportErrorFlag && pStatusMessageHandler != nullptr)
	{
		pStatusMessageHandler->showMessage(message, message + oscMessageToString(pMsg), StatusMessage::Error);
	}
}

void OSCHandler::reportSuccess(const OSCMessage* pMsg) const
{
    if (reportSuccessFlag && pStatusMessageHandler != nullptr)
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
