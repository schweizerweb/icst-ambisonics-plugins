/*
  ==============================================================================

    OSCHandler.h
    Created: 18 Mar 2017 5:07:47pm
    Author:  christian.schweizer

  ==============================================================================
*/

#ifndef OSCHANDLER_H_INCLUDED
#define OSCHANDLER_H_INCLUDED

#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"

#define OSC_ADDRESS_MUSESCORE_SSMN "/aed"
#define OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS "/zhdk/ambi/source/internal"
#define OSC_ADDRESS_ZHDK_AMBISONIC_PLUGINS_EXTERN "/zhdk/ambi/source"

class OSCHandler : OSCReceiver, OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	OSCHandler(Array<AmbiPoint>* pAmbiPointArray);
	bool start(int portNb);
	void HandleMusescoreSSMNStyle(const OSCMessage& message) const;
	void HandleOwnInternalStyle(const OSCMessage& message) const;
	void HandleOwnExternStyle(const OSCMessage& message) const;
	void oscMessageReceived(const OSCMessage& message) override;
	void stop();

private:
	Array<AmbiPoint>* pAmbiPointArray;
};



#endif  // OSCHANDLER_H_INCLUDED
