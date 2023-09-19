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
