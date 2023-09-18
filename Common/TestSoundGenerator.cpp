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
	memset(testSoundChannels, 0, MAX_NUM_CHANNELS);
}

void TestSoundGenerator::timerCallback()
{
	tempChannel = (tempChannel + 1) % pSpeakerSet->size();
}
