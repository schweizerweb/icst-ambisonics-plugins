/*
  ==============================================================================

    ZoomSettings.h
    Created: 24 Jul 2017 4:39:16am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#define DEFAULT_POINT_SCALER 1.0
#define DEFAULT_CENTER_X 0.0
#define DEFAULT_CENTER_Y 0.0
#define DEFAULT_CENTER_Z 0.0
#define DEFAULT_RADIUS 1.0

#define MIN_ZOOM_RADIUS 0.001f
#define XML_TAG_ZOOM_SETTINGS "ZoomSettings"
#define XML_ATTRIBUTE_POINT_SCALER "PointScaler"
#define XML_ATTRIBUTE_GROUP_POINT_SCALER "GroupPointScaler"
#define XML_ATTRIBUTE_CENTER_POINT_X "CenterX"
#define XML_ATTRIBUTE_CENTER_POINT_Y "CenterY"
#define XML_ATTRIBUTE_CENTER_POINT_Z "CenterZ"
#define XML_ATTRIBUTE_RADIUS "Radius"

#include "Point3D.h"
#include "ScalingInfo.h"
#include "AmbiDataSet.h"

class ZoomSettings : public ChangeBroadcaster
{
public:
	ZoomSettings(ScalingInfo* pScaling);

	Rectangle<float> getVisibleArea(bool isXZ, bool isFull);
	Point3D<float> getCurrentCenterPoint() const;
	void setCurrentCenterPointXY(float x, float y);
	void setCurrentCenterPointXZ(float x, float z);
	
	float getCurrentRadius() const;
	void setCurrentRadius(float newRadius);
	int getNumberOfRings() const;
    float getRingResolution() const;
    
    void setPointScaler(double newScaler);
    double getPointScaler();
    
    void setGroupPointScaler(double newScaler);
    double getGroupPointScaler();
    
    ScalingInfo* getScalingInfo();
    
    void Reset(AmbiDataSet* pDataSet = nullptr);
    
    void loadFromXml(XmlElement* xmlElement);
    void writeToXmlElement(XmlElement* xml) const;
    
private:
	Point3D<float> currentCenterPoint;
	
	float currentRadius;
    
    double pointScaler;
    double groupPointScaler;
    
    ScalingInfo* pScalingInfo;
};
