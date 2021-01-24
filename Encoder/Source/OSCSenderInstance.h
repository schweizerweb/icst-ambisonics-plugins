/*
  ==============================================================================

    OSCSenderInstance.h
    Created: 17 Jan 2021 9:49:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "OSCHandlerEncoder.h"
#include "../../Common/AmbiPoint.h"

class OSCSenderInstance
{
public:
	OSCSenderInstance() { sender.reset(new OSCSender()); }
	~OSCSenderInstance() { sender = nullptr; }
    
    enum ParameterType { Index, Name, Color, A, E, D, X, Y, Z, Gain };

	std::map<ParameterType, String> escapeStringMap = {
		std::make_pair(Index, "{i}"),
		std::make_pair(Name, "{n}"),
		std::make_pair(Color, "{c}"),
		std::make_pair(A, "{a}"),
		std::make_pair(E, "{e}"),
		std::make_pair(D, "{d}"),
		std::make_pair(X, "{x}"),
		std::make_pair(Y, "{y}"),
		std::make_pair(Z, "{z}"),
		std::make_pair(Gain, "{g}")
	};

    bool connect(String host, int port);
	void disconnect();
	void sendMessage(AmbiPoint* pt, int index);
	void setOscPath(String path);

private:
    std::unique_ptr<OSCSender> sender;
	String oscPath;
	Array<ParameterType> parametersInPath;
	Array<ParameterType> realParameters;
	bool isConnected;
	CriticalSection cs;
};

