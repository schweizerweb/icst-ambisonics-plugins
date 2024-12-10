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
#define MUTE_ALPHA 0.5f
#define LABEL_COLOR Colours::black

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
    Image* getLabelImage(Colour col);
	void ensureLabelImage();
	void ensureLabelImage(Colour col);
    
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
    Colour labelColor = LABEL_COLOR;
	double gain;
    bool mute;
    bool solo;
	int64 lastUpdate = 0;
	Image labelImage;
    bool enabled = true;
    AmbiGroup* pGroup;
};
