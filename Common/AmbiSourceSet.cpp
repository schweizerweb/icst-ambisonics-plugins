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



#include "AmbiSourceSet.h"
#include "EncoderConstants.h"

AmbiSource* AmbiSourceSet::get(int index) const
{
	return elements[index];
}

int AmbiSourceSet::size() const
{
	return elements.size();
}

void AmbiSourceSet::clear()
{
	const ScopedLock lock(cs);
	while (elements.size() > 0)
	{
		remove(0);
	}
}

void AmbiSourceSet::add(AmbiSource* pt)
{
	const ScopedLock lock(cs);
	elements.add(pt);
}

void AmbiSourceSet::remove(int index)
{
	const ScopedLock lock(cs);

	// rearrange audio parameter
	for (int i = elements.size() - 1; i > index; i--)
	{
		elements[i]->getRawPoint()->setAudioParameterSet(elements[i-1]->getRawPoint()->getAudioParameterSet());
	}
	
	AmbiSource* removedPt = elements.removeAndReturn(index);

	if (removedPt != nullptr)
		removedElements.add(removedPt);
    
    cleanup(100);
}

void AmbiSourceSet::swap(int a, int b)
{
	if (elements.size() > a && elements.size() > b)
	{
		// keep audio parameter for index
		Point3D<double>* pointA = elements.getUnchecked(a)->getRawPoint();
		Point3D<double>* pointB = elements.getUnchecked(b)->getRawPoint();
		
		AudioParameterSet setA = pointA->getAudioParameterSet();
		pointA->setAudioParameterSet(pointB->getAudioParameterSet());
		pointB->setAudioParameterSet(setA);
		elements.swap(a, b);
	}
}


void AmbiSourceSet::cleanup(int keepNbOfElements)
{
	const ScopedLock lock(cs);

	while (removedElements.size() > keepNbOfElements)
	{
		AmbiSource* pt = removedElements.removeAndReturn(0);
		if (pt != nullptr)
			delete pt;
	}
}

void AmbiSourceSet::setChannelXYZExt(String id, String name, double x, double y, double z, float rms, Colour color)
{
    const ScopedLock lock(cs);

	AmbiSource* matchingPt = nullptr;

	for (AmbiSource* pt : elements)
	{
		if (pt != nullptr && pt->getId() == id)
		{
			matchingPt = pt;
			break;
		}
	}

	if (matchingPt == nullptr)
	{
		// try to reactivate deleted point
		for (AmbiSource* pt : removedElements)
		{
			if (pt != nullptr && pt->getId() == id)
			{
				matchingPt = pt;
				break;
			}
		}
		if (matchingPt != nullptr)
		{
			elements.add(matchingPt);
			removedElements.removeObject(matchingPt, false);
		}
		else
			matchingPt = elements.add(new AmbiSource(id, Point3D<double>(0.0, 0.0, 0.0), name, color));
	}

	matchingPt->setName(name);
	matchingPt->getRawPoint()->setXYZ(x, y, z);
	matchingPt->setRms(rms);
	matchingPt->setColor(color);
	matchingPt->setAlive(Time::currentTimeMillis());
}


void AmbiSourceSet::setRms(int channel, float rms, bool onlyIfGreater) const
{
	AmbiSource* pt = elements[channel];

	if (pt != nullptr)
		pt->setRms(rms, onlyIfGreater);
}

void AmbiSourceSet::addNew(String id, Point3D<double> point, String name, Colour color)
{
	elements.add(new AmbiSource(id, point, name, color));
}

