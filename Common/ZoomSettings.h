/*
  ==============================================================================

    ZoomSettings.h
    Created: 24 Jul 2017 4:39:16am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "Point3D.h"

class ZoomSettings : public ChangeBroadcaster
{
public:
	ZoomSettings();

	Rectangle<float> getVisibleArea(bool isZY);
	Point3D<float> getInitialCenterPoint() const;
	Point3D<float> getCurrentCenterPoint() const;
	void setCurrentCenterPointXY(float x, float y);
	void setCurrentCenterPointYZ(float y, float z);
	
	float getInitialRadius() const;
	float getCurrentRadius() const;
	void setCurrentRadius(float newRadius);

private:
	Point3D<float> initialCenterPoint;
	Point3D<float> currentCenterPoint;
	
	float initialRadius;
	float currentRadius;

	
	
};
