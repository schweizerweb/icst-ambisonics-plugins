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

    class PinkNoiseGenerator
    {
    public:
        PinkNoiseGenerator() { std::fill(pink, pink + 7, 0.0f); }

        float getNextSample(float white)
        {
            // Paul Kellet's IIR pinking filter
            pink[0] = 0.99886f * pink[0] + white * 0.0555179f;
            pink[1] = 0.99332f * pink[1] + white * 0.0750759f;
            pink[2] = 0.96900f * pink[2] + white * 0.1538520f;
            pink[3] = 0.86650f * pink[3] + white * 0.3104856f;
            pink[4] = 0.55000f * pink[4] + white * 0.5329522f;
            pink[5] = -0.7616f * pink[5] - white * 0.0168980f;
            float pinkOut = pink[0] + pink[1] + pink[2] + pink[3] + pink[4] + pink[5] + pink[6] + white * 0.5362f;
            pink[6] = white * 0.115926f;
            return pinkOut * 0.11f; // Normalized to avoid clipping
        }
    private:
        float pink[7];
    };

    
private:
	bool testSoundChannels[MAX_NUM_CHANNELS];
    PinkNoiseGenerator pinkNoiseGenerators[MAX_NUM_CHANNELS];
	Random random;
	AmbiDataSet* pSpeakerSet;
	int tempChannel;
	double testSoundBaseGain;
};
