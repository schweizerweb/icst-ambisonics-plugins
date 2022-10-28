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

#define XML_TAG_GROUP_MODE "GroupMode"
#define XML_TAG_SOURCES "Sources"
#define XML_TAG_SOURCE "Source"
#define XML_TAG_DISTANCE_SCALER "DistanceScaler"
#define XML_TAG_MASTER_GAIN "MasterGain"
#define XML_ATTRIBUTE_ENABLE "Enable"
#define XML_ATTRIBUTE_FACTOR "Factor"
#define XML_ATTRIBUTE_VALUE "Value"

#define DEFAULT_DISTANCE_SCALER    1.0
#define DEFAULT_MASTER_GAIN        0.0f

class AmbiSourceSet : public AmbiDataSet, public AudioProcessorParameter::Listener, public ChangeBroadcaster
{
public:
    AmbiSourceSet(ScalingInfo* pScaling) : AmbiDataSet(pScaling), distanceScaler(DEFAULT_DISTANCE_SCALER), masterGain(nullptr), localMasterGain(DEFAULT_MASTER_GAIN)
    {};
    
    void initialize(AudioProcessor* pProcessor);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

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
    
    double getDistanceScaler() const;
    void setDistanceScaler(double newDistanceScaler);
    
    
    float getMasterGain() const;
    bool setMasterGain(float gainDb);
    
private:
	OwnedArray<AmbiSource> elements;
	OwnedArray<AmbiSource> removedElements;
    
    double distanceScaler;
    AudioParameterFloat* masterGain;
    float localMasterGain;
};
