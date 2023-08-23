/*
  ==============================================================================

    ZoomSettings.cpp
    Created: 24 Jul 2017 4:39:17am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "ZoomSettings.h"
#include "JuceHeader.h"

ZoomSettings::ZoomSettings(ScalingInfo* pScaling) :
	currentCenterPoint(Point3D<float>(0.0, 0.0, 0.0)),
	currentRadius(1.0),
    pointScaler(DEFAULT_POINT_SCALER),
    groupPointScaler(DEFAULT_POINT_SCALER),
    pScalingInfo(pScaling)
{
}

Rectangle<float> ZoomSettings::getVisibleArea(bool isXZ, bool isFull)
{
	if(isXZ)
	{
		Point<float> projectedPoint(currentCenterPoint.getX(), currentCenterPoint.getZ());
		return Rectangle<float>(projectedPoint.getX() - currentRadius, projectedPoint.getY() - (isFull ? currentRadius : 0), 2 * currentRadius, currentRadius * (isFull ? 2 : 1));
	}
	
	Point<float> projectedPoint(currentCenterPoint.getX(), currentCenterPoint.getY());
	return Rectangle<float>(projectedPoint.getX() - currentRadius, projectedPoint.getY() - (isFull ? currentRadius : 0), 2 * currentRadius, currentRadius * (isFull ? 2 : 1));
}

Point3D<float> ZoomSettings::getCurrentCenterPoint() const
{
	return currentCenterPoint;
}

void ZoomSettings::setCurrentCenterPointXY(float x, float y)
{
	currentCenterPoint.setXY(x, y);
	sendChangeMessage();
}

void ZoomSettings::setCurrentCenterPointXZ(float x, float z)
{
	currentCenterPoint.setXZ(x, z);
	sendChangeMessage();
}

double ZoomSettings::getPointScaler()
{
    return pointScaler;
}

void ZoomSettings::setPointScaler(double newScaler)
{
    pointScaler = newScaler;
}

double ZoomSettings::getGroupPointScaler()
{
    return groupPointScaler;
}

void ZoomSettings::setGroupPointScaler(double newScaler)
{
    groupPointScaler = newScaler;
}

float ZoomSettings::getCurrentRadius() const
{
	return currentRadius;
}

void ZoomSettings::setCurrentRadius(float newRadius)
{
	currentRadius = jmax(MIN_ZOOM_RADIUS, newRadius);
	sendChangeMessage();
}

void ZoomSettings::Reset(AmbiDataSet* pDataSet)
{
    currentCenterPoint.setXYZ(0.0, 0.0, 0.0);
    float radius = 1.0f;
    if(pScalingInfo->IsInfinite())
    {
        if(pDataSet != nullptr && pDataSet->size() > 0)
        {
            radius = jmax(1.0f, pDataSet->getMaxDistance());
        }
    }
    else
        radius =(float)pScalingInfo->GetScaler();
    
    setCurrentRadius(radius);
}

int ZoomSettings::getNumberOfRings() const
{
	if (currentRadius < 0.15)
		return 100;
	if (currentRadius < 0.6)
		return 20;
	return 10;
}

float ZoomSettings::getRingResolution() const
{
    if(currentRadius < 0.006)
        return 0.0005f;
    if(currentRadius < 0.015)
        return 0.001f;
    if(currentRadius < 0.06)
        return 0.005f;
    if(currentRadius < 0.15)
        return 0.01f;
    if(currentRadius < 0.6)
        return 0.05f;
    if(currentRadius < 1.5)
        return 0.1f;
    if(currentRadius < 6)
        return 0.5f;
    if(currentRadius < 15)
        return 1.0f;
    if(currentRadius < 60)
        return 5.0f;
    if(currentRadius < 150)
        return 10.0f;
    if(currentRadius < 600)
        return 50.0f;
    if(currentRadius < 1500)
        return 100.0f;
    if(currentRadius < 6000)
        return 500.0f;
    if(currentRadius < 15000)
        return 1000.0f;
    if(currentRadius < 60000)
        return 5000.0f;
    
    return 10000.0f;
}

ScalingInfo* ZoomSettings::getScalingInfo()
{
    return pScalingInfo;
}

void ZoomSettings::loadFromXml(XmlElement* xmlElement)
{
    XmlElement* zoomSettingsXml = xmlElement->getChildByName(XML_TAG_ZOOM_SETTINGS);
    if(zoomSettingsXml != nullptr)
    {
        pointScaler = zoomSettingsXml->getDoubleAttribute(XML_ATTRIBUTE_POINT_SCALER, DEFAULT_POINT_SCALER);
        groupPointScaler = zoomSettingsXml->getDoubleAttribute(XML_ATTRIBUTE_GROUP_POINT_SCALER, DEFAULT_POINT_SCALER);
        currentRadius = (float)zoomSettingsXml->getDoubleAttribute(XML_ATTRIBUTE_RADIUS, DEFAULT_RADIUS);
        currentCenterPoint.setXYZ(
            (float)zoomSettingsXml->getDoubleAttribute(XML_ATTRIBUTE_CENTER_POINT_X, DEFAULT_CENTER_X),
            (float)zoomSettingsXml->getDoubleAttribute(XML_ATTRIBUTE_CENTER_POINT_Y, DEFAULT_CENTER_Y),
            (float)zoomSettingsXml->getDoubleAttribute(XML_ATTRIBUTE_CENTER_POINT_Z, DEFAULT_CENTER_Z)
        );
    }
}

void ZoomSettings::writeToXmlElement(XmlElement* xml) const
{
    XmlElement* zoomSettingsXml = new XmlElement(XML_TAG_ZOOM_SETTINGS);
    zoomSettingsXml->setAttribute(XML_ATTRIBUTE_POINT_SCALER, pointScaler);
    zoomSettingsXml->setAttribute(XML_ATTRIBUTE_GROUP_POINT_SCALER, groupPointScaler);
    zoomSettingsXml->setAttribute(XML_ATTRIBUTE_CENTER_POINT_X, currentCenterPoint.getX());
    zoomSettingsXml->setAttribute(XML_ATTRIBUTE_CENTER_POINT_Y, currentCenterPoint.getY());
    zoomSettingsXml->setAttribute(XML_ATTRIBUTE_CENTER_POINT_Z, currentCenterPoint.getZ());
    zoomSettingsXml->setAttribute(XML_ATTRIBUTE_RADIUS, currentRadius);
    xml->addChildElement(zoomSettingsXml);
}
    
