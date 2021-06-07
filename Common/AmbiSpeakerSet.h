/*
  ==============================================================================

    AmbiSpeakerSet.h
    Created: 30 Oct 2019 12:36:17pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiDataSet.h"
#include "AmbiSpeaker.h"

#define XML_TAG_PRESET_POINT "Point"

class AmbiSpeakerSet : public AmbiDataSet
{
public:
    AmbiSpeakerSet(ScalingInfo* pScaling) : AmbiDataSet(pScaling) {};
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
