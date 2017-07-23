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
	AmbiPoint()
	{

	}
	AmbiPoint(Point3D<double> pt, int colorInd = 0)
	{
		point = pt;
		colorIndex = colorInd;
	}

	Point3D<double>* getPoint()
	{
		return &point;
	}

	int getColorIndex()
	{
		return colorIndex;
	}

private:
	Point3D<double> point;
	int colorIndex;
};



#endif  // AMBIPOINT_H_INCLUDED
