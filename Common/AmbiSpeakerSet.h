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
#include "AmbiDataSet.h"
#include "AmbiSpeaker.h"

#define XML_TAG_PRESET_POINTS "Points"
#define XML_TAG_PRESET_POINT "Point"

class AmbiSpeakerSet : public AmbiDataSet
{
public:
    AmbiSpeakerSet(ScalingInfo* pScaling) : AmbiDataSet(pScaling) {}
	AmbiSpeaker* get(int index) const override;

	void add(AmbiSpeaker* pt);
	int size() const override;
	void clear() override;
	void cleanup(int keepNbOfElements) override;
	void remove(int index) override;
	void swap(int a, int b) override;

    void setFilterBypass(int channel, bool byPass) const;
	double getMaxNormalizedDistance() const;

	void addNew(String id, Point3D<double> point, String name, Colour color) override;

    void loadFromXml(XmlElement* xmlElement);
    void writeToXmlElement(XmlElement* xml) const;
    
private:
	OwnedArray<AmbiSpeaker> elements;
	OwnedArray<AmbiSpeaker> removedElements;
};
