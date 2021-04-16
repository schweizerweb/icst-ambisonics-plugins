/*
  ==============================================================================

    OSCSenderInstance.cpp
    Created: 17 Jan 2021 9:49:24pm
    Author:  chris

  ==============================================================================
*/

#include "OSCSenderInstance.h"
#include <string>
#include <regex>

OSCSenderInstance::OSCSenderInstance(ScalingInfo* pScaling): isConnected(false), pScalingInfo(pScaling)
{
    sender.reset(new OSCSender());
}

OSCSenderInstance::~OSCSenderInstance()
{
    sender = nullptr;
}

bool OSCSenderInstance::connect(String host, int port)
{
    const ScopedLock lock(cs);
    bool ok = sender->connect(host, port);
    isConnected = ok;
    return ok;
}

void OSCSenderInstance::disconnect()
{
    const ScopedLock lock(cs);
    isConnected = false;
    sender->disconnect();
}

void OSCSenderInstance::sendMessage(AmbiPoint* pt, int index)
{
    const ScopedLock lock(cs);
    if (!isConnected)
        return;

    String path = String(oscPath);

    for (ParameterType parameter : parametersInPath)
    {
        switch(parameter)
        {
        case Index: path = path.replace(escapeStringMap[Index], String(index + 1)); break;
        case Name: path = path.replace(escapeStringMap[Name], pt->getName()); break;
        case Color: path = path.replace(escapeStringMap[Color], pt->getColor().toDisplayString(true)); break;
        case A: path = path.replace(escapeStringMap[A], String(Constants::RadToGrad(double(pt->getPoint()->getAzimuth())))); break;
        case E: path = path.replace(escapeStringMap[E], String(Constants::RadToGrad(double(pt->getPoint()->getElevation())))); break;
        case D: path = path.replace(escapeStringMap[D], String(pt->getPoint()->getDistance())); break;
        case X: path = path.replace(escapeStringMap[X], String(pt->getPoint()->getX())); break;
        case Y: path = path.replace(escapeStringMap[Y], String(pt->getPoint()->getY())); break;
        case Z: path = path.replace(escapeStringMap[Z], String(pt->getPoint()->getZ())); break;
        case ScaledA: path = path.replace(escapeStringMap[ScaledA], String(pt->getPoint()->getAzimuth() / (2.0 * PI))); break;
        case ScaledE: path = path.replace(escapeStringMap[ScaledE], String(pt->getPoint()->getElevation() / PI + 0.5)); break;
        case ScaledD: path = path.replace(escapeStringMap[ScaledD], String(pt->getPoint()->getDistance() / (MathConstants<double>::sqrt2 * pScalingInfo->GetScaler()))); break;
        case ScaledX: path = path.replace(escapeStringMap[ScaledX], String(pt->getPoint()->getX() / (2.0 * pScalingInfo->GetScaler()) + 0.5)); break;
        case ScaledY: path = path.replace(escapeStringMap[ScaledY], String(pt->getPoint()->getY() / (2.0 * pScalingInfo->GetScaler()) + 0.5)); break;
        case ScaledZ: path = path.replace(escapeStringMap[ScaledZ], String(pt->getPoint()->getZ() / (2.0 * pScalingInfo->GetScaler()) + 0.5)); break;
        case Gain: path = path.replace(escapeStringMap[Gain], String(pt->getGain())); break;
        default: ;
        }
    }

    OSCMessage message = OSCMessage(OSCAddressPattern(path));
    for (auto parameter : realParameters)
    {
        double scaler = pScalingInfo->GetScaler();
        
        switch(parameter.type)
        {
        case Index: message.addArgument(OSCArgument(index + 1)); break;
        case Name: message.addArgument(OSCArgument(pt->getName())); break;
        case Color: message.addArgument(OSCArgument(pt->getColor().toDisplayString(true))); break;
        case A: message.addArgument(OSCArgument(float(Constants::RadToGrad(double(pt->getPoint()->getAzimuth()))))); break;
        case E: message.addArgument(OSCArgument(float(Constants::RadToGrad(double(pt->getPoint()->getElevation()))))); break;
        case D: message.addArgument(OSCArgument(float(pt->getPoint()->getDistance()))); break;
        case X: message.addArgument(OSCArgument(float(pt->getPoint()->getX()))); break;
        case Y: message.addArgument(OSCArgument(float(pt->getPoint()->getY()))); break;
        case Z: message.addArgument(OSCArgument(float(pt->getPoint()->getZ()))); break;
        case ScaledX: message.addArgument(OSCArgument(float(jmap(pt->getPoint()->getX(), -scaler, scaler, parameter.loLim, parameter.hiLim)))); break;
        case ScaledY: message.addArgument(OSCArgument(float(jmap(pt->getPoint()->getY(), -scaler, scaler, parameter.loLim, parameter.hiLim)))); break;
        case ScaledZ: message.addArgument(OSCArgument(float(jmap(pt->getPoint()->getZ(), -scaler, scaler, parameter.loLim, parameter.hiLim)))); break;
        case ScaledA: message.addArgument(OSCArgument(float(jmap(pt->getPoint()->getAzimuth(), double(Constants::AzimuthRadMin), double(Constants::AzimuthRadMax), parameter.loLim, parameter.hiLim)))); break;
        case ScaledE: message.addArgument(OSCArgument(float(jmap(pt->getPoint()->getElevation(), double(Constants::ElevationRadMin), double(Constants::ElevationRadMin), parameter.loLim, parameter.hiLim)))); break;
        case ScaledD: message.addArgument(OSCArgument(float(jmap(pt->getPoint()->getDistance(), -MathConstants<double>::sqrt2 * scaler, MathConstants<double>::sqrt2 * scaler, parameter.loLim, parameter.hiLim)))); break;
        case Gain: message.addArgument(OSCArgument(float(pt->getGain()))); break;
        case DualScaledX: message.addArgument(OSCArgument(dualMap(pt->getPoint()->getX(), scaler, &parameter))); break;
        case DualScaledY: message.addArgument(OSCArgument(dualMap(pt->getPoint()->getY(), scaler, &parameter))); break;
        case DualScaledZ: message.addArgument(OSCArgument(dualMap(pt->getPoint()->getZ(), scaler, &parameter))); break;
        case DualScaledE: message.addArgument(OSCArgument(dualMap(pt->getPoint()->getElevation(), Constants::ElevationRadMax, &parameter))); break;
        default: message.addArgument(OSCArgument("Error"));
        }
    }

    sender->send(message);
}

