/*
  ==============================================================================

    OSCHandlerEncoder.h
    Created: 3 May 2020 10:57:23pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/OSCHandler.h"
#include "DistanceEncodingParams.h"

#define OSC_ADDRESS_MUSESCORE_SSMN "/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED "/icst/ambi/source/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ "/icst/ambi/source/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GAIN "/icst/ambi/source/gain"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED "/icst/ambi/group/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ "/icst/ambi/group/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED "/icst/ambi/sourceindex/aed"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ "/icst/ambi/sourceindex/xyz"
#define OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_GAIN "/icst/ambi/sourceindex/gain"
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

class OSCHandlerEncoder : public OSCHandler
{
public:
    OSCHandlerEncoder(AmbiSourceSet* pAmbiPointArray, StatusMessageHandler* pStatusMessageHandler, DistanceEncodingParams* pDistanceEncodingParams);
    bool handleSpecific(const juce::OSCMessage &message) override;
    
private:
    void handleMusescoreSSMNStyle(const OSCMessage& message) const;
    void handleOwnExternStyleAed(const OSCMessage& message) const;
    void handleOwnExternStyleXyz(const OSCMessage& message) const;
    void handleOwnExternStyleGain(const OSCMessage& message) const;
    void handleOwnExternStyleIndexAed(const OSCMessage& message) const;
    void handleOwnExternStyleIndexXyz(const OSCMessage& message) const;
    void handleOwnExternStyleIndexGain(const OSCMessage& message) const;
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
    
    DistanceEncodingParams* pDistanceEncodingParams;
};
