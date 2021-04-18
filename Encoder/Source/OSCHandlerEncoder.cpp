/*
  ==============================================================================

    OSCHandlerEncoder.cpp
    Created: 3 May 2020 10:57:23pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "OSCHandlerEncoder.h"

OSCHandlerEncoder::OSCHandlerEncoder(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler, DistanceEncodingParams* pDistanceEncodingParams, ScalingInfo* pScaling) : OSCHandler(pAmbiPointArray, pStatusMessageHandler), pScalingInfo(pScaling)
{
    this->pDistanceEncodingParams = pDistanceEncodingParams;
}

bool OSCHandlerEncoder::handleSpecific(const OSCMessage &message)
{
    OSCAddressPattern pattern = message.getAddressPattern();
    if (pattern.matches(OSCAddress(OSC_ADDRESS_MUSESCORE_SSMN)))
    {
        handleMusescoreSSMNStyle(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED)))
    {
        handleOwnExternStyleAed(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ)))
    {
        handleOwnExternStyleXyz(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GAIN)))
    {
        handleOwnExternStyleGain(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED)))
    {
        handleOwnExternStyleGroupAed(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ)))
    {
        handleOwnExternStyleGroupXyz(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED)))
    {
        handleOwnExternStyleIndexAed(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ)))
    {
        handleOwnExternStyleIndexXyz(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_GAIN)))
    {
        handleOwnExternStyleIndexGain(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE)))
    {
        handleOwnExternStyleGroupRotate(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE_ORIGIN)))
    {
        handleOwnExternStyleGroupRotateOrigin(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_STRETCH)))
    {
        handleOwnExternStyleGroupStretch(message);
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
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_STANDARD)))
    {
        handleOwnExternStyleDistanceEncodingStandard(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCED)))
    {
        handleOwnExternStyleDistanceEncodingAdvanced(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_EXPONENTIAL)))
    {
        handleOwnExternStyleDistanceEncodingExponential(message);
    }
    else if(pattern.matches(OSCAddress(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_INVERSE_PROPORTIONAL)))
    {
        handleOwnExternStyleDistanceEncodingInverseProportional(message);
    }
    else
    {
        return false;
    }
    
    return true;
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
    
    if(pAmbiPoints->setGain(channel, gain))
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
    if(a < Constants::AzimuthRadMin || a > Constants::AzimuthRadMax)
    {
        *errorString = "OSC-Message Azimuth out of range: " + String(a);
        return false;
    }
    if(e < Constants::ElevationRadMin || e > Constants::ElevationRadMax)
    {
        *errorString = "OSC-Message Elevation out of range: " + String(e);
        return false;
    }
    if (d < Constants::DistanceMin)
    {
        *errorString = "OSC-Message Distance out of range: " + String(d);
        return false;
    }

    return true;
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
       
