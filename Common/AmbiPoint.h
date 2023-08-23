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

#define XML_ATTRIBUTE_POINT_ID "Uuid"
#define XML_ATTRIBUTE_POINT_X "X"
#define XML_ATTRIBUTE_POINT_Y "Y"
#define XML_ATTRIBUTE_POINT_Z "Z"
#define XML_ATTRIBUTE_POINT_NAME "Name"
#define XML_ATTRIBUTE_POINT_COLOR_OLD "Color"
#define XML_ATTRIBUTE_POINT_COLOR_NEW "ColorCode"
#define XML_ATTRIBUTE_POINT_GAIN "Gain"
#define XML_ATTRIBUTE_POINT_MUTE "Mute"
#define XML_ATTRIBUTE_POINT_ENABLED "Enabled"
#define FONT_SIZE	20

class AmbiGroup;

class AmbiPoint
{
protected:
	AmbiPoint(AmbiPoint* other, bool copyImage = false);
	AmbiPoint();
	AmbiPoint(String id, Point3D<double> point, String name, Colour color = Colour(), double gain = 1.0, bool mute = false, bool solo = false);
    AmbiPoint(XmlElement* element);
	AmbiPoint(XmlElement* element, AudioParameterSet audioParams);
    AmbiPoint(AudioParameterSet audioParams);
	~AmbiPoint();
	XmlElement* getBaseXmlElement(String tagName);

public:
	Point3D<double>* getRawPoint();
    Vector3D<double> getVector3D();
    
	Colour getColor() const;
	void setColor(Colour newColor);
    
    String getName() const;
	void setName(String newName);
	
    double getGain() const;
	void setGain(double newGain, bool notify);
	
    bool getMute() const;
    void setMute(bool newMute, bool notify);
    
    bool getSolo() const;
    void setSolo(bool newSolo);
    
    String getId();
	void resetId();
	
    bool getEnabled();
    void setEnabled(bool enable);
	
    bool checkAlive(int64 referenceTime, int timeout) const;
	void setAlive(int64 currentTimeMillis);
	
    Image* getLabelImage();
	void ensureLabelImage();
    
    void setGroup(AmbiGroup* pG);
    AmbiGroup* getGroup();
	
    virtual float getDisplayScaler() = 0;
	virtual XmlElement* getAsXmlElement(String tagName) = 0;

    virtual bool allowSolo() const = 0;
    virtual bool allowMute() const = 0;
    
protected:
    AudioParameterSet audioParams;
    
private:
	Colour loadColorAttribute(XmlElement* element);

    String id;
	Point3D<double> point;
	String name;
	Colour color;
	double gain;
    bool mute;
    bool solo;
	int64 lastUpdate = 0;
	Image labelImage;
    bool enabled = true;
    AmbiGroup* pGroup;
};



#endif  // AMBIPOINT_H_INCLUDED
