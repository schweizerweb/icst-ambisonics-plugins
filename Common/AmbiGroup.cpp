/*
  ==============================================================================

    AmbiGroup.cpp
    Created: 5 Nov 2019 6:01:24pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiGroup.h"

AmbiGroup::AmbiGroup(XmlElement* xmlElement, OwnedArray<AmbiSource>* pSources) : AmbiPoint(xmlElement)
{
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
					groupPoints.add(pSources->getUnchecked(i));
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
	return element;
}

void AmbiGroup::moveXYZ(double dx, double dy, double dz, bool moveSubElements)
{
	Point3D<double> p = *getPoint();

    if(moveSubElements)
    {
        // check borders first
        checkAndAdjustDeltaXYZ(p.getX(), &dx, p.getY(), &dy, p.getZ(), &dz);
        for (AmbiPoint* sp : groupPoints)
            checkAndAdjustDeltaXYZ(sp->getPoint()->getX(), &dx, sp->getPoint()->getY(), &dy, sp->getPoint()->getZ(), &dz);

        if (!checkXYZ(p.getX() + dx, p.getY() + dy, p.getZ() + dz))
            return;
        
        for (AmbiPoint* sp : groupPoints)
        {
            if(!checkXYZ(sp->getPoint()->getX() + dx, sp->getPoint()->getY() + dy, sp->getPoint()->getZ() + dz))
                return;
        }
    }

	getPoint()->setXYZ(p.getX() + dx, p.getY() + dy, p.getZ() + dz);

	if(moveSubElements)
	{
		for(AmbiPoint* sp : groupPoints)
		{
			sp->getPoint()->setXYZ(sp->getPoint()->getX() + dx, sp->getPoint()->getY() + dy, sp->getPoint()->getZ() + dz);
		}
	}
}

bool AmbiGroup::checkXYZ(double x, double y, double z)
{
    return (Globals::CartesianMin() <= x && x <= Globals::CartesianMax())
        && (Globals::CartesianMin() <= y && y <= Globals::CartesianMax())
        && (Globals::CartesianMin() <= z && z <= Globals::CartesianMax());
}

bool AmbiGroup::checkAED(double a, double e, double d)
{
    double x = d * cos(e) * cos(a);
    double y = d * cos(e) * sin(a);
    double z = d * sin(e);

    return checkXYZ(x, y, z);
}

void AmbiGroup::checkAndAdjustDeltaXYZ(double x, double* dx, double y, double* dy, double z, double* dz)
{
    *dx = (x + *dx) < Globals::CartesianMin() ? Globals::CartesianMin() - x : *dx;
    *dx = (x + *dx) > Globals::CartesianMax() ? Globals::CartesianMax() - x : *dx;
    *dy = (y + *dy) < Globals::CartesianMin() ? Globals::CartesianMin() - y : *dy;
    *dy = (y + *dy) > Globals::CartesianMax() ? Globals::CartesianMax() - y : *dy;
    *dz = (z + *dz) < Globals::CartesianMin() ? Globals::CartesianMin() - z : *dz;
    *dz = (z + *dz) > Globals::CartesianMax() ? Globals::CartesianMax() - z : *dz;
}

void AmbiGroup::setXYZ(double newX, double newY, double newZ, bool moveSubElements)
{
	double dx = newX - getPoint()->getX();
	double dy = newY - getPoint()->getY();
	double dz = newZ - getPoint()->getZ();

    // check new coordinates first
    checkAndAdjustDeltaXYZ(getPoint()->getX(), &dx, getPoint()->getY(), &dy, getPoint()->getZ(), &dz);
    for (AmbiPoint* sp : groupPoints)
        checkAndAdjustDeltaXYZ(sp->getPoint()->getX(), &dx, sp->getPoint()->getY(), &dy, sp->getPoint()->getZ(), &dz);

    newX = getPoint()->getX() + dx;
    newY = getPoint()->getY() + dy;
    newZ = getPoint()->getZ() + dz;

    if (!checkXYZ(newX, newY, newZ))
        return;

    if (moveSubElements)
    {
        // bounding box
        for (AmbiPoint* p : groupPoints)
        {
            if (!checkXYZ(p->getPoint()->getX() + dx, p->getPoint()->getY() + dy, p->getPoint()->getZ() + dz))
                return;
        }
    }

	getPoint()->setXYZ(newX, newY, newZ);

	if(moveSubElements)
	{
		for (AmbiPoint* p : groupPoints)
		{
			p->getPoint()->setXYZ(p->getPoint()->getX() + dx, p->getPoint()->getY() + dy, p->getPoint()->getZ() + dz);
		}
	}
}

void AmbiGroup::setAED(double newA, double newE, double newD, bool moveSubElements)
{
	double da = newA - getPoint()->getAzimuth();
	double de = newE - getPoint()->getElevation();
	double dd = newD - getPoint()->getDistance();

    // check new coordinates first
    if (!checkAED(newA, newE, newD))
        return;

    if (moveSubElements)
    {
        // bounding box
        for (AmbiPoint* p : groupPoints)
        {
            if (!checkAED(p->getPoint()->getAzimuth() + da, p->getPoint()->getElevation() + de, p->getPoint()->getDistance() + dd))
                return;
        }
    }

	getPoint()->setAzimuth(newA);
	getPoint()->setElevation(newE);
	getPoint()->setDistance(newD);

	if (moveSubElements)
	{
		for (AmbiPoint* p : groupPoints)
		{
			double a = p->getPoint()->getAzimuth() + da;
			double e = p->getPoint()->getElevation() + de;
			double d = p->getPoint()->getDistance() + dd;
			p->getPoint()->setAzimuth(a);
			p->getPoint()->setElevation(e);
			p->getPoint()->setDistance(d);
		}
	}
}

void AmbiGroup::setChildrenColor()
{
    for (AmbiPoint* p : groupPoints)
    {
        p->setColor(getColor());
    }
}
