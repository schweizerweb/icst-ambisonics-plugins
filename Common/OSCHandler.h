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

#define OSC_ADDRESS_MUSESCORE_SSMN "/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_INTERNAL "/icst/ambi/source/internal"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED "/icst/ambi/source/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ "/icst/ambi/source/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED "/icst/ambi/group/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ "/icst/ambi/group/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED "/icst/ambi/sourceindex/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ "/icst/ambi/sourceindex/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE "/icst/ambi/group/rotate"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE_ORIGIN "/icst/ambi/group/rotateorigin"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_STRETCH "/icst/ambi/group/stretch"

#define ERROR_STRING_MALFORMATTED_OSC String("Malformatted OSC message received ")
#define ERROR_STRING_NONEXISTING_TARGET String("OSC message for non-existing target received ")

class OSCHandler : OSCReceiver, OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	OSCHandler(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler = nullptr);
	bool start(int portNb);
	void stop();

private:
	static double GetIntOrFloat(const OSCArgument* pOscArgument);
	void handleMusescoreSSMNStyle(const OSCMessage& message) const;
	void handleOwnInternalStyle(const OSCMessage& message) const;
	void handleOwnExternStyleAed(const OSCMessage& message) const;
	void handleOwnExternStyleXyz(const OSCMessage& message) const;
	void handleOwnExternStyleIndexAed(const OSCMessage& message) const;
	void handleOwnExternStyleIndexXyz(const OSCMessage& message) const;
	void handleOwnExternStyleGroupAed(const OSCMessage& message) const;
	void handleOwnExternStyleGroupXyz(const OSCMessage& message) const;
    void handleOwnExternStyleGroupRotate(const OSCMessage& message) const;
    void handleOwnExternStyleGroupRotateOrigin(const OSCMessage& message) const;
    void handleOwnExternStyleGroupStretch(const OSCMessage& message) const;
	void oscMessageReceived(const OSCMessage& message) override;
	void reportError(String message, const OSCMessage* pMsg) const;
	void reportSuccess(const OSCMessage* pMsg) const;
	bool checkAed(double a, double e, double d, String* errorString) const;
	bool checkXyz(double x, double y, double z, String* errorString) const;
    String oscMessageToString(const OSCMessage* pMsg) const;
	AmbiSourceSet* pAmbiPoints;
	StatusMessageHandler* pStatusMessageHandler;
};



#endif  // OSCHANDLER_H_INCLUDED
