/*
  ==============================================================================

    TestSoundGenerator.cpp
    Created: 16 Oct 2017 10:26:17am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "TestSoundGenerator.h"

TestSoundGenerator::TestSoundGenerator(AmbiDataSet* speakerSet): tempChannel(NO_TEST_SOUND)
{
	pSpeakerSet = speakerSet;
	testSoundBaseGain = Decibels::decibelsToGain(-12.0);
	reset();
}

void TestSoundGenerator::process(float* sampleData, int sampleCount, int speakerIndex)
{
	if(testSoundChannels[speakerIndex] || speakerIndex == tempChannel)
	{
		AmbiPoint* p = pSpeakerSet->get(speakerIndex);
		if (p != nullptr)
		{
			double testSoundGain = p->getGain();
			for (int i = 0; i < sampleCount; i++)
			{
				sampleData[i] = float((random.nextFloat() * 2.0f - 1.0f) * testSoundGain * testSoundBaseGain);
			}
		}
	}
}

void TestSoundGenerator::toggle(int speakerIndex)
{
	testSoundChannels[speakerIndex] = !testSoundChannels[speakerIndex];
}

void TestSoundGenerator::toggleAutoTest()
{
	if(isTimerRunning())
	{
		stopTimer();
		tempChannel = NO_TEST_SOUND;
	}
	else
	{
		tempChannel = 0;
		startTimer(1000);
	}
}

void TestSoundGenerator::reset()
{
	stopTimer();
	tempChannel = NO_TEST_SOUND;
	memset(testSoundChannels, 0, JucePlugin_MaxNumOutputChannels);
}

void TestSoundGenerator::timerCallback()
{
	tempChannel = (tempChannel + 1) % pSpeakerSet->size();
}
