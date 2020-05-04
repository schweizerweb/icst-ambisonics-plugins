/*
  ==============================================================================

    OSCHandler.h
    Created: 18 Mar 2017 5:07:47pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#ifndef OSCHANDLER_H_INCLUDED
#define OSCHANDLER_H_INCLUDED

#include "JuceHeader.h"
#include "StatusMessageHandler.h"
#include "AmbiSourceSet.h"

#define ERROR_STRING_MALFORMATTED_OSC String("Malformatted OSC message received ")
#define ERROR_STRING_NONEXISTING_TARGET String("OSC message for non-existing target received ")

class OSCHandler : OSCReceiver, OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	OSCHandler(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler = nullptr);
	bool start(int portNb);
	void stop();

protected:
    virtual bool handleSpecific(const OSCMessage& message) = 0;
	static double GetIntOrFloat(const OSCArgument* pOscArgument);
	void oscMessageReceived(const OSCMessage& message) override;
	void reportError(String message, const OSCMessage* pMsg) const;
	void reportSuccess(const OSCMessage* pMsg) const;
	bool checkAed(double a, double e, double d, String* errorString) const;
	bool checkXyz(double x, double y, double z, String* errorString) const;
    bool checkGain(double gain, String* errorString) const;
    String oscMessageToString(const OSCMessage* pMsg) const;
    
	AmbiSourceSet* pAmbiPoints;
	StatusMessageHandler* pStatusMessageHandler;
};



#endif  // OSCHANDLER_H_INCLUDED
