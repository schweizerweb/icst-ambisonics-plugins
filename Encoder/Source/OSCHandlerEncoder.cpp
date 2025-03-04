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



#include "OSCHandlerEncoder.h"
#include "../../Common/MathHelper.h"

OSCHandlerEncoder::OSCHandlerEncoder(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* _pStatusMessageHandler, EncoderSettings* _pEncoderSettings, ScalingInfo* pScaling) : OSCHandler(pAmbiPointArray, _pStatusMessageHandler), pScalingInfo(pScaling), pEncoderSettings(_pEncoderSettings)
{
    pCustomOscInput = &pEncoderSettings->customOscInput;
    pDistanceEncodingParams = &pEncoderSettings->distanceEncodingParams;
    jsEngine.reset(new JavascriptEngine());
}

bool OSCHandlerEncoder::initialize()
{
    setVerbosity(true, !pEncoderSettings->hideWarnings);
    if (pEncoderSettings->oscReceiveFlag)
    {
        if (!start(pEncoderSettings->oscReceivePort))
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC Receiver on Port " + String(pEncoderSettings->oscReceivePort));
            pEncoderSettings->oscReceiveFlag = false;
            return false;
        }
    }
    else
    {
        stop();
    }
    
    return true;
}

bool OSCHandlerEncoder::getReceiverStatus(int rowNumber, bool* isInit, bool* hasIncomingData, bool* hasSuccessfulIncomingData, String* errorMessage)
{
    *isInit = false;
    *hasIncomingData = false;
    
    int iReceiver = -1;
    for(int i = 0; i <= rowNumber; i++)
    {
        if(pEncoderSettings->customOscInput[i]->enabledFlag)
            iReceiver++;
    }
    
    if(iReceiver >= 0 && iReceiver < customOscReceivers.size())
    {
        *isInit = customOscReceivers[iReceiver]->getInitFlag();
        *hasIncomingData = customOscReceivers[iReceiver]->getLastRxTimestamp() > Time::getMillisecondCounter() - 1000;
        *hasSuccessfulIncomingData = customOscReceivers[iReceiver]->getLastSuccessfulRxTimestamp() > Time::getMillisecondCounter() - 1000;
        *errorMessage = customOscReceivers[iReceiver]->getErrorMessage();
    }
    
    return true;
}

bool OSCHandlerEncoder::initSpecific()
{
    customOscReceivers.clear();
    for(auto& c : *pCustomOscInput)
    {
        if(c->enabledFlag)
        {
            auto r = new CustomOscReceiver(c, pScalingInfo);
            if(!r->getErrorMessage().isEmpty())
            {
                AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "OSC error", "Error initializing custom OSC receiver (" + String(pCustomOscInput->indexOf(c)) + "): " + r->getErrorMessage());
            }
            
            // add in any case
            customOscReceivers.add(r);
        }
    }
    
    return true;
}

