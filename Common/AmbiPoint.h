/*
  ==============================================================================

    AmbiPoint.h
    Created: 3 Sep 2016 11:29:07am
    Author:  christian.schweizer

  ==============================================================================
*/

#ifndef AMBIPOINT_H_INCLUDED
#define AMBIPOINT_H_INCLUDED

#include "JuceHeader.h"
#include "Point3D.h"
#include "PointLabel.h"

class AmbiPoint
{
public:
	AmbiPoint()
	{

	}
	AmbiPoint(Point3D<double> pt, PointLabel lbl, int colorInd = 0)
	{
		point = pt;
		label = lbl;
		colorIndex = colorInd;
	}

	Point3D<double>* getPoint()
	{
		return &point;
	}

	PointLabel* getLabel()
	{
		return &label;
	}

	int getColorIndex()
	{
		return colorIndex;
	}

private:
	Point3D<double> point;
	PointLabel label;
	int colorIndex;
};



#endif  // AMBIPOINT_H_INCLUDED
