/*
  ==============================================================================

    DistanceEncodingParams.h
    Created: 22 Nov 2019 1:32:33pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../../Common/Constants.h"
#include "EncoderConstants.h"

#define DEFAULT_UNIT_CIRCLE_SIZE	    0.1f
#define DEFAULT_DB_UNIT         	    10.0f
#define DEFAULT_DISTANCE_ATTENUATION	5.0f
#define DEFAULT_CENTER_CURVE        	0.02f
#define DEFAULT_ADVANCED_FACTOR  	    1.0f
#define DEFAULT_ADVANCED_EXPONENT  	    1.0f
#define DEFAULT_DISTANCE_ENCODING_MODE  EncoderConstants::Standard

class DistanceEncodingParams : AudioProcessorParameter::Listener, public ChangeBroadcaster
{
public:
    
	DistanceEncodingParams():
        encodingMode(nullptr),
        unitCircleRadius(nullptr),
        dbUnit(nullptr),
        inverseProportionalDistanceAttenuation(nullptr),
        centerCurve(nullptr),
        advancedFactor(nullptr),
        advancedExponent(nullptr),
        localEncodingMode(DEFAULT_DISTANCE_ENCODING_MODE),
        localUnitCircleRadius(DEFAULT_UNIT_CIRCLE_SIZE),
        localDbUnit(DEFAULT_DB_UNIT),
        localInverseProportionalDistanceAttenuation(DEFAULT_DISTANCE_ATTENUATION),
        localCenterCurve(DEFAULT_CENTER_CURVE),
        localAdvancedFactor(DEFAULT_ADVANCED_FACTOR),
        localAdvancedExponent(DEFAULT_ADVANCED_EXPONENT)
    {
    }

    void initialize(AudioProcessor* pProcessor){
        encodingMode = new AudioParameterChoice("EncodingMode", "Encoding Mode",
                                                EncoderConstants::encodingModeStrings, localEncodingMode, "Distance Encoding: Mode");
        
        unitCircleRadius = new AudioParameterFloat("UnitCircle", "Unit Circle", NormalisableRange<float>(EncoderConstants::UnitCircleRadiusMin, EncoderConstants::UnitCircleRadiusMax), localUnitCircleRadius, "Distance Encoding: Unit Circle");
        
        dbUnit = new AudioParameterFloat("dBUnit", "dB Unit", NormalisableRange<float>(EncoderConstants::DbUnitMin, EncoderConstants::DbUnitMax), localDbUnit, "Distance Encoding: dB Unit");
        
        inverseProportionalDistanceAttenuation = new AudioParameterFloat("DistanceAttenuation", "Distance Attenuation", NormalisableRange<float>(EncoderConstants::DistanceAttenuationMin, EncoderConstants::DistanceAttenuationMax), localInverseProportionalDistanceAttenuation, "Distance Encoding: Inverse Proportional Distance Attenuation");
        
        centerCurve = new AudioParameterFloat("CenterCurve", "Center Curve", NormalisableRange<float>(EncoderConstants::CenterCurveMin, EncoderConstants::CenterCurveMax), localCenterCurve, "Distance Encoding: Center Curve");
        
        advancedFactor = new AudioParameterFloat("AdvancedFactor", "Advanced Factor", NormalisableRange<float>(EncoderConstants::AdvancedFactorMin, EncoderConstants::AdvancedFactorMax), localAdvancedFactor, "Distance Encoding: Advanced Factor");
        
        advancedExponent = new AudioParameterFloat("AdvancedExponent", "Advanced Exponent", NormalisableRange<float>(EncoderConstants::AdvancedExponentMin, EncoderConstants::AdvancedExponentMax), localAdvancedExponent, "Distance Encoding: Advanced Exponent");
        
        pProcessor->addParameter(encodingMode);
        pProcessor->addParameter(unitCircleRadius);
        pProcessor->addParameter(dbUnit);
        pProcessor->addParameter(inverseProportionalDistanceAttenuation);
        pProcessor->addParameter(centerCurve);
        pProcessor->addParameter(advancedFactor);
        pProcessor->addParameter(advancedExponent);
        
        encodingMode->addListener(this);
        unitCircleRadius->addListener(this);
        dbUnit->addListener(this);
        inverseProportionalDistanceAttenuation->addListener(this);
        centerCurve->addListener(this);
        advancedFactor->addListener(this);
        advancedExponent->addListener(this);
    }
    
	void calculateAttenuation(double distance, double* wFactor, double* otherFactor) const
	{
        EncoderConstants::EncodingMode mode = getEncodingMode();
        float unitCircle = getUnitCircleRadius();
        float dbUnitValue = getDbUnit();
        float inverseProportionalDistanceAttenuationValue = getInverseProportionalDistanceAttenuation();
        float centerCurveValue = getCenterCurve();
        float advancedFactorValue = getAdvancedFactor();
        float advancedExponentValue = getAdvancedExponent();
        
        if (mode == EncoderConstants::Standard)
        {
            double scaledDistance = distance * (1.0 / unitCircle);
            *wFactor = atan(scaledDistance * PI / 2.0) / (scaledDistance * PI / 2.0);
            *otherFactor = (1 - exp(-scaledDistance)) * (*wFactor);
        }
        else if(mode == EncoderConstants::Advanced)
        {
            // factor 0-5; power 0-20
            double scaler = distance < unitCircle ? 1.0 : pow(1 - advancedFactorValue * (distance - unitCircle), advancedExponentValue);
            double scaledDistance = distance * (1.0 / unitCircle);
            *wFactor = atan(scaledDistance * PI / 2.0) / (scaledDistance * PI / 2.0) * scaler;
            *otherFactor = (1 - exp(-scaledDistance)) * (*wFactor);
        }
        else
        {
            if(distance < unitCircleRadius->get())
            {
                // inside unit circle
                *wFactor = 1.0;
                *otherFactor = (pow((distance * (1.0 / unitCircle)), -centerCurveValue) * (1 - dbUnitValue)) + dbUnitValue;
            }
            else
            {
                double amplitude;

                if(mode == EncoderConstants::Exponential)
                {
                    amplitude = pow(10.0, (distance - unitCircle) * -dbUnitValue / 20.0);
                }
                else // InverseProportional
                {
                    amplitude = pow(distance + (1 - unitCircle), -inverseProportionalDistanceAttenuationValue);
                }

                *wFactor = amplitude;
                *otherFactor = amplitude;
            }
        }
	}

    EncoderConstants::EncodingMode getEncodingMode() const
    {
        return encodingMode != nullptr ? (EncoderConstants::EncodingMode)encodingMode->getIndex() : localEncodingMode;
    }
    
    void setEncodingMode(EncoderConstants::EncodingMode mode)
    {
        if(encodingMode != nullptr)
            *encodingMode = (int)mode;
        else
            localEncodingMode = mode;
    }
    
    float getUnitCircleRadius() const
    {
        return unitCircleRadius != nullptr ? unitCircleRadius->get() : localUnitCircleRadius;
    }
    
    void setUnitCircleRadius(float radius)
    {
        if(unitCircleRadius != nullptr)
            *unitCircleRadius = radius;
        else
            localUnitCircleRadius = radius;
    }
    
    float getDbUnit() const
    {
        return dbUnit != nullptr ? dbUnit->get() : localDbUnit;
    }
    
    void setDbUnit(float newDbUnit)
    {
        if(dbUnit != nullptr)
            *dbUnit = newDbUnit;
        else
            localDbUnit = newDbUnit;
    }
    
    float getInverseProportionalDistanceAttenuation() const
    {
        return inverseProportionalDistanceAttenuation != nullptr ? inverseProportionalDistanceAttenuation->get() : localInverseProportionalDistanceAttenuation;
    }
    
    void setInverseProportionalDistanceAttenuation(float newAttenuation)
    {
        if(inverseProportionalDistanceAttenuation != nullptr)
            *inverseProportionalDistanceAttenuation = newAttenuation;
        else
            localInverseProportionalDistanceAttenuation = newAttenuation;
    }
    
    float getCenterCurve() const
    {
        return centerCurve != nullptr ? centerCurve->get() : localCenterCurve;
    }
    
    void setCenterCurve(float newValue)
    {
        if(centerCurve != nullptr)
            *centerCurve = newValue;
        else
            localCenterCurve = newValue;
    }
    
    float getAdvancedFactor() const
    {
        return advancedFactor != nullptr ? advancedFactor->get() : localAdvancedFactor;
    }
    
    void setAdvancedFactor(float newFactor)
    {
        if(advancedFactor != nullptr)
            *advancedFactor = newFactor;
        else
            localAdvancedFactor = newFactor;
    }
    
    float getAdvancedExponent() const
    {
        return advancedExponent != nullptr ? advancedExponent->get() : localAdvancedExponent;
    }
    
    void setAdvancedExponent(float newPower)
    {
        if(advancedExponent != nullptr)
            *advancedExponent = newPower;
        else
            localAdvancedExponent = newPower;
    }
    
    
private:
    AudioParameterChoice* encodingMode;
    AudioParameterFloat* unitCircleRadius;
    AudioParameterFloat* dbUnit;
    AudioParameterFloat* inverseProportionalDistanceAttenuation;
    AudioParameterFloat* centerCurve;
    AudioParameterFloat* advancedFactor;
    AudioParameterFloat* advancedExponent;

    // local fields for usage as temporary settings, not connected with audio processor
    EncoderConstants::EncodingMode localEncodingMode;
    float localUnitCircleRadius;
    float localDbUnit;
    float localInverseProportionalDistanceAttenuation;
    float localCenterCurve;
    float localAdvancedFactor;
    float localAdvancedExponent;
    
    void parameterValueChanged(int parameterIndex, float newValue) override {
        sendChangeMessage();
    }
    
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {
        
    }
    
};
