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
	AmbiGroup(String _id, Point3D<double> _point, String _name, Colour _color, ScalingInfo* _pScaling) : AmbiPoint(_id, _point, _name, _color), pScalingInfo(_pScaling), rotationQuaternion(Quaternion<double>(0,0,0,1)), stretchFactor(1.0) {}
	AmbiGroup(XmlElement* _xmlElement, Array<AmbiPoint*>* _pSources, AudioParameterSet _audioParameterSet, ScalingInfo* _pScaling);
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
    //void extracted(Point3D<double> &center, double cx, AmbiPoint *p, double sx);
    
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
