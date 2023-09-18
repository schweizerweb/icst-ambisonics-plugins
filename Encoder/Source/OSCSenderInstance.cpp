/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