bool OSCHandlerEncoder::handleSpecific(const OSCMessage &message)
{
    OSCAddressPattern pattern = message.getAddressPattern();
    
    // first check for custom osc pattern match
    bool hasMatch = false;
    if(!pattern.containsWildcards()) {
        OSCAddress address(pattern.toString());
        
        for(auto& r : customOscReceivers)
        {
            if(r->matchesPattern(&address))
            {
                hasMatch = true;
                if(r->handleMessage(pAmbiPoints, &message))
                {
                    reportSuccess(&message);
                }
                else
                {
                    if(!r->getErrorMessage().isEmpty())
                    {
                        reportError(r->getErrorMessage(), &message);
                    }
                }
            }
        }
    }
    
    if(!hasMatch && pEncoderSettings->oscHandleStandardFormatFlag)
    {
        if (pattern.matches(OSCAddress(OSC_ADDRESS_MUSESCORE_SSMN)))
        {
            handleMusescoreSSMNStyle(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED)))
        {
            handleOwnExternStyleAed(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ)))
        {
            handleOwnExternStyleXyz(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GAIN)))
        {
            handleOwnExternStyleGain(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED)))
        {
            handleOwnExternStyleGroupAed(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ)))
        {
            handleOwnExternStyleGroupXyz(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED)))
        {
            handleOwnExternStyleIndexAed(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ)))
        {
            handleOwnExternStyleIndexXyz(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_GAIN)))
        {
            handleOwnExternStyleIndexGain(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_NAME)))
        {
            handleOwnExternStyleIndexName(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE)))
        {
            handleOwnExternStyleGroupRotate(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_SETROTATION_QUATERNION)))
        {
            handleOwnExternStyleGroupSetRotationQuaternion(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_SETROTATION_EULER)))
        {
            handleOwnExternStyleGroupSetRotationEuler(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE_ORIGIN)))
        {
            handleOwnExternStyleGroupRotateOrigin(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_STRETCH)))
        {
            handleOwnExternStyleGroupStretch(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_SETSTRETCH)))
        {
            handleOwnExternStyleGroupSetStretch(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_MODE))
                || pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_UNITCIRCLE))
                || pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_DBUNIT))
                || pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_DISTANCEATTENUATION))
                || pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_CENTERCURVE))
                || pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCEDFACTOR))
                || pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCEDEXPONENT)))
        {
            handleOwnExternStyleDistanceEncoding(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_STANDARD)))
        {
            handleOwnExternStyleDistanceEncodingStandard(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCED)))
        {
            handleOwnExternStyleDistanceEncodingAdvanced(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_EXPONENTIAL)))
        {
            handleOwnExternStyleDistanceEncodingExponential(message);
            hasMatch = true;
        }
        else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_INVERSE_PROPORTIONAL)))
        {
            handleOwnExternStyleDistanceEncodingInverseProportional(message);
            hasMatch = true;
        }
    }
    
    if(hasMatch)
        sendActionMessage(OSC_HANDLER_ACTION_OSC_RECEIVED);
    
    return hasMatch;
}

