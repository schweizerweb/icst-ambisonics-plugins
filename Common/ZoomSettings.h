/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once
#define DEFAULT_POINT_SCALER 1.0
#define DEFAULT_CENTER_X 0.0
#define DEFAULT_CENTER_Y 0.0
#define DEFAULT_CENTER_Z 0.0
#define DEFAULT_RADIUS 1.0
#define DEFAULT_LABEL_IN_POINT_FLAG false

#define MIN_ZOOM_RADIUS 0.001f
#define XML_TAG_ZOOM_SETTINGS "ZoomSettings"
#define XML_ATTRIBUTE_POINT_SCALER "PointScaler"
#define XML_ATTRIBUTE_GROUP_POINT_SCALER "GroupPointScaler"
#define XML_ATTRIBUTE_LABEL_IN_POINT "LabelInPoint"
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

    void setLabelInPointFlag(bool flag);
    bool getLabelInPointFlag() const;
    
    ScalingInfo* getScalingInfo();
    
    void Reset(AmbiDataSet* pDataSet = nullptr);
    
    void loadFromXml(XmlElement* xmlElement);
    void writeToXmlElement(XmlElement* xml) const;
    
private:
	Point3D<float> currentCenterPoint;
	
	float currentRadius;
    
    double pointScaler;
    double groupPointScaler;
    bool labelInPointFlag;
    
    ScalingInfo* pScalingInfo;
};
