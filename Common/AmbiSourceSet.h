/*
  ==============================================================================

    AmbiSourceSet.h
    Created: 30 Oct 2019 12:36:29pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "AmbiDataSet.h"
#include "AmbiSource.h"
#include "AudioParams.h"

#define XML_TAG_SOURCES "Sources"
#define XML_TAG_SOURCE "Source"
#define XML_TAG_GROUPS "Groups"
#define XML_TAG_GROUP "Group"

class AmbiSourceSet : public AmbiDataSet
{
public:
    AmbiSourceSet(ScalingInfo* pScaling) : AmbiDataSet(pScaling) {};
	AmbiSource* get(int index) const override;

	void add(AmbiSource* pt);
	int size() const override;
	void clear() override;
	void cleanup(int keepNbOfElements) override;
	void remove(int index) override;
	void swap(int a, int b) override;

	void setChannelXYZExt(String id, String name, double x, double y, double z, float rms, Colour color);
	void setRms(int channel, float rms, bool onlyIfGreater) const;

	void addNew(String id, Point3D<double> point, String name, Colour color) override;
	void loadFromXml(XmlElement* xmlElement, AudioParams* pAudioParams);
	void writeToXmlElement(XmlElement* xml) const;
    void resetIds();
    
    bool anySolo() const;
    
private:
	OwnedArray<AmbiSource> elements;
	OwnedArray<AmbiSource> removedElements;
};
