/*
  ==============================================================================

    OSCSenderInstance.h
    Created: 17 Jan 2021 9:49:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "UserDefinedParameter.h"
#include "CustomOscBase.h"

class OSCSenderInstance : public CustomOscBase
{
public:
    OSCSenderInstance(ScalingInfo* pScaling);
    ~OSCSenderInstance();

    bool connect(String host, int port);
	void disconnect();
	void sendMessage(Vector3D<double> absPt, AmbiPoint* pt, int index);
	
private:
    std::unique_ptr<OSCSender> sender;
	bool isConnected;
    CriticalSection cs;
};

