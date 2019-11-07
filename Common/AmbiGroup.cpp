/*
  ==============================================================================

    AmbiGroup.cpp
    Created: 5 Nov 2019 6:01:24pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiGroup.h"

float AmbiGroup::getDisplayScaler()
{
	return 0.8f;
}

XmlElement* AmbiGroup::getAsXmlElement(String tagName)
{
	throw;
}

void AmbiGroup::moveXYZ(double dx, double dy, double dz, bool moveSubElements)
{
	Point3D<double> p = *getPoint();
	getPoint()->setXYZ(p.getX() + dx, p.getY() + dy, p.getZ() + dz);

	if(moveSubElements)
	{
		for(AmbiPoint* p : groupPoints)
		{
			p->getPoint()->setXYZ(p->getPoint()->getX() + dx, p->getPoint()->getY() + dy, p->getPoint()->getZ() + dz);
		}
	}
}

void AmbiGroup::setXYZ(double newX, double newY, double newZ, bool moveSubElements)
{
	double dx = newX - getPoint()->getX();
	double dy = newY - getPoint()->getY();
	double dz = newZ - getPoint()->getZ();

	getPoint()->setXYZ(newX, newY, newZ);

	if(moveSubElements)
	{
		for (AmbiPoint* p : groupPoints)
		{
			p->getPoint()->setXYZ(p->getPoint()->getX() + dx, p->getPoint()->getY() + dy, p->getPoint()->getZ() + dz);
		}
	}
}
