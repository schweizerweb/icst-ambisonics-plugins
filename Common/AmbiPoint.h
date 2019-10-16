/*
  ==============================================================================

    AmbiPoint.h
    Created: 3 Sep 2016 11:29:07am
    Author:  Christian Schweizer

  ==============================================================================
*/

#ifndef AMBIPOINT_H_INCLUDED
#define AMBIPOINT_H_INCLUDED

#include "Point3D.h"

#define XML_ATTRIBUTE_PRESET_POINT_ID "Uuid"
#define XML_ATTRIBUTE_PRESET_POINT_X "X"
#define XML_ATTRIBUTE_PRESET_POINT_Y "Y"
#define XML_ATTRIBUTE_PRESET_POINT_Z "Z"
#define XML_ATTRIBUTE_PRESET_POINT_NAME "Name"
#define XML_ATTRIBUTE_PRESET_POINT_COLOR "Color"
#define XML_ATTRIBUTE_PRESET_POINT_GAIN "Gain"
#define FONT_SIZE	20

class AmbiPoint
{
public:
	AmbiPoint(AmbiPoint* other, bool copyImage = false);
	AmbiPoint();
	AmbiPoint(String id, Point3D<double> point, String name, Colour color = Colour(), double gain = 1.0);
	AmbiPoint(Point3D<double> point, AudioParameterSet audioParams, String name, Colour color = Colour(), double gain = 1.0);
	AmbiPoint(XmlElement* element);
	AmbiPoint(XmlElement* element, AudioParameterSet audioParams);
	~AmbiPoint();

	Point3D<double>* getPoint();
	Colour getColor() const;
	String getName() const;
	void setName(String newName);
	double getGain() const;
	XmlElement* getAsXmlElement(String tagName);
	void setGain(double newGain);
	void setRms(float newRmsLevel, bool onlyIfGreater = false);
	float getRms() const;
	String getId();
	void setColor(Colour newColor);
    void setSubwooferFlag(bool flag);
    bool getSubwooferFlag();

	bool checkAlive(int64 referenceTime, int timeout) const;
	void setAlive(int64 currentTimeMillis);
	Image* getLabelImage();
	void ensureLabelImage();

private:
	String id;
	Point3D<double> point;
	Colour color;
	String name;
	double gain;
    bool isSubwoofer;
	float rms;
	int64 lastUpdate = 0;
	Image labelImage;
};



#endif  // AMBIPOINT_H_INCLUDED
