/*
  ==============================================================================

    TestSoundGenerator.cpp
    Created: 16 Oct 2017 10:26:17am
    Author:  chris

  ==============================================================================
*/

#include "TestSoundGenerator.h"

TestSoundGenerator::TestSoundGenerator(): testSoundChannel(NO_TEST_SOUND)
{
}

void TestSoundGenerator::process(AudioSampleBuffer* audioBuffer)
{
	if (testSoundChannel != NO_TEST_SOUND)
	{
		float* channelData = audioBuffer->getWritePointer(testSoundChannel);
		for (int i = 0; i < audioBuffer->getNumSamples(); i++)
		{
			channelData[i] = random.nextFloat() * 0.25f - 0.125f;
		}
	}
}

void TestSoundGenerator::timerCallback()
{
	stopTimer();
	testSoundChannel = NO_TEST_SOUND;
}

void TestSoundGenerator::startSpeakerTest(int channelNb)
{
	stopTimer();
	startTimer(2000);
	testSoundChannel = channelNb;
}

void TestSoundGenerator::actionListenerCallback(const String& message)
{
	int speakerId = message.getTrailingIntValue();
	if (speakerId >= 0)
		startSpeakerTest(speakerId);
}
