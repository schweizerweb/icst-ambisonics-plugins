/*
  ==============================================================================

    AmbiGroup.h
    Created: 5 Nov 2019 6:01:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"
#include "AmbiSource.h"

#define XML_TAG_SUBPOINTS "SubPoints"
#define XML_TAG_SUBPOINT "SubPoint"
#define XML_TAG_ROTATION "Rotation"
#define XML_TAG_STRETCH "Stretch"
#define XML_ATTRIBUTE_GROUP_ID "Id"
#define XML_ATTRIBUTE_ROTATION_Q0 "Q0"
#define XML_ATTRIBUTE_ROTATION_Q1 "Q1"
#define XML_ATTRIBUTE_ROTATION_Q2 "Q2"
#define XML_ATTRIBUTE_ROTATION_Q3 "Q3"
#define XML_ATTRIBUTE_FACTOR "Factor"

class AmbiGroup : public AmbiPoint
{
public:
	AmbiGroup(String id, Point3D<double> point, String name, Colour color, ScalingInfo* pScaling) : AmbiPoint(id, point, name, color), pScalingInfo(pScaling), rotationQuaternion(Quaternion<double>(0,0,0,1)), stretchFactor(1.0) {}
	AmbiGroup(XmlElement* xmlElement, Array<AmbiPoint*>* pSources, AudioParameterSet audioParameterSet, ScalingInfo* pScaling);
    virtual ~AmbiGroup() = default;

	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;
    void moveXYZ(double dx, double dy, double dz, bool moveSubElements, bool groupModeFlag);
    bool checkXYZ(Vector3D<double> v);
    bool checkAED(double a, double e, double d);
    void checkAndAdjustDeltaXYZ(Vector3D<double> v, Vector3D<double>* pDelta);
    void setXYZ(double newX, double newY, double newZ, bool moveSubElements, bool groupModeFlag);
    void setAED(double newA, double newE, double newD, bool moveSubElements, bool groupModeFlag);
    void setChildrenColor();
    void stretch(double stretchValue, bool groupModeFlag);
    void extracted(Point3D<double> &center, double cx, AmbiPoint *p, double sx);
    
    void rotate(double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool groupModeFlag);
    void rotateAroundOrigin(double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements, bool groupModeFlag);

    ScalingInfo* pScalingInfo;
    
    bool allowMute() const override { return false; }
    bool allowSolo() const override { return false; }
    
    void addPointToGroup(AmbiPoint* pt);
    void removePointFromGroup(AmbiPoint* pt);
    int groupPointCount();
    void removeAllPoints();
    
    void setRotation(Quaternion<double> rotation, bool notify = true);
    void setStretch(double stretchFactor, bool notify = true);
    
    Quaternion<double> getRotation() const;
    double getStretch() const;
    
    void applyTransform(Vector3D<double>* pt);
    void applyInverseTransform(Vector3D<double>* pt);
    
private:
    void compensateGroupPointMovement(Vector3D<double> originalVector);
    
private:
    Array<AmbiPoint*> groupPoints;
    Quaternion<double> rotationQuaternion;
    Matrix3D<double> rotationMatrix;
    double stretchFactor;
};
