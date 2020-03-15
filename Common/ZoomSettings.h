/*
  ==============================================================================

    ZoomSettings.h
    Created: 24 Jul 2017 4:39:16am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#define DEFAULT_POINT_SCALER 1.0

#include "Point3D.h"

class ZoomSettings : public ChangeBroadcaster
{
public:
	ZoomSettings();

	Rectangle<float> getVisibleArea(bool isXZ, bool isFull);
	Point3D<float> getInitialCenterPoint() const;
	Point3D<float> getCurrentCenterPoint() const;
	void setCurrentCenterPointXY(float x, float y);
	void setCurrentCenterPointXZ(float x, float z);
	
	float getInitialRadius() const;
	float getCurrentRadius() const;
	void setCurrentRadius(float newRadius);
	int getNumberOfRings() const;
    
    void setPointScaler(double newScaler);
    double getPointScaler();

private:
	Point3D<float> initialCenterPoint;
	Point3D<float> currentCenterPoint;
	
	float initialRadius;
	float currentRadius;
    
    double pointScaler;
};
