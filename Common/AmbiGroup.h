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
#define XML_ATTRIBUTE_GROUP_ID "Id"


class AmbiGroup : public AmbiPoint
{
public:
	AmbiGroup(String id, Point3D<double> point, String name, Colour color, ScalingInfo* pScaling) : AmbiPoint(id, point, name, color), pScalingInfo(pScaling) {}
	AmbiGroup(XmlElement* xmlElement, OwnedArray<AmbiSource>* pSources, AudioParameterSet audioParameterSet, ScalingInfo* pScaling);
	virtual ~AmbiGroup() = default;

	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;
    void moveXYZ(double dx, double dy, double dz, bool moveSubElements);
    bool checkXYZ(double x, double y, double z);
    bool checkAED(double a, double e, double d);
    void checkAndAdjustDeltaXYZ(double x, double* dx, double y, double* dy, double z, double* dz);
    void setXYZ(double newX, double newY, double newZ, bool moveSubElements);
    void setAED(double newA, double newE, double newD, bool moveSubElements);
    void setChildrenColor();
    void stretch(double stretchValue);
    void extracted(Point3D<double> &center, double cx, AmbiPoint *p, double sx);
    
    void rotate(double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis);
    void rotateAroundOrigin(double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements);

	Array<AmbiPoint*> groupPoints;
    ScalingInfo* pScalingInfo;
    
    bool allowMute() const override { return false; }
    bool allowSolo() const override { return false; }
};
