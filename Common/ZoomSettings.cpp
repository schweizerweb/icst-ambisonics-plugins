/*
  ==============================================================================

    ZoomSettings.cpp
    Created: 24 Jul 2017 4:39:17am
    Author:  chris

  ==============================================================================
*/

#include "ZoomSettings.h"

ZoomSettings::ZoomSettings() : 
	initialCenterPoint(Point3D<float>(0.0, 0.0, 0.0)),
	currentCenterPoint(Point3D<float>(0.0, 0.0, 0.0)),
	initialRadius(1.0),
	currentRadius(1.0)
{
}

Rectangle<float> ZoomSettings::getVisibleArea(bool isZY)
{
	Point<float> projectedPoint(currentCenterPoint.getY(), isZY ? currentCenterPoint.getZ() : currentCenterPoint.getX());
	return Rectangle<float>(projectedPoint.getX() - currentRadius, projectedPoint.getY() - currentRadius, 2 * currentRadius, isZY ? currentRadius : 2 * currentRadius);
}

Point3D<float> ZoomSettings::getInitialCenterPoint() const
{
	return initialCenterPoint;
}

Point3D<float> ZoomSettings::getCurrentCenterPoint() const
{
	return currentCenterPoint;
}

void ZoomSettings::setCurrentCenterPointXY(float x, float y)
{
	currentCenterPoint.setXY(x, y);
	sendChangeMessage();
}

void ZoomSettings::setCurrentCenterPointYZ(float y, float z)
{
	currentCenterPoint.setYZ(y, z);
	sendChangeMessage();
}

float ZoomSettings::getInitialRadius() const
{
	return initialRadius;
}

float ZoomSettings::getCurrentRadius() const
{
	return currentRadius;
}

void ZoomSettings::setCurrentRadius(float newRadius)
{
	currentRadius = newRadius;
	sendChangeMessage();
}
