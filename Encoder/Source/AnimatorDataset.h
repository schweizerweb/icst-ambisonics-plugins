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
#include <JuceHeader.h>
#include "AnimatorAction.h"
#include "AnimatorMovement.h"

#define XML_TAG_ACTIONS "Actions"
#define XML_TAG_ACTION "Action"
#define XML_TAG_MOVEMENTS "Movements"
#define XML_TAG_MOVEMENT "Movement"



class AnimatorDataset
{
public:
    OwnedArray<AnimatorAction> actions;
    OwnedArray<AnimatorMovement> movements;
    
    XmlElement* getAsXmlElement(String tagName) const
    {
        XmlElement* element = new XmlElement(tagName);

        XmlElement* xmlActions = new XmlElement(XML_TAG_ACTIONS);
        for (auto i : actions)
        {
            xmlActions->addChildElement(i->getAsXmlElement(XML_TAG_ACTION));
        }
        element->addChildElement(xmlActions);
        
        XmlElement* xmlMovements = new XmlElement(XML_TAG_MOVEMENTS);
        for (auto i : movements)
        {
            xmlMovements->addChildElement(i->getAsXmlElement(XML_TAG_MOVEMENT));
        }
        element->addChildElement(xmlMovements);
        
        return element;
    }
    
    void loadFromXml(XmlElement* element)
    {
        if (element == nullptr)
            return;

        actions.clear();
        movements.clear();
        
        XmlElement* xmlActions = element->getChildByName(XML_TAG_ACTIONS);
        if (xmlActions != nullptr)
        {
            XmlElement* i = xmlActions->getChildByName(XML_TAG_ACTION);
            while (i != nullptr)
            {
                actions.add(new AnimatorAction(i));

                i = i->getNextElement();
            }
        }
        
        XmlElement* xmlMovements = element->getChildByName(XML_TAG_MOVEMENTS);
        if (xmlMovements != nullptr)
        {
            XmlElement* i = xmlMovements->getChildByName(XML_TAG_MOVEMENT);
            while (i != nullptr)
            {
                movements.add(new AnimatorMovement(i));

                i = i->getNextElement();
            }
        }
    }
};
