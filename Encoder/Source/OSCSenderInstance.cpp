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
              
        default: message.addArgument(OSCArgument("Error"));
        }
    }

    sender->send(message);
}

bool OSCSenderInstance::setOscPath(String path)
{
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

    StringArray followingParameters;
    followingParameters.addTokens(path.substring(oscPath.length()), String(" "), "");
    for (String p : followingParameters)
    {
        if(p.isNotEmpty())
        {
            auto f = std::find_if(escapeStringMap.begin(), escapeStringMap.end(), [&p](const auto& x) { return x.second == p; });
            if (f != escapeStringMap.end())
            {
                realParameters.add(ComplexParameter(f->first));
            }
            else
            {
                // regex matching for user defined scaling option
                std::regex r("\\{[ ]*s([xyzaed])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*\\}");
                std::smatch sm;
                std::string str = p.toStdString();
                if(regex_search(str, sm, r))
                {
                    double lo = std::stod(sm[2]);
                    double hi = std::stod(sm[3]);
                    if(lo == hi)
                        return false;
                    
                    realParameters.add(ComplexParameter(String(sm[0]), sm[1].str(), lo, hi));
                }
                else
                {
                    return false;
                }
            }
        }
    }

    return true;
}

String OSCSenderInstance::getOscPath()
{
    return oscPath;
}

