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



#pragma once
#include "JuceHeader.h"
#include "../../Common/OSCHandler.h"
#include "CustomOscTarget.h"
#include "CustomOscInput.h"
#include "CustomOscReceiver.h"
#include "DistanceEncodingParams.h"
#include "EncoderSettings.h"

#define OSC_HANDLER_ACTION_OSC_RECEIVED "oscrcv"
#define OSC_ADDRESS_MUSESCORE_SSMN "/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED "/icst/ambi/source/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ "/icst/ambi/source/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GAIN "/icst/ambi/source/gain"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED "/icst/ambi/group/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ "/icst/ambi/group/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED "/icst/ambi/sourceindex/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ "/icst/ambi/sourceindex/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_GAIN "/icst/ambi/sourceindex/gain"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_NAME "/icst/ambi/sourceindex/name"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE "/icst/ambi/group/rotate"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_ROTATE_ORIGIN "/icst/ambi/group/rotateorigin"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_STRETCH "/icst/ambi/group/stretch"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_MODE "/icst/ambi/distanceencoding/mode"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_UNITCIRCLE "/icst/ambi/distanceencoding/unitcircle"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_DBUNIT "/icst/ambi/distanceencoding/dbunit"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_DISTANCEATTENUATION "/icst/ambi/distanceencoding/distanceattenuation"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_CENTERCURVE "/icst/ambi/distanceencoding/centercurve"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCEDFACTOR "/icst/ambi/distanceencoding/advancedfactor"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCEDEXPONENT "/icst/ambi/distanceencoding/advancedexponent"
#define ERROR_STRING_INVALID_DISTANCE_ENCODING_PARAMETER String("OSC message with invalid distance encoding parameter received ")
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_STANDARD "/icst/ambi/distanceencoding/standard"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_ADVANCED "/icst/ambi/distanceencoding/advanced"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_EXPONENTIAL "/icst/ambi/distanceencoding/exponential"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_DISTANCEENCODING_INVERSE_PROPORTIONAL "/icst/ambi/distanceencoding/inverseproportional"

class OSCHandlerEncoder : public OSCHandler, public ActionBroadcaster
{
public:
    OSCHandlerEncoder(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler, EncoderSettings* pEncoderSettings, ScalingInfo* pScaling);
    bool handleSpecific(const juce::OSCMessage &message) override;
    bool initSpecific() override;
    bool initialize();
    bool getReceiverStatus(int rowNumber, bool* isInit, bool* hasIncomingData, bool* hasSuccessfulIncomingData, String* errorMessage);
    
private:
    void handleMusescoreSSMNStyle(const OSCMessage& message) const;
    void handleOwnExternStyleAed(const OSCMessage& message) const;
    void handleOwnExternStyleXyz(const OSCMessage& message) const;
    void handleOwnExternStyleGain(const OSCMessage& message) const;
    void handleOwnExternStyleIndexAed(const OSCMessage& message) const;
    void handleOwnExternStyleIndexXyz(const OSCMessage& message) const;
    void handleOwnExternStyleIndexGain(const OSCMessage& message) const;
    void handleOwnExternStyleIndexName(const OSCMessage& message) const;
    void handleOwnExternStyleGroupAed(const OSCMessage& message) const;
    void handleOwnExternStyleGroupXyz(const OSCMessage& message) const;
    void handleOwnExternStyleGroupRotate(const OSCMessage& message) const;
    void handleOwnExternStyleGroupRotateOrigin(const OSCMessage& message) const;
    void handleOwnExternStyleGroupStretch(const OSCMessage& message) const;
    void handleOwnExternStyleDistanceEncoding(const OSCMessage& message) const;
    void handleOwnExternStyleDistanceEncodingStandard(const OSCMessage& message) const;
    void handleOwnExternStyleDistanceEncodingAdvanced(const OSCMessage& message) const;
    void handleOwnExternStyleDistanceEncodingExponential(const OSCMessage& message) const;
    void handleOwnExternStyleDistanceEncodingInverseProportional(const OSCMessage& message) const;
    bool checkAed(double a, double e, double d, String* errorString) const;
    bool checkXyz(double x, double y, double z, String* errorString) const;
    bool checkGain(double gain, String* errorString) const;
    bool checkName(String name, String* errorString) const;
    
    ScalingInfo* pScalingInfo;
    OwnedArray<CustomOscInput>* pCustomOscInput;
    DistanceEncodingParams* pDistanceEncodingParams;
    OwnedArray<CustomOscReceiver> customOscReceivers;
    std::unique_ptr<JavascriptEngine> jsEngine;
    EncoderSettings* pEncoderSettings;
};
