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

class OSCSenderInstance
{
public:
    OSCSenderInstance(ScalingInfo* pScaling);
    ~OSCSenderInstance();

    enum ParameterType { Index, Name, Color, A, E, D, X, Y, Z, ScaledA, ScaledE, ScaledD, ScaledX, ScaledY, ScaledZ, Gain };
    class ComplexParameter
    {
    public:
        ComplexParameter(String originalString, std::string typeString, double lo, double hi) : originalString(originalString), loLim(lo), hiLim(hi)
        {
            switch(typeString.substr()[0])
            {
                case 'x': type = ParameterType::ScaledX; break;
                case 'y': type = ParameterType::ScaledY; break;
                case 'z': type = ParameterType::ScaledZ; break;
                case 'a': type = ParameterType::ScaledA; break;
                case 'e': type = ParameterType::ScaledE; break;
                case 'd': type = ParameterType::ScaledD; break;
            }
        }
        
        ComplexParameter(ParameterType t)
        {
            type = t;
            switch(t)
            {
                case ScaledA:
                case ScaledE:
                case ScaledD:
                case ScaledX:
                case ScaledY:
                case ScaledZ:
                    loLim = 0.0; hiLim = 1.0; break;
                default: break;
            }
        }
        
        String originalString;
        ParameterType type;
        double loLim;
        double hiLim;
    };
    
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
		std::make_pair(ScaledA, "{sa}"),
		std::make_pair(ScaledE, "{se}"),
		std::make_pair(ScaledD, "{sd}"),
		std::make_pair(ScaledX, "{sx}"),
		std::make_pair(ScaledY, "{sy}"),
		std::make_pair(ScaledZ, "{sz}"),
		std::make_pair(Gain, "{g}")
	};

    bool connect(String host, int port);
	void disconnect();
	void sendMessage(AmbiPoint* pt, int index);
	bool setOscPath(String path);
	String getOscPath();

private:
    std::unique_ptr<OSCSender> sender;
	String oscPath;
	Array<ParameterType> parametersInPath;
    Array<ComplexParameter> realParameters;
	bool isConnected;
	CriticalSection cs;
    ScalingInfo* pScalingInfo;
};

