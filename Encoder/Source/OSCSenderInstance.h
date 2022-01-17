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

class OSCSenderInstance
{
public:
    OSCSenderInstance(ScalingInfo* pScaling);
    ~OSCSenderInstance();

    bool connect(String host, int port);
	void disconnect();
	void sendMessage(AmbiPoint* pt, int index);
	bool setOscPath(String path);
	String getOscPath();

private:
    bool analyzeString(std::string parameterString, Array<UserDefinedParameter>* pArray);
    
    float dualMap(double value, double maxValue, UserDefinedParameter* pParam);
    std::unique_ptr<OSCSender> sender;
	String oscPath;
	Array<UserDefinedParameter> parametersInPath;
    Array<UserDefinedParameter> realParameters;
	bool isConnected;
	CriticalSection cs;
    ScalingInfo* pScalingInfo;
};

