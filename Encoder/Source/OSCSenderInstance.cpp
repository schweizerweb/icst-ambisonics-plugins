/*
  ==============================================================================

    OSCSenderInstance.cpp
    Created: 17 Jan 2021 9:49:24pm
    Author:  chris

  ==============================================================================
*/

#include "OSCSenderInstance.h"

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
        case SA: path = path.replace(escapeStringMap[SA], String(pt->getPoint()->getAzimuth() / (2.0 * PI))); break;
        case SE: path = path.replace(escapeStringMap[SE], String(pt->getPoint()->getElevation() / PI + 0.5)); break;
        case SD: path = path.replace(escapeStringMap[SD], String(pt->getPoint()->getDistance() / (MathConstants<double>::sqrt2 * pScalingInfo->GetScaler()))); break;
        case SX: path = path.replace(escapeStringMap[SX], String(pt->getPoint()->getX() / (2.0 * pScalingInfo->GetScaler()) + 0.5)); break;
        case SY: path = path.replace(escapeStringMap[SY], String(pt->getPoint()->getY() / (2.0 * pScalingInfo->GetScaler()) + 0.5)); break;
        case SZ: path = path.replace(escapeStringMap[SZ], String(pt->getPoint()->getZ() / (2.0 * pScalingInfo->GetScaler()) + 0.5)); break;
        case Gain: path = path.replace(escapeStringMap[Gain], String(pt->getGain())); break;
        default: ;
        }
    }

    OSCMessage message = OSCMessage(OSCAddressPattern(path));
    for (auto parameter : realParameters)
    {
        switch (parameter)
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
        case SA: message.addArgument(OSCArgument(float(pt->getPoint()->getAzimuth() / (2.0 * PI)))); break;
        case SE: message.addArgument(OSCArgument(float(pt->getPoint()->getElevation() / PI + 0.5))); break;
        case SD: message.addArgument(OSCArgument(float(pt->getPoint()->getDistance() / (MathConstants<double>::sqrt2 * pScalingInfo->GetScaler())))); break;
        case SX: message.addArgument(OSCArgument(float(pt->getPoint()->getX() / (2.0 * pScalingInfo->GetScaler()) + 0.5))); break;
        case SY: message.addArgument(OSCArgument(float(pt->getPoint()->getY() / (2.0 * pScalingInfo->GetScaler()) + 0.5))); break;
        case SZ: message.addArgument(OSCArgument(float(pt->getPoint()->getZ() / (2.0 * pScalingInfo->GetScaler()) + 0.5))); break;
        case Gain: message.addArgument(OSCArgument(float(pt->getGain()))); break;
        default:;
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
                realParameters.add(f->first);
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

String OSCSenderInstance::getOscPath()
{
    return oscPath;
}

