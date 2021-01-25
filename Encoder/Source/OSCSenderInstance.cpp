/*
  ==============================================================================

    OSCSenderInstance.cpp
    Created: 17 Jan 2021 9:49:24pm
    Author:  chris

  ==============================================================================
*/

#include "OSCSenderInstance.h"

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
        case Index: path = path.replace(escapeStringMap[Index], String(index)); break;
        case Name: path = path.replace(escapeStringMap[Name], pt->getName()); break;
        case Color: path = path.replace(escapeStringMap[Color], pt->getColor().toDisplayString(true)); break;
        case A: path = path.replace(escapeStringMap[A], String(Constants::RadToGrad(double(pt->getPoint()->getAzimuth())))); break;
        case E: path = path.replace(escapeStringMap[E], String(Constants::RadToGrad(double(pt->getPoint()->getElevation())))); break;
        case D: path = path.replace(escapeStringMap[D], String(pt->getPoint()->getDistance())); break;
        case X: path = path.replace(escapeStringMap[X], String(pt->getPoint()->getX())); break;
        case Y: path = path.replace(escapeStringMap[Y], String(pt->getPoint()->getY())); break;
        case Z: path = path.replace(escapeStringMap[Z], String(pt->getPoint()->getZ())); break;
        case Gain: path = path.replace(escapeStringMap[Gain], String(pt->getGain())); break;
        default: ;
        }
    }

    OSCMessage message = OSCMessage(OSCAddressPattern(path));
    for (auto parameter : realParameters)
    {
        switch (parameter)
        {
        case Index: message.addArgument(OSCArgument(index)); break;
        case Name: message.addArgument(OSCArgument(pt->getName())); break;
        case Color: message.addArgument(OSCArgument(pt->getColor().toDisplayString(true))); break;
        case A: message.addArgument(OSCArgument((float)Constants::RadToGrad(double(pt->getPoint()->getAzimuth())))); break;
        case E: message.addArgument(OSCArgument((float)Constants::RadToGrad(double(pt->getPoint()->getElevation())))); break;
        case D: message.addArgument(OSCArgument((float)pt->getPoint()->getDistance())); break;
        case X: message.addArgument(OSCArgument((float)pt->getPoint()->getX())); break;
        case Y: message.addArgument(OSCArgument((float)pt->getPoint()->getY())); break;
        case Z: message.addArgument(OSCArgument((float)pt->getPoint()->getZ())); break;
        case Gain: message.addArgument(OSCArgument((float)pt->getGain())); break;
        default:;
        }
    }

    sender->send(message);
}

void OSCSenderInstance::setOscPath(String path)
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
            realParameters.add(f->first);
        }
    }
}

