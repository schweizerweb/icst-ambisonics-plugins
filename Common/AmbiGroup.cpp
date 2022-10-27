/*
  ==============================================================================

    AmbiGroup.cpp
    Created: 5 Nov 2019 6:01:24pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiGroup.h"

AmbiGroup::AmbiGroup(XmlElement* xmlElement, OwnedArray<AmbiSource>* pSources, AudioParameterSet audioParameterSet, ScalingInfo* pScaling) : AmbiPoint(xmlElement, audioParameterSet), pScalingInfo(pScaling), rotationQuaternion(Quaternion<double>(0, 0, 0, 1)), stretchFactor(1.0)
{
    XmlElement* rot = xmlElement->getChildByName(XML_TAG_ROTATION);
    if(rot != nullptr)
    {
        double x = rot->getDoubleAttribute(XML_ATTRIBUTE_ROTATION_Q0);
        double y = rot->getDoubleAttribute(XML_ATTRIBUTE_ROTATION_Q1);
        double z = rot->getDoubleAttribute(XML_ATTRIBUTE_ROTATION_Q2);
        double w = rot->getDoubleAttribute(XML_ATTRIBUTE_ROTATION_Q3);
        setRotation(Quaternion<double>(x, y, z, w));
    }
    
    XmlElement* stretch = xmlElement->getChildByName(XML_TAG_STRETCH);
    if(stretch != nullptr)
    {
        double f = stretch->getDoubleAttribute(XML_ATTRIBUTE_FACTOR);
        setStretch(f);
    }
    
	XmlElement* subPointsElement = xmlElement->getChildByName(XML_TAG_SUBPOINTS);
	groupPoints.clear();
	if (subPointsElement != nullptr)
	{
		XmlElement* xmlSubPoint = subPointsElement->getChildByName(XML_TAG_SUBPOINT);
		while (xmlSubPoint != nullptr)
		{
			String idStr = xmlSubPoint->getStringAttribute(XML_ATTRIBUTE_GROUP_ID);
			for (int i = 0; i < pSources->size(); i++)
			{
				if (pSources->getUnchecked(i)->getId() == idStr)
				{
					//groupPoints.add(pSources->getUnchecked(i));
                    addPointToGroup(pSources->getUnchecked(i));
					break;
				}
			}

			xmlSubPoint = xmlSubPoint->getNextElement();
		}
	}
}

float AmbiGroup::getDisplayScaler()
{
	return 0.8f;
}

XmlElement* AmbiGroup::getAsXmlElement(String tagName)
{
	XmlElement* element = getBaseXmlElement(tagName);
	XmlElement* subPoints = element->createNewChildElement(XML_TAG_SUBPOINTS);
	for(int i = 0; i < groupPoints.size(); i++)
	{
		XmlElement* xmlPt = subPoints->createNewChildElement(XML_TAG_SUBPOINT);
		xmlPt->setAttribute(XML_ATTRIBUTE_GROUP_ID, groupPoints.getUnchecked(i)->getId());
	}
    
    XmlElement* rot = element->createNewChildElement(XML_TAG_ROTATION);
    rot->setAttribute(XML_ATTRIBUTE_ROTATION_Q0, rotationQuaternion.vector.x);
    rot->setAttribute(XML_ATTRIBUTE_ROTATION_Q1, rotationQuaternion.vector.y);
    rot->setAttribute(XML_ATTRIBUTE_ROTATION_Q2, rotationQuaternion.vector.z);
    rot->setAttribute(XML_ATTRIBUTE_ROTATION_Q3, rotationQuaternion.scalar);
    
    XmlElement* stretch = element->createNewChildElement(XML_TAG_STRETCH);
    stretch->setAttribute(XML_ATTRIBUTE_FACTOR, stretchFactor);
    
	return element;
}

void AmbiGroup::moveXYZ(double dx, double dy, double dz, bool moveSubElements, bool groupModeFlag)
{
    Vector3D<double> p = getVector3D();
    Vector3D<double> delta(dx, dy, dz);
    
    if(groupModeFlag)
    {
        auto originalVector = getVector3D();
        
        getRawPoint()->setXYZ(p.x + dx, p.y + dy, p.z + dz);
        if(!moveSubElements)
        {
            compensateGroupPointMovement(originalVector);
        }
    }
    else
    {
        if(moveSubElements)
        {
            // check borders first
            checkAndAdjustDeltaXYZ(p, &delta);
            for (AmbiPoint* sp : groupPoints)
                checkAndAdjustDeltaXYZ(sp->getVector3D(), &delta);

            if (!checkXYZ(p + delta))
                return;
            
            for (AmbiPoint* sp : groupPoints)
            {
                if(!checkXYZ(sp->getVector3D() + delta))
                    return;
            }
        }

        getRawPoint()->setXYZ(p.x + dx, p.y + dy, p.z + dz);

        if(moveSubElements)
        {
            for(AmbiPoint* sp : groupPoints)
            {
                sp->getRawPoint()->setXYZ(sp->getRawPoint()->getX() + dx, sp->getRawPoint()->getY() + dy, sp->getRawPoint()->getZ() + dz);
            }
        }
    }
}

bool AmbiGroup::checkXYZ(Vector3D<double> v)
{
    return (pScalingInfo->CartesianMin() <= v.x && v.x <= pScalingInfo->CartesianMax())
        && (pScalingInfo->CartesianMin() <= v.y && v.y <= pScalingInfo->CartesianMax())
        && (pScalingInfo->CartesianMin() <= v.z && v.z <= pScalingInfo->CartesianMax());
}

bool AmbiGroup::checkAED(double a, double e, double d)
{
    double x = d * cos(e) * cos(a);
    double y = d * cos(e) * sin(a);
    double z = d * sin(e);

    return checkXYZ(Vector3D<double>(x, y, z));
}

void AmbiGroup::checkAndAdjustDeltaXYZ(Vector3D<double> v, Vector3D<double>* pDelta)
{
    pDelta->x = (v.x + pDelta->x) < pScalingInfo->CartesianMin() ? pScalingInfo->CartesianMin() - v.x : pDelta->x;
    pDelta->x = (v.x + pDelta->x) > pScalingInfo->CartesianMax() ? pScalingInfo->CartesianMax() - v.x : pDelta->x;
    pDelta->y = (v.y + pDelta->y) < pScalingInfo->CartesianMin() ? pScalingInfo->CartesianMin() - v.y : pDelta->y;
    pDelta->y = (v.y + pDelta->y) > pScalingInfo->CartesianMax() ? pScalingInfo->CartesianMax() - v.y : pDelta->y;
    pDelta->z = (v.z + pDelta->z) < pScalingInfo->CartesianMin() ? pScalingInfo->CartesianMin() - v.z : pDelta->z;
    pDelta->z = (v.z + pDelta->z) > pScalingInfo->CartesianMax() ? pScalingInfo->CartesianMax() - v.z : pDelta->z;
}

void AmbiGroup::setXYZ(double newX, double newY, double newZ, bool moveSubElements, bool groupModeFlag)
{
    if(groupModeFlag)
    {
        auto originalVector = getVector3D();
        getRawPoint()->setXYZ(newX, newY, newZ);
  
        if(!moveSubElements)
        {
            compensateGroupPointMovement(originalVector);
        }
    }
    else
    {
        Vector3D<double> d = Vector3D<double>(newX, newY, newZ) - getVector3D();
        
        // check new coordinates first
        checkAndAdjustDeltaXYZ(getVector3D(), &d);
        if(moveSubElements)
        {
            for (AmbiPoint* sp : groupPoints)
                checkAndAdjustDeltaXYZ(sp->getVector3D(), &d);
        }
        
        Vector3D<double> nV = getVector3D() + d;

        if (!checkXYZ(nV))
            return;

        if (moveSubElements)
        {
            // bounding box
            for (AmbiPoint* p : groupPoints)
            {
                if (!checkXYZ(p->getVector3D() + d))
                    return;
            }
        }

        getRawPoint()->setXYZ(newX, newY, newZ);

        if(moveSubElements)
        {
            for (AmbiPoint* p : groupPoints)
            {
                Vector3D<double> nV = p->getVector3D() + d;
                p->getRawPoint()->setXYZ(nV.x, nV.y, nV.z);
            }
        }
    }
}

void AmbiGroup::compensateGroupPointMovement(Vector3D<double> originalVector)
{
    auto newVector = getVector3D();
    auto diff = newVector - originalVector;
    for(auto& p : groupPoints)
    {
        auto pv = p->getVector3D();
        applyTransform(&pv);
        pv -= diff;
        applyInverseTransform(&pv);
        p->getRawPoint()->setXYZ(pv.x, pv.y, pv.z);
    }
}

void AmbiGroup::setAED(double newA, double newE, double newD, bool moveSubElements, bool groupModeFlag)
{
    if(groupModeFlag)
    {
        auto originalVector = getVector3D();
        getRawPoint()->setAed(newA, newE, newD);
        
        if(!moveSubElements)
        {
            compensateGroupPointMovement(originalVector);
        }
    }
    else
    {
        double da = newA - getRawPoint()->getAzimuth();
        double de = newE - getRawPoint()->getElevation();
        double dd = newD - getRawPoint()->getDistance();

        // check new coordinates first
        if (!checkAED(newA, newE, newD))
            return;

        if (moveSubElements)
        {
            // bounding box
            for (AmbiPoint* p : groupPoints)
            {
                if (!checkAED(p->getRawPoint()->getAzimuth() + da, p->getRawPoint()->getElevation() + de, p->getRawPoint()->getDistance() + dd))
                    return;
            }
        }

        getRawPoint()->setAed(newA, newE, newD);

        if (moveSubElements)
        {
            for (AmbiPoint* p : groupPoints)
            {
                double a = p->getRawPoint()->getAzimuth() + da;
                double e = p->getRawPoint()->getElevation() + de;
                double d = p->getRawPoint()->getDistance() + dd;
                p->getRawPoint()->setAed(a, e, d);
            }
        }
    }
}

void AmbiGroup::stretch(double stretchValue, bool groupModeFlag)
{
    if(groupModeFlag)
    {
        setStretch(stretchFactor + stretchValue);
    }
    else
    {
        for (AmbiPoint* p : groupPoints)
        {
            Point3D<double> delta = *(p->getRawPoint())-(*getRawPoint());
            delta.setDistance(delta.getDistance() + stretchValue);
            Point3D<double> newPoint = *getRawPoint() + delta;
            p->getRawPoint()->setAed(newPoint.getAzimuth(), newPoint.getElevation(), newPoint.getDistance());
        }
    }
}

void AmbiGroup::rotate(double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool groupModeFlag)
{
    if(groupModeFlag)
    {
        rotationMatrix *= Matrix3D<double>::rotation(Vector3D<double>(angleAroundXAxis, angleAroundYAxis, angleAroundZAxis));
        
        if(angleAroundXAxis != 0.0)
        {
            Quaternion<double> xRot = Quaternion<double>::fromAngle(angleAroundXAxis, Vector3D<double>(1, 0, 0));
            rotationQuaternion *= xRot;
        }
        
        if(angleAroundYAxis != 0.0)
        {
            Quaternion<double> yRot = Quaternion<double>::fromAngle(angleAroundYAxis, Vector3D<double>(0, 1, 0));
            rotationQuaternion *= yRot;
        }
        
        if(angleAroundZAxis != 0.0)
        {
            Quaternion<double> zRot = Quaternion<double>::fromAngle(angleAroundZAxis, Vector3D<double>(0, 0, -1));
            rotationQuaternion *= zRot;
        }
        
        setRotation(rotationQuaternion);
    }
    else
    {
        double sx = sin(-angleAroundXAxis);
        double cx = cos(-angleAroundXAxis);
        double sy = sin(-angleAroundYAxis);
        double cy = cos(-angleAroundYAxis);
        double sz = sin(-angleAroundZAxis);
        double cz = cos(-angleAroundZAxis);
        
        for (AmbiPoint* p : groupPoints)
        {
            Point3D<double> center = *getRawPoint();
            
            if(angleAroundXAxis != 0.0)
            {
                Point3D<double> relativeChild = *(p->getRawPoint())-center;
                p->getRawPoint()->setYZ(
                                     center.getY() + relativeChild.getY() * cx - relativeChild.getZ() * sx,
                                     center.getZ() + relativeChild.getY() * sx + relativeChild.getZ() * cx);
            }
            
            if(angleAroundYAxis != 0.0)
            {
                Point3D<double> relativeChild = *(p->getRawPoint())-center;
                p->getRawPoint()->setXZ(
                                    center.getX() + relativeChild.getX() * cy - relativeChild.getZ() * sy,
                                    center.getZ() + relativeChild.getX() * sy + relativeChild.getZ() * cy);
            }
            
            if(angleAroundZAxis != 0.0)
            {
                Point3D<double> relativeChild = *(p->getRawPoint())-center;
                p->getRawPoint()->setXY(
                                     center.getX() + relativeChild.getX() * cz - relativeChild.getY() * sz,
                                     center.getY() + relativeChild.getX() * sz + relativeChild.getY() * cz);
            }
        }
    }
}

void AmbiGroup::rotateAroundOrigin(double angleAroundXAxis, double angleAroundYAxis, double angleAroundZAxis, bool moveSubElements, bool groupModeFlag)
{
    if(angleAroundXAxis != 0.0)
    {
        double sx = sin(-angleAroundXAxis);
        double cx = cos(-angleAroundXAxis);
        
        setXYZ(getRawPoint()->getX(),
               getRawPoint()->getY() * cx - getRawPoint()->getZ() * sx,
               getRawPoint()->getY() * sx + getRawPoint()->getZ() * cx,
               moveSubElements, groupModeFlag);
    }
    
    if(angleAroundYAxis != 0.0)
    {
        double sy = sin(-angleAroundYAxis);
        double cy = cos(-angleAroundYAxis);
        
        setXYZ(
               getRawPoint()->getX() * cy - getRawPoint()->getZ() * sy,
               getRawPoint()->getY(),
               getRawPoint()->getX() * sy + getRawPoint()->getZ() * cy,
               moveSubElements, groupModeFlag);
    }
    
    if(angleAroundZAxis != 0.0)
    {
        double sz = sin(-angleAroundZAxis);
        double cz = cos(-angleAroundZAxis);
        
        setXYZ(
               getRawPoint()->getX() * cz - getRawPoint()->getY() * sz,
               getRawPoint()->getX() * sz + getRawPoint()->getY() * cz,
               getRawPoint()->getZ(),
               moveSubElements, groupModeFlag);
    }
}

void AmbiGroup::setChildrenColor()
{
    for (AmbiPoint* p : groupPoints)
    {
        p->setColor(getColor());
    }
}

void AmbiGroup::addPointToGroup(AmbiPoint *pt)
{
    if(pt->getGroup() != nullptr)
    {
        pt->getGroup()->removePointFromGroup(pt);
    }
    
    pt->setGroup(this);
    groupPoints.add(pt);
}

void AmbiGroup::removePointFromGroup(AmbiPoint *pt)
{
    for(int i = 0; i < groupPoints.size(); i++)
    {
        if(groupPoints[i] == pt)
        {
            pt->setGroup(nullptr);
            groupPoints.remove(i);
        }
    }
}

int AmbiGroup::groupPointCount() { 
    return groupPoints.size();
}

void AmbiGroup::removeAllPoints() {
    while (groupPoints.size()) {
        removePointFromGroup(groupPoints[0]);
    }
}

void AmbiGroup::setRotation(Quaternion<double> rotation, bool notify)
{
    rotationQuaternion = rotation.normalised();
    rotationMatrix = rotation.getRotationMatrix();
    
    if(notify)
    {
        audioParams.notifyQ(rotationQuaternion.vector.x, rotationQuaternion.vector.y, rotationQuaternion.vector.z, rotationQuaternion.scalar);
    }
}

void AmbiGroup::setStretch(double stretch, bool notify)
{
    stretchFactor = stretch;

    if(notify)
    {
        stretchFactor = audioParams.notifyStretch(stretch);
    }
}

void AmbiGroup::applyTransform(Vector3D<double> *pt)
{
    auto o = *pt;
    *pt = Vector3D<double>(
               rotationMatrix.mat[0] * o.x + rotationMatrix.mat[1] * o.y + rotationMatrix.mat[2] * o.z + rotationMatrix.mat[3] * 1,
               rotationMatrix.mat[4] * o.x + rotationMatrix.mat[5] * o.y + rotationMatrix.mat[6] * o.z + rotationMatrix.mat[7] * 1,
               rotationMatrix.mat[8] * o.x + rotationMatrix.mat[9] * o.y + rotationMatrix.mat[10] * o.z + rotationMatrix.mat[11] * 1);
    *pt *= stretchFactor;
}

void AmbiGroup::applyInverseTransform(Vector3D<double> *pt)
{
    auto o = *pt;
    o /= stretchFactor;
    *pt = Vector3D<double>(
               rotationMatrix.mat[0] * o.x + rotationMatrix.mat[4] * o.y + rotationMatrix.mat[8] * o.z + rotationMatrix.mat[12] * 1,
               rotationMatrix.mat[1] * o.x + rotationMatrix.mat[5] * o.y + rotationMatrix.mat[9] * o.z + rotationMatrix.mat[13] * 1,
               rotationMatrix.mat[2] * o.x + rotationMatrix.mat[6] * o.y + rotationMatrix.mat[10] * o.z + rotationMatrix.mat[14] * 1);
}
