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
	AmbiPoint(): colorIndex(0)
	{
	}

	AmbiPoint(Point3D<double> point, String name, int colorIndex = 0) : 
		point(point),
		name(name),
		colorIndex(colorIndex)
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

private:
	Point3D<double> point;
	int colorIndex;
	String name;
};



#endif  // AMBIPOINT_H_INCLUDED