void AmbiSourceSet::loadFromXml(XmlElement* xmlElement, AudioParams* pAudioParams)
{
	const ScopedLock lock(cs);

    XmlElement* distanceScalerXml = xmlElement->getChildByName(XML_TAG_DISTANCE_SCALER);
    if (distanceScalerXml != nullptr)
    {
        setDistanceScaler(float(distanceScalerXml->getDoubleAttribute(XML_ATTRIBUTE_FACTOR, DEFAULT_DISTANCE_SCALER)));
    }
    
    XmlElement* masterGainElement = xmlElement->getChildByName(XML_TAG_MASTER_GAIN);
    if (masterGainElement != nullptr)
    {
        setMasterGain(float(masterGainElement->getDoubleAttribute(XML_ATTRIBUTE_VALUE)));
    }
    
    clear();
    XmlElement* sourcesElement = xmlElement->getChildByName(XML_TAG_SOURCES);
	if (sourcesElement != nullptr)
	{
		int index = 0;
		XmlElement* xmlPoint = sourcesElement->getChildByName(XML_TAG_SOURCE);
		while (xmlPoint != nullptr)
		{
			if (pAudioParams != nullptr && pAudioParams->sourceParams.size() > index)
				add(new AmbiSource(xmlPoint, pAudioParams->sourceParams.getUnchecked(index)));
			else
				add(new AmbiSource(xmlPoint, AudioParameterSet()));

			xmlPoint = xmlPoint->getNextElement();
			index++;
		}
        if(pAudioParams != nullptr)
        {
            while (elements.size() < pAudioParams->sourceParams.size())
            {
				String indexString = String(index + 1);
                add(new AmbiSource(pAudioParams->sourceParams.getUnchecked(index), indexString));
                index++;
            }
        }
	}

	// groups
    groups.clear();
    XmlElement* groupsElement = xmlElement->getChildByName(XML_TAG_GROUPS);
	if(groupsElement != nullptr)
	{
		int index = 0;
		XmlElement* xmlGroup = groupsElement->getChildByName(XML_TAG_GROUP);
        // create base type pointer array
        Array<AmbiPoint*> ambiPointPointers;
        for(int i = 0; i < elements.size(); i++)
            ambiPointPointers.add(elements[i]);
		while (xmlGroup != nullptr)
		{
            if(pAudioParams != nullptr && pAudioParams->groupParams.size() > index)
                groups.add(new AmbiGroup(xmlGroup, &ambiPointPointers, pAudioParams->groupParams.getUnchecked(index), pScalingInfo));
            else
                groups.add(new AmbiGroup(xmlGroup, &ambiPointPointers, AudioParameterSet(), pScalingInfo));
            
			xmlGroup = xmlGroup->getNextElement();
			index++;
		}
        if(pAudioParams != nullptr)
        {
            while (groups.size() < pAudioParams->groupParams.size())
            {
                String indexString = String(index + 1);
                AmbiGroup* g = new AmbiGroup(Uuid().toString(), Point3D<double>(0.0, 0.0, 0.0, pAudioParams->groupParams.getUnchecked(index)), "G"+indexString, Colours::orangered, pScalingInfo);
                g->setEnabled(false);
                groups.add(g);
                index++;
            }
        }
	}
}

void AmbiSourceSet::writeToXmlElement(XmlElement* xml) const
{
	const ScopedLock lock(cs);
	
    XmlElement* distanceScalerXml = new XmlElement(XML_TAG_DISTANCE_SCALER);
    distanceScalerXml->setAttribute(XML_ATTRIBUTE_FACTOR, getDistanceScaler());
    xml->addChildElement(distanceScalerXml);
    
    XmlElement* masterGainElement = new XmlElement(XML_TAG_MASTER_GAIN);
    masterGainElement->setAttribute(XML_ATTRIBUTE_VALUE, getMasterGain());
    xml->addChildElement(masterGainElement);
    
	// sources
	XmlElement* sourcesElement = new XmlElement(XML_TAG_SOURCES);
	for (int i = 0; i < size(); i++)
	{
		AmbiPoint* pt = get(i);
		if (pt != nullptr)
			sourcesElement->addChildElement(pt->getAsXmlElement(XML_TAG_SOURCE));
	}

	xml->addChildElement(sourcesElement);

	// groups
	XmlElement* groupsElement = new XmlElement(XML_TAG_GROUPS);
	for(int i = 0; i < groupCount(); i++)
	{
		AmbiGroup* g = getGroup(i);
		if (g != nullptr)
			groupsElement->addChildElement(g->getAsXmlElement(XML_TAG_GROUP));
	}

	xml->addChildElement(groupsElement);
}

void AmbiSourceSet::resetIds()
{
    // set new IDs
    for (int i = 0; i < size(); i++)
        get(i)->resetId();
    for (AmbiGroup* g : groups)
        g->resetId();
}


void AmbiSourceSet::setDistanceScaler(double newDistanceScaler)
{
    distanceScaler = newDistanceScaler;
}

double AmbiSourceSet::getDistanceScaler() const
{
    return distanceScaler;
}

float AmbiSourceSet::getMasterGain() const
{
    return masterGain != nullptr ? masterGain->get() : localMasterGain;
}

bool AmbiSourceSet::setMasterGain(float gainDb)
{
    if (gainDb < EncoderConstants::MasterGainMin || gainDb > EncoderConstants::MasterGainMax)
        return false;

    if (masterGain != nullptr)
        *masterGain = gainDb;
    else
        localMasterGain = gainDb;

    return true;
}

void AmbiSourceSet::initialize(AudioProcessor* pProcessor)
{
    masterGain = new AudioParameterFloat(ParameterID("MasterGain", Constants::audioParamVersion), "MasterGain", NormalisableRange<float>(EncoderConstants::MasterGainMin, EncoderConstants::MasterGainMax), localMasterGain, AudioParameterFloatAttributes().withLabel("Master Gain for B-Format output"));
    
    pProcessor->addParameter(masterGain);

    masterGain->addListener(this);
}

void AmbiSourceSet::parameterValueChanged(int /*parameterIndex*/, float /*newValue*/)
{
    sendChangeMessage();
}

void AmbiSourceSet::parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/)
{
}
