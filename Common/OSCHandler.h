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



#pragma once
#include "JuceHeader.h"
#include "StatusMessageHandler.h"
#include "AmbiSourceSet.h"

#define OSC_ADDRESS_AMBISONIC_PLUGINS_INTERNAL "/icst/ambi/source/internal"
#define ERROR_STRING_MALFORMATTED_OSC String("Malformatted OSC message received ")
#define ERROR_STRING_UNKNOWN_OSC String("Unknown OSC message received ")
#define ERROR_STRING_NONEXISTING_TARGET String("OSC message for non-existing target received ")

class OSCHandler : OSCReceiver, OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	OSCHandler(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler = nullptr);
    bool start(int portNb);
    void stop();
    void setVerbosity(bool reportSuccess = true, bool reportError = true);
    
protected:
    virtual bool initSpecific() = 0;
    virtual bool handleSpecific(const OSCMessage& message) = 0;
	static double GetIntOrFloat(const OSCArgument* pOscArgument);
	void oscMessageReceived(const OSCMessage& message) override;
    void oscBundleReceived(const OSCBundle &) override;
	void reportError(String message, const OSCMessage* pMsg) const;
	void reportSuccess(const OSCMessage* pMsg) const;
	String oscMessageToString(const OSCMessage* pMsg) const;
    
	AmbiSourceSet* pAmbiPoints;
	StatusMessageHandler* pStatusMessageHandler;
    
private:
    bool reportSuccessFlag;
    bool reportErrorFlag;
};
