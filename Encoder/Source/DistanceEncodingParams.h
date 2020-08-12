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
#define XML_ATTRIBUTE_DISTANCE_ENCODING_MODE "DistEncMode"
#define XML_ATTRIBUTE_DISTANCE_ENCODING_DB_UNIT "DistEncDbUnit"
#define XML_ATTRIBUTE_DISTANCE_ENCODING_DISTANCE_ATTENUATION "DistEncDistanceAttenuation"
#define XML_ATTRIBUTE_DISTANCE_ENCODING_CENTER_CURVE "DistEncCenterCurve"
#define XML_ATTRIBUTE_DISTANCE_ENCODING_ADVANCED_FACTOR "DistEncAdvancedFactor"
#define XML_ATTRIBUTE_DISTANCE_ENCODING_ADVANCED_EXPONENT "DistEncAdvancedExponent"
#define XML_ATTRIBUTE_DISTANCE_ENCODING_UNIT_CIRCLE_RADIUS "UnitCircleRadius"

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
    
    bool setEncodingMode(EncoderConstants::EncodingMode mode)
    {
        if(mode < 0 || mode >= EncoderConstants::encodingModeStrings.size())
            return false;
        
        if(encodingMode != nullptr)
            *encodingMode = (int)mode;
        else
            localEncodingMode = mode;
        
        return true;
    }
    
    float getUnitCircleRadius() const
    {
        return unitCircleRadius != nullptr ? unitCircleRadius->get() : localUnitCircleRadius;
    }
    
    bool setUnitCircleRadius(float radius)
    {
        if(radius < EncoderConstants::UnitCircleRadiusMin || radius > EncoderConstants::UnitCircleRadiusMax)
            return false;
        
        if(unitCircleRadius != nullptr)
            *unitCircleRadius = radius;
        else
            localUnitCircleRadius = radius;
        
        return true;
    }
    
    float getDbUnit() const
    {
        return dbUnit != nullptr ? dbUnit->get() : localDbUnit;
    }
    
    bool setDbUnit(float newDbUnit)
    {
        if(newDbUnit < EncoderConstants::DbUnitMin || newDbUnit > EncoderConstants::DbUnitMax)
            return false;
        
        if(dbUnit != nullptr)
            *dbUnit = newDbUnit;
        else
            localDbUnit = newDbUnit;
        
        return true;
    }
    
    float getInverseProportionalDistanceAttenuation() const
    {
        return inverseProportionalDistanceAttenuation != nullptr ? inverseProportionalDistanceAttenuation->get() : localInverseProportionalDistanceAttenuation;
    }
    
    bool setInverseProportionalDistanceAttenuation(float newAttenuation)
    {
        if(newAttenuation < EncoderConstants::DistanceAttenuationMin || newAttenuation > EncoderConstants::DistanceAttenuationMax)
            return false;
        
        if(inverseProportionalDistanceAttenuation != nullptr)
            *inverseProportionalDistanceAttenuation = newAttenuation;
        else
            localInverseProportionalDistanceAttenuation = newAttenuation;
        
        return true;
    }
    
    float getCenterCurve() const
    {
        return centerCurve != nullptr ? centerCurve->get() : localCenterCurve;
    }
    
    bool setCenterCurve(float newValue)
    {
        if(newValue < EncoderConstants::CenterCurveMin || newValue > EncoderConstants::CenterCurveMax)
            return false;
        
        if(centerCurve != nullptr)
            *centerCurve = newValue;
        else
            localCenterCurve = newValue;
        
        return true;
    }
    
    float getAdvancedFactor() const
    {
        return advancedFactor != nullptr ? advancedFactor->get() : localAdvancedFactor;
    }
    
    bool setAdvancedFactor(float newFactor)
    {
        if(newFactor < EncoderConstants::AdvancedFactorMin || newFactor > EncoderConstants::AdvancedFactorMax)
            return false;
        
        if(advancedFactor != nullptr)
            *advancedFactor = newFactor;
        else
            localAdvancedFactor = newFactor;
        
        return true;
    }
    
    float getAdvancedExponent() const
    {
        return advancedExponent != nullptr ? advancedExponent->get() : localAdvancedExponent;
    }
    
    bool setAdvancedExponent(float newExponent)
    {
        if(newExponent < EncoderConstants::AdvancedExponentMin || newExponent > EncoderConstants::AdvancedExponentMax)
            return false;
        
        if(advancedExponent != nullptr)
            *advancedExponent = newExponent;
        else
            localAdvancedExponent = newExponent;
        
        return true;
    }
    
    bool loadFromXmlElement(XmlElement* xmlElement)
    {
        setEncodingMode((EncoderConstants::EncodingMode)xmlElement->getIntAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_MODE, DEFAULT_DISTANCE_ENCODING_MODE));
        setUnitCircleRadius((float)xmlElement->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_UNIT_CIRCLE_RADIUS, DEFAULT_UNIT_CIRCLE_SIZE));
        setDbUnit((float)xmlElement->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_DB_UNIT, DEFAULT_DB_UNIT));
        setInverseProportionalDistanceAttenuation((float)xmlElement->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_DISTANCE_ATTENUATION, DEFAULT_DISTANCE_ATTENUATION));
        setCenterCurve((float)xmlElement->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_CENTER_CURVE, DEFAULT_CENTER_CURVE));
        setAdvancedFactor((float)xmlElement->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_ADVANCED_FACTOR, DEFAULT_ADVANCED_FACTOR));
        setAdvancedExponent((float)xmlElement->getDoubleAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_ADVANCED_EXPONENT, DEFAULT_ADVANCED_EXPONENT));
        
        return true;
    }
    
    bool writeToXmlElement(XmlElement* xmlElement) const
    {
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_MODE, (int)getEncodingMode());
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_UNIT_CIRCLE_RADIUS, getUnitCircleRadius());
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_DB_UNIT, getDbUnit());
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_DISTANCE_ATTENUATION, getInverseProportionalDistanceAttenuation());
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_CENTER_CURVE, getCenterCurve());
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_ADVANCED_FACTOR, getAdvancedFactor());
        xmlElement->setAttribute(XML_ATTRIBUTE_DISTANCE_ENCODING_ADVANCED_EXPONENT, getAdvancedExponent());
        
        return true;
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
    
    void parameterValueChanged(int /*parameterIndex*/, float /*newValue*/) override {
        sendChangeMessage();
    }
    
    void parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/) override {
        
    }
    
};
