/*
  ==============================================================================

    DistanceEncodingParams.h
    Created: 22 Nov 2019 1:32:33pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../../Common/Constants.h"
#define DEFAULT_UNIT_CIRCLE_SIZE	0.1f

class DistanceEncodingParams
{
public:
	DistanceEncodingParams(): unitCircleRadius(DEFAULT_UNIT_CIRCLE_SIZE)
	{
	}

	float unitCircleRadius;
	void calculateAttenuation(double distance, double* wFactor, double* otherFactor) const
	{
		double scaledDistance = distance * (1.0 / unitCircleRadius);
		*wFactor = atan(scaledDistance * PI / 2.0) / (scaledDistance * PI / 2.0);
		*otherFactor = (1 - exp(-scaledDistance)) * (*wFactor);
	}
};
