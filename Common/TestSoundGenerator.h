/*
  ==============================================================================

    TestSoundGenerator.h
    Created: 16 Oct 2017 10:26:17am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#define NO_TEST_SOUND	-1

class TestSoundGenerator : public Timer, public ActionListener
{
public:
	TestSoundGenerator();

	void process(AudioSampleBuffer* audioBuffer);

private:
	void timerCallback() override;
	void startSpeakerTest(int channelNb);
	void actionListenerCallback(const String& message) override;

private:
	int testSoundChannel;
	Random random;
};