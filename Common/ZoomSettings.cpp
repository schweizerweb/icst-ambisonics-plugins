/*
  ==============================================================================

    ZoomSettings.cpp
    Created: 24 Jul 2017 4:39:17am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "ZoomSettings.h"
#include "JuceHeader.h"

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
	currentRadius = jmax(MIN_ZOOM_RADIUS, newRadius);
	sendChangeMessage();
}

void ZoomSettings::Reset()
{
    currentCenterPoint.setXYZ(0.0, 0.0, 0.0);
    setCurrentRadius(Globals::IsInfinite() ? 1.0f : Globals::GetScaler());
}

int ZoomSettings::getNumberOfRings() const
{
	if (currentRadius < 0.15)
		return 100;
	if (currentRadius < 0.6)
		return 20;
	return 10;
}

float ZoomSettings::getRingResolution() const
{
    if(currentRadius < 0.006)
        return 0.0005f;
    if(currentRadius < 0.015)
        return 0.001f;
    if(currentRadius < 0.06)
        return 0.005f;
    if(currentRadius < 0.15)
        return 0.01f;
    if(currentRadius < 0.6)
        return 0.05f;
    if(currentRadius < 1.5)
        return 0.1f;
    if(currentRadius < 6)
        return 0.5f;
    if(currentRadius < 15)
        return 1.0f;
    if(currentRadius < 60)
        return 5.0f;
    if(currentRadius < 150)
        return 10.0f;
    if(currentRadius < 600)
        return 50.0f;
    if(currentRadius < 1500)
        return 100.0f;
    if(currentRadius < 6000)
        return 500.0f;
    if(currentRadius < 15000)
        return 1000.0f;
    if(currentRadius < 60000)
        return 5000.0f;
    
    return 10000.0f;
}
