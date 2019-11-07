/*
  ==============================================================================

    AmbiGroup.h
    Created: 5 Nov 2019 6:01:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiPoint.h"

class AmbiGroup : public AmbiPoint
{
public:
	AmbiGroup(String id, Point3D<double> point, String name, Colour color) : AmbiPoint(id, point, name, color) {}
	virtual ~AmbiGroup() = default;

	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;
	void moveXYZ(double dx, double dy, double dz, bool moveSubElements);
	void setXYZ(double newX, double newY, double newZ, bool moveSubElements);

	Array<AmbiPoint*> groupPoints;
};