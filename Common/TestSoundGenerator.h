/*
  ==============================================================================

    TestSoundGenerator.h
    Created: 16 Oct 2017 10:26:17am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AmbiDataSet.h"
#include "AmbiSettings.h"

#define NO_TEST_SOUND	-1

class AmbiDataSet;

class TestSoundGenerator : public Timer
{
public:
	TestSoundGenerator(AmbiDataSet* speakerSet);

	void process(float* sampleData, int sampleCount, int speakerIndex);
	void toggle(int speakerIndex);
	void toggleAutoTest();
	void reset();
	
private:
	void timerCallback() override;

private:
	bool testSoundChannels[MAX_NUM_CHANNELS];
	Random random;
	AmbiDataSet* pSpeakerSet;
	int tempChannel;
	double testSoundBaseGain;
};
