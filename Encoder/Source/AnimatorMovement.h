/*
  ==============================================================================

    AnimatorMovement.h
    Created: 12 Sep 2022 8:49:11am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#define XML_ATTRIBUTE_MOVEMENT_NAME "Name"
#define XML_ATTRIBUTE_MOVEMENT_POLAR "PolarFlag"
#define XML_ATTRIBUTE_MOVEMENT_TIMESPAN "TimeSpanSeconds"
#define XML_ATTRIBUTE_MOVEMENT_GROUPINDEX "GroupIndex"
#define XML_TAG_MOVEMENTS_POSITIONS "Positions"
#define XML_TAG_MOVEMENTS_POSITION "Position"
#define XML_ATTRIBUTE_MOVEMENT_POS_X "X"
#define XML_ATTRIBUTE_MOVEMENT_POS_Y "Y"
#define XML_ATTRIBUTE_MOVEMENT_POS_Z "Z"

class AnimatorMovement
{
public:
    AnimatorMovement()
    {
        reset();
    }

    AnimatorMovement(XmlElement* xmlElement)
    {
        reset();
        name = xmlElement->getStringAttribute(XML_ATTRIBUTE_MOVEMENT_NAME, "");
        polarFlag = xmlElement->getBoolAttribute(XML_ATTRIBUTE_MOVEMENT_POLAR, false);
        timeSpanSeconds = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_MOVEMENT_TIMESPAN, 0.0);
        XmlElement* xmlPositions = xmlElement->getChildByName(XML_TAG_MOVEMENTS_POSITIONS);
        if (xmlPositions != nullptr)
        {
            XmlElement* i = xmlPositions->getChildByName(XML_TAG_MOVEMENTS_POSITION);
            while (i != nullptr)
            {
                int index = i->getIntAttribute(XML_ATTRIBUTE_MOVEMENT_GROUPINDEX, -1);
                if(index != -1)
                {
                    Point3D<double> p;
                    p.setXYZ(
                             i->getDoubleAttribute(XML_ATTRIBUTE_MOVEMENT_POS_X, 0.0),
                             i->getDoubleAttribute(XML_ATTRIBUTE_MOVEMENT_POS_Y, 0.0),
                             i->getDoubleAttribute(XML_ATTRIBUTE_MOVEMENT_POS_Z, 0.0));
                    groupPositions[index] = p;
                }
                
                i = i->getNextElement();
            }
        }
    }
    
    void reset()
    {
        name = "Preset";
        polarFlag = false;
        timeSpanSeconds = 0.0;
        groupPositions.clear();
    }
    
    XmlElement* getAsXmlElement(String tagName) const
    {
        XmlElement* element = new XmlElement(tagName);

        element->setAttribute(XML_ATTRIBUTE_MOVEMENT_NAME, name);
        element->setAttribute(XML_ATTRIBUTE_MOVEMENT_POLAR, polarFlag);
        element->setAttribute(XML_ATTRIBUTE_MOVEMENT_TIMESPAN, timeSpanSeconds);
        
        XmlElement* xmlPositions = new XmlElement(XML_TAG_MOVEMENTS_POSITIONS);
        for (auto p : groupPositions)
        {
            XmlElement* xmlPosition = new XmlElement(XML_TAG_MOVEMENTS_POSITION);
            xmlPosition->setAttribute(XML_ATTRIBUTE_MOVEMENT_GROUPINDEX, p.first);
            xmlPosition->setAttribute(XML_ATTRIBUTE_MOVEMENT_POS_X, p.second.getX());
            xmlPosition->setAttribute(XML_ATTRIBUTE_MOVEMENT_POS_Y, p.second.getY());
            xmlPosition->setAttribute(XML_ATTRIBUTE_MOVEMENT_POS_Z, p.second.getZ());
            
            xmlPositions->addChildElement(xmlPosition);
        }
        element->addChildElement(xmlPositions);
        
        
        return element;
    }
    
    String name;
    bool polarFlag;
    double timeSpanSeconds;
    std::map<int, Point3D<double>> groupPositions;
};
