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
#define XML_ATTRIBUTE_ACTION_NAME   "Name"
#define XML_ATTRIBUTE_ACTION_ENABLED    "Enabled"
#define XML_ATTRIBUTE_ACTION_GROUPINDEX     "GroupIndex"
#define XML_ATTRIBUTE_ACTION_ROTATIONX  "RotationX"
#define XML_ATTRIBUTE_ACTION_ROTATIONY  "RotationY"
#define XML_ATTRIBUTE_ACTION_ROTATIONZ  "RotationZ"
#define XML_ATTRIBUTE_ACTION_ROTATIONORIGINX  "RotationOriginX"
#define XML_ATTRIBUTE_ACTION_ROTATIONORIGINY  "RotationOriginY"
#define XML_ATTRIBUTE_ACTION_ROTATIONORIGINZ  "RotationOriginZ"
#define XML_ATTRIBUTE_ACTION_STRETCH  "Stretch"


class AnimatorAction
{
public:
    AnimatorAction()
    {
        reset();
    }

    AnimatorAction(XmlElement* xmlElement)
    {
        reset();
        name = xmlElement->getStringAttribute(XML_ATTRIBUTE_ACTION_NAME, "");
        enabled = xmlElement->getBoolAttribute(XML_ATTRIBUTE_ACTION_ENABLED, false);
        groupIndex = xmlElement->getIntAttribute(XML_ATTRIBUTE_ACTION_GROUPINDEX, -1);
        rotationX = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_ROTATIONX, 0.0);
        rotationY = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_ROTATIONY, 0.0);
        rotationZ = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_ROTATIONZ, 0.0);
        rotationOriginX = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_ROTATIONORIGINX, 0.0);
        rotationOriginY = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_ROTATIONORIGINY, 0.0);
        rotationOriginZ = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_ROTATIONORIGINZ, 0.0);
        stretch = xmlElement->getDoubleAttribute(XML_ATTRIBUTE_ACTION_STRETCH, 0.0);
    }
    
    void reset()
    {
        name = "Action";
        enabled = false;
        groupIndex = -1;
        rotationX = 0.0;
        rotationY = 0.0;
        rotationZ = 0.0;
        rotationOriginX = 0.0;
        rotationOriginY = 0.0;
        rotationOriginZ = 0.0;
        stretch = 0.0;
    }
    
    XmlElement* getAsXmlElement(String tagName) const
    {
        XmlElement* element = new XmlElement(tagName);

        element->setAttribute(XML_ATTRIBUTE_ACTION_NAME, name);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ENABLED, enabled);
        element->setAttribute(XML_ATTRIBUTE_ACTION_GROUPINDEX, groupIndex);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ROTATIONX, rotationX);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ROTATIONY, rotationY);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ROTATIONZ, rotationZ);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ROTATIONORIGINX, rotationOriginX);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ROTATIONORIGINY, rotationOriginY);
        element->setAttribute(XML_ATTRIBUTE_ACTION_ROTATIONORIGINZ, rotationOriginZ);
        element->setAttribute(XML_ATTRIBUTE_ACTION_STRETCH, stretch);
        
        return element;
    }
    
    String name;
    bool enabled;
    int groupIndex;
    double rotationX;
    double rotationY;
    double rotationZ;
    double rotationOriginX;
    double rotationOriginY;
    double rotationOriginZ;
    double stretch;
};
