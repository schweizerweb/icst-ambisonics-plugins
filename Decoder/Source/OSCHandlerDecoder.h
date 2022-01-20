/*
  ==============================================================================

    OSCHandlerDecoder.h
    Created: 4 May 2020 11:22:17am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/OSCHandler.h"

class OSCHandlerDecoder : public OSCHandler
{
public:
    OSCHandlerDecoder(AmbiSourceSet* pAmbiPointArray);
    
protected:
    bool handleSpecific(const OSCMessage &message) override;
    
private:
    void handleOwnInternalStyle(const OSCMessage& message) const;
    bool initSpecific() override;
};
