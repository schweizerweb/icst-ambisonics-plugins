/*
  ==============================================================================

    DistanceEncodingParams.h
    Created: 22 Nov 2019 1:32:33pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../../Common/Constants.h"
#define DEFAULT_UNIT_CIRCLE_SIZE	    0.1f
#define DEFAULT_DB_UNIT         	    10.0f
#define DEFAULT_DISTANCE_ATTENUATION	5.0f
#define DEFAULT_CENTER_CURVE        	0.02f
#define DEFAULT_CENTER_ATTENUATION  	20.0f
#define DEFAULT_DISTANCE_ENCODING_MODE  DistanceEncodingParams::Standard

class DistanceEncodingParams
{
public:
    enum EncodingMode { None, Standard, Exponential, InverseProportional };

	DistanceEncodingParams(): 
        encodingMode(Standard), 
        unitCircleRadius(DEFAULT_UNIT_CIRCLE_SIZE), 
        dbUnit(DEFAULT_DB_UNIT), 
        inverseProportionalDistanceAttenuation(DEFAULT_DISTANCE_ATTENUATION), 
        centerCurve(DEFAULT_CENTER_CURVE),
        centerAttenuation(DEFAULT_CENTER_ATTENUATION)
    {
    }

    EncodingMode encodingMode;
	float unitCircleRadius;
    float dbUnit;
    float inverseProportionalDistanceAttenuation;
    float centerCurve;
    float centerAttenuation;

	void calculateAttenuation(double distance, double* wFactor, double* otherFactor) const
	{
        if (encodingMode == Standard)
        {
            double scaledDistance = distance * (1.0 / unitCircleRadius);
            *wFactor = atan(scaledDistance * PI / 2.0) / (scaledDistance * PI / 2.0) * scaler;
            *otherFactor = (1 - exp(-scaledDistance)) * (*wFactor);
        }
        else
        {
            if(distance < unitCircleRadius)
            {
                // inside unit circle
                *wFactor = 1.0;
                *otherFactor = (pow((distance * (1.0 / unitCircleRadius)), -centerCurve) * (1 - dbUnit)) + dbUnit;
            }
            else
            {
                double amplitude;

                if(encodingMode == Exponential)
                {
                    amplitude = pow(10.0, (distance - unitCircleRadius) * -dbUnit / 20.0);
                }
                else // InverseProportional
                {
                    amplitude = pow(distance + (1 - unitCircleRadius), -inverseProportionalDistanceAttenuation);
                }

                *wFactor = amplitude;
                *otherFactor = amplitude;
            }
        }
	}
};
