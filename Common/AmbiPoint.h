/*
  ==============================================================================

    AmbiPoint.h
    Created: 3 Sep 2016 11:29:07am
    Author:  christian.schweizer

  ==============================================================================
*/

#ifndef AMBIPOINT_H_INCLUDED
#define AMBIPOINT_H_INCLUDED

#include "Point3D.h"

class AmbiPoint
{
public:
	AmbiPoint(): colorIndex(0), gain(1.0)
	{
	}

	AmbiPoint(Point3D<double> point, String name, int colorIndex = 0, double gain = 1.0) :
		point(point),
		colorIndex(colorIndex), 
		name(name),
		gain(gain)
	{
	}

	Point3D<double>* getPoint()
	{
		return &point;
	}

	int getColorIndex() const
	{
		return colorIndex;
	}

	String getName() const
	{
		return name;
	}

	void setName(String newName)
	{
		name = newName;
	}

	double getGain() const
	{
		return gain;
	};

	void setGain(double newGain)
	{
		gain = newGain;
	}

private:
	Point3D<double> point;
	int colorIndex;
	String name;
	double gain;
};



#endif  // AMBIPOINT_H_INCLUDED
