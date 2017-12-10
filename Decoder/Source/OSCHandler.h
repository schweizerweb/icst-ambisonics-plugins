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

class OSCHandler : OSCReceiver, OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	OSCHandler(Array<AmbiPoint>* pAmbiPointArray);
	bool start(int portNb);
	void oscMessageReceived(const OSCMessage& message) override;

private:
	Array<AmbiPoint>* pAmbiPointArray;
};



#endif  // OSCHANDLER_H_INCLUDED
