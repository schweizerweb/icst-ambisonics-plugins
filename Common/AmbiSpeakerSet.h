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

class AmbiSpeakerSet : public AmbiDataSet
{
public:
	AmbiSpeaker* get(int index) const override;

	void add(AmbiSpeaker* pt);
	int size() const override;
	void clear() override;
	void cleanup(int keepNbOfElements) override;
	void remove(int index) override;
	void swap(int a, int b) override;

	void setSubwooferFlag(int channel, bool flag) const;
	double getMaxNormalizedDistance() const;

	void addNew(String id, Point3D<double> point, String name, Colour color) override;

private:
	OwnedArray<AmbiSpeaker> elements;
	OwnedArray<AmbiSpeaker> removedElements;
};
