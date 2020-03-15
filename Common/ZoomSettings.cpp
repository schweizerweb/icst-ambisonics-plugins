/*
  ==============================================================================

    ZoomSettings.cpp
    Created: 24 Jul 2017 4:39:17am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "ZoomSettings.h"

ZoomSettings::ZoomSettings() : 
	initialCenterPoint(Point3D<float>(0.0, 0.0, 0.0)),
	currentCenterPoint(Point3D<float>(0.0, 0.0, 0.0)),
	initialRadius(1.0),
	currentRadius(1.0),
    pointScaler(DEFAULT_POINT_SCALER)
{
}

Rectangle<float> ZoomSettings::getVisibleArea(bool isXZ, bool isFull)
{
	if(isXZ)
	{
		Point<float> projectedPoint(currentCenterPoint.getX(), currentCenterPoint.getZ());
		return Rectangle<float>(projectedPoint.getX() - currentRadius, projectedPoint.getY() - (isFull ? currentRadius : 0), 2 * currentRadius, currentRadius * (isFull ? 2 : 1));
	}
	
	Point<float> projectedPoint(currentCenterPoint.getX(), currentCenterPoint.getY());
	return Rectangle<float>(projectedPoint.getX() - currentRadius, projectedPoint.getY() - (isFull ? currentRadius : 0), 2 * currentRadius, currentRadius * (isFull ? 2 : 1));
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

void ZoomSettings::setCurrentCenterPointXZ(float x, float z)
{
	currentCenterPoint.setXZ(x, z);
	sendChangeMessage();
}

double ZoomSettings::getPointScaler()
{
    return pointScaler;
}

void ZoomSettings::setPointScaler(double newScaler)
{
    pointScaler = newScaler;
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

int ZoomSettings::getNumberOfRings() const
{
	if (currentRadius < 0.15)
		return 100;
	if (currentRadius < 0.6)
		return 20;
	return 10;
}
