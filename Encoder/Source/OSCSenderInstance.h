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

    enum ParameterType { Index, Name, Color, A, E, D, X, Y, Z, ScaledA, ScaledE, ScaledD, ScaledX, ScaledY, ScaledZ, Gain, DualScaledE, DualScaledX, DualScaledY, DualScaledZ };
    class UserDefinedParameter
    {
    public:
        UserDefinedParameter(String originalString, std::string typeString, double lo, double hi) : originalString(originalString), loLim(lo), hiLim(hi)
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
        
        UserDefinedParameter(String originalString, std::string typeString, double lo, double hi, double zero) : originalString(originalString), loLim(lo), hiLim(hi), zero(zero)
        {
            switch(typeString.substr()[0])
            {
                case 'x': type = ParameterType::DualScaledX; break;
                case 'y': type = ParameterType::DualScaledY; break;
                case 'z': type = ParameterType::DualScaledZ; break;
                case 'e': type = ParameterType::DualScaledE; break;
            }
        }
        
        UserDefinedParameter(String originalString, ParameterType t): originalString(originalString)
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
        
        OSCArgument getOSCArgument(AmbiPoint* pt, double scaler, int index)
        {
            switch(type)
            {
                case Index:
                    return OSCArgument(index + 1);
                    
                case Name:
                case Color:
                    return OSCArgument(getString(pt, scaler, index));
            
                case A:
                case E:
                case D:
                case X:
                case Y:
                case Z:
                case ScaledX:
                case ScaledY:
                case ScaledZ:
                case ScaledA:
                case ScaledE:
                case ScaledD:
                case Gain:
                case DualScaledX:
                case DualScaledY:
                case DualScaledZ:
                case DualScaledE:
                    return OSCArgument(getValue(pt, scaler, index));
                    
                default:
                    return OSCArgument("Error");
            }
        }
        
        String getString(AmbiPoint* pt, double scaler, int index)
        {
            switch(type)
            {
                case Index: return String(index + 1);
                case Name: return pt->getName();
                case Color: return pt->getColor().toDisplayString(true);
                    
                case A:
                case E:
                case D:
                case X:
                case Y:
                case Z:
                case ScaledX:
                case ScaledY:
                case ScaledZ:
                case ScaledA:
                case ScaledE:
                case ScaledD:
                case Gain:
                case DualScaledX:
                case DualScaledY:
                case DualScaledZ:
                case DualScaledE:
                    return String(getValue(pt, scaler, index));
                    
                default: return "Error";
            }
        }
        
        String getOriginalString()
        {
            return originalString;
        }
        
    private:
        float getValue(AmbiPoint* pt, double scaler, int index)
        {
            switch(type)
            {
            case A: return float(Constants::RadToGrad(double(pt->getPoint()->getAzimuth())));
            case E: return float(Constants::RadToGrad(double(pt->getPoint()->getElevation())));
            case D: return float(pt->getPoint()->getDistance());
            case X: return float(pt->getPoint()->getX());
            case Y: return float(pt->getPoint()->getY());
            case Z: return float(pt->getPoint()->getZ());
            case ScaledX: return float(jmap(pt->getPoint()->getX(), -scaler, scaler, loLim, hiLim));
            case ScaledY: return float(jmap(pt->getPoint()->getY(), -scaler, scaler, loLim, hiLim));
            case ScaledZ: return float(jmap(pt->getPoint()->getZ(), -scaler, scaler, loLim, hiLim));
            case ScaledA: return float(jmap(pt->getPoint()->getAzimuth(), double(Constants::AzimuthRadMin), double(Constants::AzimuthRadMax), loLim, hiLim));
            case ScaledE: return float(jmap(pt->getPoint()->getElevation(), double(Constants::ElevationRadMin), double(Constants::ElevationRadMin), loLim, hiLim));
            case ScaledD: return float(jmap(pt->getPoint()->getDistance(), -MathConstants<double>::sqrt2 * scaler, MathConstants<double>::sqrt2 * scaler, loLim, hiLim));
            case Gain: return float(pt->getGain());
            case DualScaledX: return dualMap(pt->getPoint()->getX(), scaler);
            case DualScaledY: return dualMap(pt->getPoint()->getY(), scaler);
            case DualScaledZ: return dualMap(pt->getPoint()->getZ(), scaler);
            case DualScaledE: return dualMap(pt->getPoint()->getElevation(), Constants::ElevationRadMax);
            
            case Index:
            case Name:
            case Color:
            default:
                throw;
            }
        }
        
        float dualMap(double value, double maxValue)
        {
            return value <= 0.0
            ? float(jmap(value, -maxValue, 0.0, loLim, zero))
            : float(jmap(value, 0.0, maxValue, zero, hiLim));
        }
        
        String originalString;
        ParameterType type;
        double loLim;
        double hiLim;
        double zero;
    };
    
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

