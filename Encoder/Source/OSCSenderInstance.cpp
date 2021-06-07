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
    double scaler = pScalingInfo->GetScaler();
    
    for (UserDefinedParameter parameter : parametersInPath)
    {
        auto original = parameter.getOriginalString();
        auto replace = parameter.getString(pt, scaler, index);
        path = path.replace(original, replace);
    }

    OSCMessage message = OSCMessage(OSCAddressPattern(path));
    for (auto parameter : realParameters)
    {
        message.addArgument(parameter.getOSCArgument(pt, scaler, index));
    }

    sender->send(message);
}

bool OSCSenderInstance::setOscPath(String path)
{
    oscPath = path.upToFirstOccurrenceOf(" ", false, false);
    
    std::string oscPathString = oscPath.toStdString();
    std::string parameterString = path.substring(oscPath.length()).toStdString();
    
    return analyzeString(oscPathString, &parametersInPath) && analyzeString(parameterString, &realParameters);
}

bool OSCSenderInstance::analyzeString(std::string parameterString, Array<UserDefinedParameter> *pArray)
{
    // regex patterns
    std::regex rParameter("[ ]*\\{[ ]*(.+?)[ ]*\\}[ ]*");
    std::regex rTwoValues("s([xyzaed])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
    std::regex rThreeValues("s([xyze])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
    std::smatch parameterMatch;
    
    pArray->clear();

    while(regex_search(parameterString, parameterMatch, rParameter))
    {
        auto fullString = parameterMatch[0].str();
        auto userString = parameterMatch[1].str();
        if(userString.length() == 1)
        {
            switch(userString.substr()[0])
            {
                case 'a': pArray->add(UserDefinedParameter(fullString, ParameterType::A)); break;
                case 'e': pArray->add(UserDefinedParameter(fullString, ParameterType::E)); break;
                case 'd': pArray->add(UserDefinedParameter(fullString, ParameterType::D)); break;
                case 'x': pArray->add(UserDefinedParameter(fullString, ParameterType::X)); break;
                case 'y': pArray->add(UserDefinedParameter(fullString, ParameterType::Y)); break;
                case 'z': pArray->add(UserDefinedParameter(fullString, ParameterType::Z)); break;
                case 'i': pArray->add(UserDefinedParameter(fullString, ParameterType::Index)); break;
                case 'n': pArray->add(UserDefinedParameter(fullString, ParameterType::Name)); break;
                case 'c': pArray->add(UserDefinedParameter(fullString, ParameterType::Color)); break;
                case 'g': pArray->add(UserDefinedParameter(fullString, ParameterType::Gain)); break;
            }
        }
        else if(userString.substr()[0]== 's')
        {
            if(userString.length() == 2)
            {
                switch(userString.substr()[1])
                {
                    case 'a': pArray->add(UserDefinedParameter(fullString, ParameterType::ScaledA)); break;
                    case 'e': pArray->add(UserDefinedParameter(fullString, ParameterType::ScaledE)); break;
                    case 'd': pArray->add(UserDefinedParameter(fullString, ParameterType::ScaledD)); break;
                    case 'x': pArray->add(UserDefinedParameter(fullString, ParameterType::ScaledX)); break;
                    case 'y': pArray->add(UserDefinedParameter(fullString, ParameterType::ScaledY)); break;
                    case 'z': pArray->add(UserDefinedParameter(fullString, ParameterType::ScaledZ)); break;
                        default: return false;
                }
            }
            else
            {
                std::smatch sm;
                        
                if(regex_search(userString, sm, rThreeValues))
                {
                    double lo = std::stod(sm[2]);
                    double zero = std::stod(sm[3]);
                    double hi = std::stod(sm[4]);
                    if(lo == hi && lo == zero)
                        return false;
                            
                    pArray->add(UserDefinedParameter(fullString, sm[1].str(), lo, hi, zero));
                }
                else if(regex_search(userString, sm, rTwoValues))
                {
                    double lo = std::stod(sm[2]);
                    double hi = std::stod(sm[3]);
                    if(lo == hi)
                        return false;
                        
                    pArray->add(UserDefinedParameter(fullString, sm[1].str(), lo, hi));
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
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

