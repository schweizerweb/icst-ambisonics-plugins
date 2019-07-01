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
	reset();
}

void TestSoundGenerator::process(AudioSampleBuffer* audioBuffer)
{
	for(int iSpeaker = 0; iSpeaker < JucePlugin_MaxNumOutputChannels; iSpeaker++)
	{
		if(testSoundChannels[iSpeaker] || iSpeaker == tempChannel)
		{
			AmbiPoint* p = pSpeakerSet->get(iSpeaker);
			if (p != nullptr)
			{
				double testSoundGain = p->getGain();

				float* channelData = audioBuffer->getWritePointer(iSpeaker);
				for (int i = 0; i < audioBuffer->getNumSamples(); i++)
				{
					channelData[i] = float((random.nextFloat() - 0.5f) * testSoundGain);
				}
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