float OSCSenderInstance::dualMap(double value, double maxValue, ComplexParameter *pParam)
{
    return value <= 0.0
    ? float(jmap(value, -maxValue, 0.0, pParam->loLim, pParam->zero))
    : float(jmap(value, 0.0, maxValue, pParam->zero, pParam->hiLim));
}

bool OSCSenderInstance::setOscPath(String path)
{
    // regex patterns
    std::regex rParameter("[ ]*\\{[ ]*(.+?)[ ]*\\}[ ]*");
    std::regex rTwoValues("s([xyzaed])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
    std::regex rThreeValues("s([xyze])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
    
    parametersInPath.clear();
    realParameters.clear();

    oscPath = path.upToFirstOccurrenceOf(" ", false, false);

    for (auto pair : escapeStringMap) 
    {
        if(oscPath.containsIgnoreCase(pair.second))
        {
            parametersInPath.add(pair.first);
        }
    }

    std::smatch parameterMatch;
    std::string parameterString = path.substring(oscPath.length()).toStdString();
    
    while(regex_search(parameterString, parameterMatch, rParameter))
    {
        auto str = parameterMatch[1].str();
        switch(str.substr()[0])
        {
            case 'a': realParameters.add(ComplexParameter(ParameterType::A)); break;
            case 'e': realParameters.add(ComplexParameter(ParameterType::E)); break;
            case 'd': realParameters.add(ComplexParameter(ParameterType::D)); break;
            case 'x': realParameters.add(ComplexParameter(ParameterType::X)); break;
            case 'y': realParameters.add(ComplexParameter(ParameterType::Y)); break;
            case 'z': realParameters.add(ComplexParameter(ParameterType::Z)); break;
            case 'i': realParameters.add(ComplexParameter(ParameterType::Index)); break;
            case 'n': realParameters.add(ComplexParameter(ParameterType::Name)); break;
            case 'c': realParameters.add(ComplexParameter(ParameterType::Color)); break;
            case 'g': realParameters.add(ComplexParameter(ParameterType::Gain)); break;
            case 's':
                if(str.length() == 2)
                {
                    switch(str.substr()[1])
                    {
                        case 'a': realParameters.add(ComplexParameter(ParameterType::ScaledA)); break;
                        case 'e': realParameters.add(ComplexParameter(ParameterType::ScaledE)); break;
                        case 'd': realParameters.add(ComplexParameter(ParameterType::ScaledD)); break;
                        case 'x': realParameters.add(ComplexParameter(ParameterType::ScaledX)); break;
                        case 'y': realParameters.add(ComplexParameter(ParameterType::ScaledY)); break;
                        case 'z': realParameters.add(ComplexParameter(ParameterType::ScaledZ)); break;
                        default: return false;
                    }
                }
                else
                {
                    std::smatch sm;
                        
                    if(regex_search(str, sm, rTwoValues))
                    {
                        double lo = std::stod(sm[2]);
                        double hi = std::stod(sm[3]);
                        if(lo == hi)
                            return false;
                            
                        realParameters.add(ComplexParameter(String(sm[0]), sm[1].str(), lo, hi));
                    }
                    else if(regex_search(str, sm, rThreeValues))
                    {
                        double lo = std::stod(sm[2]);
                        double zero = std::stod(sm[3]);
                        double hi = std::stod(sm[4]);
                        if(lo == hi && lo == zero)
                            return false;
                            
                        realParameters.add(ComplexParameter(String(sm[0]), sm[1].str(), lo, hi, zero));
                    }
                    else
                    {
                        return false;
                    }
                }
                break;
            default:
                return false;
        }
        
        parameterString = parameterMatch.suffix();
    }

    return true;
}

String OSCSenderInstance::getOscPath()
{
    return oscPath;
}

