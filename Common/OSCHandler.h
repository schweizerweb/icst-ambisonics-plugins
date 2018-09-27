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
#include "../../Common/AmbiPoint.h"
#include "StatusMessageHandler.h"

#define OSC_ADDRESS_MUSESCORE_SSMN "/aed"
#define OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS "/zhdk/ambi/source/internal"
#define OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_EXTERN "/zhdk/ambi/source"
#define OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_INT_EXTERN "/zhdk/ambi/channelsource"
#define ERROR_STRING_MALFORMATTED_OSC String("Malformatted OSC message received ")
#define ERROR_STRING_NONEXISTING_TARGET String("OSC message for non-existing target received ")

class OSCHandler : OSCReceiver, OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	OSCHandler(OwnedArray<AmbiPoint>* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler = nullptr);
	bool start(int portNb);
	void stop();

private:
	void handleMusescoreSSMNStyle(const OSCMessage& message) const;
	void handleOwnInternalStyle(const OSCMessage& message) const;
	void handleOwnExternStyle(const OSCMessage& message) const;
	void handleOwnExternIntStyle(const OSCMessage& message) const;
	void oscMessageReceived(const OSCMessage& message) override;
	void reportError(String message) const;
	void reportSuccess() const;
	bool checkAed(double a, double e, double d) const;

	OwnedArray<AmbiPoint>* pAmbiPointArray;
	StatusMessageHandler* pStatusMessageHandler;
};



#endif  // OSCHANDLER_H_INCLUDED
