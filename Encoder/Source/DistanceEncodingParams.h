/*
  ==============================================================================

    DistanceEncodingParams.h
    Created: 22 Nov 2019 1:32:33pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../../Common/Constants.h"
#include "DistanceEncodingParameterSet.h"
#include "EncoderConstants.h"

#define DEFAULT_UNIT_CIRCLE_SIZE	    0.1f
#define DEFAULT_DB_UNIT         	    10.0f
#define DEFAULT_DISTANCE_ATTENUATION	5.0f
#define DEFAULT_CENTER_CURVE        	0.02f
#define DEFAULT_EXPERIMENTAL_FACTOR  	1.0f
#define DEFAULT_EXPERIMENTAL_POWER  	1.0f
#define DEFAULT_DISTANCE_ENCODING_MODE  EncoderConstants::Standard

class DistanceEncodingParams
{
public:
    
	DistanceEncodingParams():
        encodingMode(nullptr),
        unitCircleRadius(nullptr),
        dbUnit(nullptr),
        inverseProportionalDistanceAttenuation(nullptr),
        centerCurve(nullptr),
        experimentalFactor(nullptr),
        experimentalPower(nullptr),
        localEncodingMode(DEFAULT_DISTANCE_ENCODING_MODE),
        localUnitCircleRadius(DEFAULT_UNIT_CIRCLE_SIZE),
        localDbUnit(DEFAULT_DB_UNIT),
        localInverseProportionalDistanceAttenuation(DEFAULT_DISTANCE_ATTENUATION),
        localCenterCurve(DEFAULT_CENTER_CURVE),
        localExperimentalFactor(DEFAULT_EXPERIMENTAL_FACTOR),
        localExperimentalPower(DEFAULT_EXPERIMENTAL_POWER)
    {
    }

    void initialize(AudioProcessor* pProcessor){
        encodingMode = new AudioParameterChoice("EncodingMode", "Encoding Mode",
                                                EncoderConstants::encodingModeStrings, localEncodingMode, "Distance Encoding: Mode");
        
        unitCircleRadius = new AudioParameterFloat("UnitCircle", "Unit Circle", NormalisableRange<float>(EncoderConstants::UnitCircleRadiusMin, EncoderConstants::UnitCircleRadiusMax), localUnitCircleRadius, "Distance Encoding: Unit Circle");
        
        dbUnit = new AudioParameterFloat("dBUnit", "dB Unit", NormalisableRange<float>(EncoderConstants::DbUnitMin, EncoderConstants::DbUnitMax), localDbUnit, "Distance Encoding: dB Unit");
        
        inverseProportionalDistanceAttenuation = new AudioParameterFloat("DistanceAttenuation", "Distance Attenuation", NormalisableRange<float>(EncoderConstants::DistanceAttenuationMin, EncoderConstants::DistanceAttenuationMax), localInverseProportionalDistanceAttenuation, "Distance Encoding: Inverse Proportional Distance Attenuation");
        
        centerCurve = new AudioParameterFloat("CenterCurve", "Center Curve", NormalisableRange<float>(EncoderConstants::CenterCurveMin, EncoderConstants::CenterCurveMax), localCenterCurve, "Distance Encoding: Center Curve");
        
        experimentalFactor = new AudioParameterFloat("ExperimentalFactor", "Experimental Factor", NormalisableRange<float>(EncoderConstants::ExperimentalFactorMin, EncoderConstants::ExperimentalFactorMax), localExperimentalFactor, "Distance Encoding: Experimental Factor");
        
        experimentalPower = new AudioParameterFloat("ExperimentalPower", "Experimental Power", NormalisableRange<float>(EncoderConstants::ExperimentalPowerMin, EncoderConstants::ExperimentalPowerMax), localExperimentalPower, "Distance Encoding: Experimental Power");
        
        pProcessor->addParameter(encodingMode);
        pProcessor->addParameter(unitCircleRadius);
        pProcessor->addParameter(dbUnit);
        pProcessor->addParameter(inverseProportionalDistanceAttenuation);
        pProcessor->addParameter(centerCurve);
        pProcessor->addParameter(experimentalFactor);
        pProcessor->addParameter(experimentalPower);
    }
    
	void calculateAttenuation(double distance, double* wFactor, double* otherFactor) const
	{
        EncoderConstants::EncodingMode mode = getEncodingMode();
        float unitCircle = getUnitCircleRadius();
        float dbUnitValue = getDbUnit();
        float inverseProportionalDistanceAttenuationValue = getInverseProportionalDistanceAttenuation();
        float centerCurveValue = getCenterCurve();
        float experimentalFactorValue = getExperimentalFactor();
        float experimentalPowerValue = getExperimentalPower();
        
        if (mode == EncoderConstants::Standard)
        {
            double scaledDistance = distance * (1.0 / unitCircle);
            *wFactor = atan(scaledDistance * PI / 2.0) / (scaledDistance * PI / 2.0);
            *otherFactor = (1 - exp(-scaledDistance)) * (*wFactor);
        }
        else if(mode == EncoderConstants::Experimental)
        {
            // factor 0-5; power 0-20
            double scaler = distance < unitCircle ? 1.0 : pow(1 - experimentalFactorValue * (distance - unitCircle), experimentalPowerValue);
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
    
    float getExperimentalFactor() const
    {
        return experimentalFactor != nullptr ? experimentalFactor->get() : localExperimentalFactor;
    }
    
    void setExperimentalFactor(float newFactor)
    {
        if(experimentalFactor != nullptr)
            *experimentalFactor = newFactor;
        else
            localExperimentalFactor = newFactor;
    }
    
    float getExperimentalPower() const
    {
        return experimentalPower != nullptr ? experimentalPower->get() : localExperimentalPower;
    }
    
    void setExperimentalPower(float newPower)
    {
        if(experimentalPower != nullptr)
            *experimentalPower = newPower;
        else
            localExperimentalPower = newPower;
    }
    
    
private:
    AudioParameterChoice* encodingMode;
    AudioParameterFloat* unitCircleRadius;
    AudioParameterFloat* dbUnit;
    AudioParameterFloat* inverseProportionalDistanceAttenuation;
    AudioParameterFloat* centerCurve;
    AudioParameterFloat* experimentalFactor;
    AudioParameterFloat* experimentalPower;

    // local fields for usage as temporary settings, not connected with audio processor
    EncoderConstants::EncodingMode localEncodingMode;
    float localUnitCircleRadius;
    float localDbUnit;
    float localInverseProportionalDistanceAttenuation;
    float localCenterCurve;
    float localExperimentalFactor;
    float localExperimentalPower;
};