void OSCHandlerEncoder::handleMusescoreSSMNStyle(const OSCMessage& message) const
{
    bool valid =
        message.size() == 5
        && message[0].isInt32()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32())
        && message[4].isInt32();
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)", &message);
        return;
    }

    int channel = message[0].getInt32();
    double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
    double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double d = GetIntOrFloat(&message[3]);
    int channelCheck = message[4].getInt32();
    String errorString;
    if (!checkAed(a, e, d, &errorString))
    {
        reportError(errorString, &message);
        return;
    }
    
    if (channel == channelCheck)
    {
        if (pAmbiPoints->setChannelNameAED(String(channel), a, e, d))
        {
            reportSuccess(&message);

        }
        else
        {
            reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
        }
    }
    else
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(MuseScoreSSMN style)", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleAed(const OSCMessage& message) const
{
    bool valid =
        message.size() == 4
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(AED)", &message);
        return;
    }

    String channelString = message[0].getString();
    double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
    double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double d = GetIntOrFloat(&message[3]);
    String errorMessage;
    if(!checkAed(a, e, d, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if(pAmbiPoints->setChannelNameAED(channelString, a, e, d))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleXyz(const OSCMessage& message) const
{
    bool valid =
        message.size() == 4
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(XYZ)", &message);
        return;
    }

    String channelString = message[0].getString();
    double x = GetIntOrFloat(&message[1]);
    double y = GetIntOrFloat(&message[2]);
    double z = GetIntOrFloat(&message[3]);
    String errorMessage;
    if (!checkXyz(x, y, z, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if (pAmbiPoints->setChannelNameXYZ(channelString, x, y, z))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGain(const OSCMessage& message) const
{
    bool valid =
        message.size() == 2
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(Gain)", &message);
        return;
    }

    String channelString = message[0].getString();
    double gain = GetIntOrFloat(&message[1]);
    String errorMessage;
    if (!checkGain(gain, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if (pAmbiPoints->setChannelNameGain(channelString, gain))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + channelString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleIndexAed(const OSCMessage& message) const
{
    bool valid =
        message.size() == 4
        && message[0].isInt32()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if(!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(AED index style)", &message);
        return;
    }

    int channel = message[0].getInt32();
    double a = Constants::GradToRad(Constants::NormalizeAzimuthGrad(int(GetIntOrFloat(&message[1]))));
    double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double d = GetIntOrFloat(&message[3]);
    String errorMessage;
    if (!checkAed(a, e, d, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if(pAmbiPoints->setChannelAED(channel-1, a, e, d))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleIndexXyz(const OSCMessage& message) const
{
    bool valid =
        message.size() == 4
        && message[0].isInt32()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(XYZ index style)", &message);
        return;
    }

    int channel = message[0].getInt32();
    double x = GetIntOrFloat(&message[1]);
    double y = GetIntOrFloat(&message[2]);
    double z = GetIntOrFloat(&message[3]);
    String errorMessage;
    if (!checkXyz(x, y, z, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if(pAmbiPoints->setChannelXYZ(channel-1, x, y, z))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleIndexGain(const OSCMessage &message) const
{
    bool valid =
        message.size() == 2
        && message[0].isInt32()
        && (message[1].isInt32() || message[1].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(Gain index style)", &message);
        return;
    }

    int channel = message[0].getInt32();
    double gain = GetIntOrFloat(&message[1]);
    String errorMessage;
    if (!checkGain(gain, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if(pAmbiPoints->setGain(channel-1, gain))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleIndexName(const OSCMessage &message) const
{
    bool valid =
        message.size() == 2
        && message[0].isInt32()
        && (message[1].isString());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(Name index style)", &message);
        return;
    }

    int channel = message[0].getInt32();
    String name = message[1].getString();
    String errorMessage;
    if (!checkName(name, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if(pAmbiPoints->setChannelName(channel-1, name))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + String(channel) + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupAed(const OSCMessage& message) const
{
    bool valid =
        message.size() == 5
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32())
        && message[4].isInt32();
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(group AED)", &message);
        return;
    }

    String groupString = message[0].getString();
    double a = Constants::GradToRad(GetIntOrFloat(&message[1]));
    double e = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double d = GetIntOrFloat(&message[3]);
    bool movePoints = message[4].getInt32();
    String errorMessage;
    if (!checkAed(a, e, d, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if (pAmbiPoints->setGroupAed(groupString, a, e, d, movePoints))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupXyz(const OSCMessage& message) const
{
    bool valid =
        message.size() == 5
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32())
        && message[4].isInt32();
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(group XYZ)", &message);
        return;
    }

    String groupString = message[0].getString();
    double x = GetIntOrFloat(&message[1]);
    double y = GetIntOrFloat(&message[2]);
    double z = GetIntOrFloat(&message[3]);
    bool movePoints = message[4].getInt32();
    String errorMessage;
    if (!checkXyz(x, y, z, &errorMessage))
    {
        reportError(errorMessage, &message);
        return;
    }
    
    if (pAmbiPoints->setGroupXyz(groupString, x, y, z, movePoints))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupRotate(const OSCMessage& message) const
{
    bool valid =
        message.size() == 4
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(group rotation)", &message);
        return;
    }

    String groupString = message[0].getString();
    double x = Constants::GradToRad(GetIntOrFloat(&message[1]));
    double y = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double z = Constants::GradToRad(GetIntOrFloat(&message[3]));
    
    if (pAmbiPoints->rotateGroup(groupString, x, y, z))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupSetRotationEuler(const OSCMessage& message) const
{
    bool valid =
        message.size() == 4
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(set group rot. euler)", &message);
        return;
    }

    String groupString = message[0].getString();
    double x = Constants::GradToRad(GetIntOrFloat(&message[1]));
    double y = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double z = Constants::GradToRad(GetIntOrFloat(&message[3]));
    
    if (pAmbiPoints->setGroupRotation(groupString, MathHelper::EulerToQuaternion(x, y, z)))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupSetRotationQuaternion(const OSCMessage& message) const
{
    bool valid =
        message.size() == 5
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32())
        && (message[4].isInt32() || message[4].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(set group rot. quat)", &message);
        return;
    }

    String groupString = message[0].getString();
    double q1 = Constants::GradToRad(GetIntOrFloat(&message[1]));
    double q2 = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double q3 = Constants::GradToRad(GetIntOrFloat(&message[3]));
    double q4 = Constants::GradToRad(GetIntOrFloat(&message[4]));
    
    if (pAmbiPoints->setGroupRotation(groupString, Quaternion<double>(q1, q2, q3, q4)))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupRotateOrigin(const OSCMessage& message) const
{
    bool valid =
        message.size() == 5
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32())
        && message[4].isInt32();
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(group origin rotation)", &message);
        return;
    }

    String groupString = message[0].getString();
    double x = Constants::GradToRad(GetIntOrFloat(&message[1]));
    double y = Constants::GradToRad(GetIntOrFloat(&message[2]));
    double z = Constants::GradToRad(GetIntOrFloat(&message[3]));
    bool movePoints = message[4].getInt32();
    
    if (pAmbiPoints->rotateGroupAroundOrigin(groupString, x, y, z, movePoints))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupStretch(const OSCMessage& message) const
{
    bool valid =
        message.size() == 2
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(group stretch)", &message);
        return;
    }

    String groupString = message[0].getString();
    double x = GetIntOrFloat(&message[1]);
    
    if (pAmbiPoints->stretchGroup(groupString, x))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleGroupSetStretch(const OSCMessage& message) const
{
    bool valid =
        message.size() == 2
        && message[0].isString()
        && (message[1].isInt32() || message[1].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(set group stretch)", &message);
        return;
    }

    String groupString = message[0].getString();
    double x = GetIntOrFloat(&message[1]);
    
    if (pAmbiPoints->setGroupStretch(groupString, x))
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_NONEXISTING_TARGET + "(" + groupString + ")", &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleDistanceEncoding(const OSCMessage &message) const
{
    bool valid =
        message.size() == 1
        && (message[0].isInt32() || message[0].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(distance encoding)", &message);
        return;
    }

    bool ok = false;
    OSCAddressPattern pattern = message.getAddressPattern();
    if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_MODE))
    {
        ok = pDistanceEncodingParams->setEncodingMode((EncoderConstants::EncodingMode)GetIntOrFloat(&message[0]));
    }
    else if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_UNITCIRCLE))
    {
        ok = pDistanceEncodingParams->setUnitCircleRadius((float)GetIntOrFloat(&message[0]));
    }
    else if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_DBUNIT))
    {
        ok = pDistanceEncodingParams->setDbUnit((float)GetIntOrFloat(&message[0]));
    }
    else if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_DISTANCEATTENUATION))
    {
        ok = pDistanceEncodingParams->setInverseProportionalDistanceAttenuation((float)GetIntOrFloat(&message[0]));
    }
    else if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_CENTERCURVE))
    {
        ok = pDistanceEncodingParams->setCenterCurve((float)GetIntOrFloat(&message[0]));
    }
    else if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCEDFACTOR))
    {
        ok = pDistanceEncodingParams->setAdvancedFactor((float)GetIntOrFloat(&message[0]));
    }
    else if(pattern.matches(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCEDEXPONENT))
    {
        ok = pDistanceEncodingParams->setAdvancedExponent((float)GetIntOrFloat(&message[0]));
    }
    
    if (ok)
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_INVALID_DISTANCE_ENCODING_PARAMETER, &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleDistanceEncodingStandard(const OSCMessage &message) const
{
    bool valid =
        message.size() == 1
        && (message[0].isInt32() || message[0].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(distance encoding)", &message);
        return;
    }

    bool ok = false;
    ok = pDistanceEncodingParams->setEncodingMode(EncoderConstants::EncodingMode::Standard);
    ok &= pDistanceEncodingParams->setUnitCircleRadius((float)GetIntOrFloat(&message[0]));
    
    if (ok)
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_INVALID_DISTANCE_ENCODING_PARAMETER, &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleDistanceEncodingAdvanced(const OSCMessage &message) const
{
    bool valid =
        message.size() == 3
        && (message[0].isInt32() || message[0].isFloat32())
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(distance encoding)", &message);
        return;
    }

    bool ok = false;
    ok = pDistanceEncodingParams->setEncodingMode(EncoderConstants::EncodingMode::Advanced);
    ok &= pDistanceEncodingParams->setUnitCircleRadius((float)GetIntOrFloat(&message[0]));
    ok &= pDistanceEncodingParams->setAdvancedFactor((float)GetIntOrFloat(&message[1]));
    ok &= pDistanceEncodingParams->setAdvancedExponent((float)GetIntOrFloat(&message[2]));
    
    if (ok)
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_INVALID_DISTANCE_ENCODING_PARAMETER, &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleDistanceEncodingExponential(const OSCMessage &message) const
{
    bool valid =
        message.size() == 3
        && (message[0].isInt32() || message[0].isFloat32())
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(distance encoding)", &message);
        return;
    }

    bool ok = false;
    ok = pDistanceEncodingParams->setEncodingMode(EncoderConstants::EncodingMode::Exponential);
    ok &= pDistanceEncodingParams->setUnitCircleRadius((float)GetIntOrFloat(&message[0]));
    ok &= pDistanceEncodingParams->setDbUnit((float)GetIntOrFloat(&message[1]));
    ok &= pDistanceEncodingParams->setCenterCurve((float)GetIntOrFloat(&message[2]));
    
    if (ok)
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_INVALID_DISTANCE_ENCODING_PARAMETER, &message);
    }
}

void OSCHandlerEncoder::handleOwnExternStyleDistanceEncodingInverseProportional(const OSCMessage &message) const
{
    bool valid =
        message.size() == 4
        && (message[0].isInt32() || message[0].isFloat32())
        && (message[1].isInt32() || message[1].isFloat32())
        && (message[2].isInt32() || message[2].isFloat32())
        && (message[3].isInt32() || message[3].isFloat32());
    if (!valid)
    {
        reportError(ERROR_STRING_MALFORMATTED_OSC + "(distance encoding)", &message);
        return;
    }

    bool ok = false;
    ok = pDistanceEncodingParams->setEncodingMode(EncoderConstants::EncodingMode::InverseProportional);
    ok &= pDistanceEncodingParams->setUnitCircleRadius((float)GetIntOrFloat(&message[0]));
    ok &= pDistanceEncodingParams->setDbUnit((float)GetIntOrFloat(&message[1]));
    ok &= pDistanceEncodingParams->setCenterCurve((float)GetIntOrFloat(&message[2]));
    ok &= pDistanceEncodingParams->setInverseProportionalDistanceAttenuation((float)GetIntOrFloat(&message[3]));
    
    if (ok)
    {
        reportSuccess(&message);
    }
    else
    {
        reportError(ERROR_STRING_INVALID_DISTANCE_ENCODING_PARAMETER, &message);
    }
}

bool OSCHandlerEncoder::checkAed(double a, double e, double d, String* errorString) const
{
    double x = d * cos(e) * cos(a);
    double y = d * cos(e) * sin(a);
    double z = d * sin(e);

    return checkXyz(x, y, z, errorString);
}

bool OSCHandlerEncoder::checkXyz(double x, double y, double z, String* errorString) const
{
    if (x < pScalingInfo->CartesianMin() || x > pScalingInfo->CartesianMax())
    {
        *errorString = "OSC-Message X out of range: " + String(x);
        return false;
    }
    if (y < pScalingInfo->CartesianMin() || y > pScalingInfo->CartesianMax())
    {
        *errorString = "OSC-Message Y out of range: " + String(y);
        return false;
    }
    if (z < pScalingInfo->CartesianMin() || z > pScalingInfo->CartesianMax())
    {
        *errorString = "OSC-Message Z out of range: " + String(z);
        return false;
    }

    return true;
}

bool OSCHandlerEncoder::checkGain(double gain, String *errorString) const
{
    if(gain < Constants::GainDbMin || gain > Constants::GainDbMax)
    {
        *errorString = "OSC-Message Gain out of range: " + String(gain);
        return false;
    }
    
    return true;
}
       
bool OSCHandlerEncoder::checkName(String name, String *errorString) const
{
    if(name.length() < 1)
    {
        *errorString = "OSC-Message name not specified";
        return false;
    }
    
    return true;
}
       
