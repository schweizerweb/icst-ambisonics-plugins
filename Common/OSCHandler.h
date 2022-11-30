/*
  ==============================================================================

    OSCHandler.h
    Created: 18 Mar 2017 5:07:47pm
    Author:  Christian Schweizer

  ==============================================================================
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
