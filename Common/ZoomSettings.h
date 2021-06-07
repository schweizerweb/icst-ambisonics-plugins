/*
  ==============================================================================

    ZoomSettings.h
    Created: 24 Jul 2017 4:39:16am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#define DEFAULT_POINT_SCALER 1.0
#define MIN_ZOOM_RADIUS 0.001f

#include "Point3D.h"
#include "ScalingInfo.h"
#include "AmbiDataSet.h"

class ZoomSettings : public ChangeBroadcaster
{
public:
	ZoomSettings(ScalingInfo* pScaling);

	Rectangle<float> getVisibleArea(bool isXZ, bool isFull);
	Point3D<float> getInitialCenterPoint() const;
	Point3D<float> getCurrentCenterPoint() const;
	void setCurrentCenterPointXY(float x, float y);
	void setCurrentCenterPointXZ(float x, float z);
	
	float getInitialRadius() const;
	float getCurrentRadius() const;
	void setCurrentRadius(float newRadius);
	int getNumberOfRings() const;
    float getRingResolution() const;
    
    void setPointScaler(double newScaler);
    double getPointScaler();
    
    ScalingInfo* getScalingInfo();
    
    void Reset(AmbiDataSet* pDataSet = nullptr);

private:
	Point3D<float> initialCenterPoint;
	Point3D<float> currentCenterPoint;
	
	float initialRadius;
	float currentRadius;
    
    double pointScaler;
    
    ScalingInfo* pScalingInfo;
};
