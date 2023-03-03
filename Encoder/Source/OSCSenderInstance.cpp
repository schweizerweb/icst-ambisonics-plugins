/*
  ==============================================================================

    OSCSenderInstance.cpp
    Created: 17 Jan 2021 9:49:24pm
    Author:  chris

  ==============================================================================
*/

#include "OSCSenderInstance.h"
#include <string>

OSCSenderInstance::OSCSenderInstance(ScalingInfo* pScaling): CustomOscBase(pScaling), isConnected(false)
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

void OSCSenderInstance::sendMessage(Vector3D<double> absPt, AmbiPoint* pt, int index)
{
    const ScopedLock lock(cs);
    if (!isConnected)
        return;

    String path = String(oscPath);
    double scaler = pScalingInfo->GetScaler();
    
    for (auto& parameter : parametersInPath)
    {
        auto original = parameter->getOriginalString();
        auto replace = parameter->getString(absPt, pt, scaler, index);
        path = path.replace(original, replace);
    }

    OSCMessage message = OSCMessage(OSCAddressPattern(path));
    for (auto& parameter : realParameters)
    {
        message.addArgument(parameter->getOSCArgument(absPt, pt, scaler, index));
    }

    sender->send(message);
}
