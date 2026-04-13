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
#include <JuceHeader.h>

class BlauertDirectionalFilter
{
public:
    void prepare (double newSampleRate)
    {
        sampleRate = newSampleRate;
        reset();
        hasLastValues = false;
        update();
    }

    void reset()
    {
        rear1k.reset();
        front350.reset();
        front4k.reset();
        above8k.reset();
        below8k.reset();
        below700.reset();
    }

    void setPosition (float newY, float newZ)
    {
        setValues (newY, newZ, amount);
    }

    void setAmount (float newAmount)
    {
        setValues (y, z, newAmount);
    }

    void setValues (float newY, float newZ, float newAmount)
    {
        newY = juce::jlimit (-1.0f, 1.0f, newY);
        newZ = juce::jlimit (-1.0f, 1.0f, newZ);
        newAmount = juce::jlimit (0.0f, 5.0f, newAmount);

        if (hasLastValues
            && approximatelyEqual (y, newY)
            && approximatelyEqual (z, newZ)
            && approximatelyEqual (amount, newAmount))
            return;

        y = newY;
        z = newZ;
        amount = newAmount;
        update();
    }

    float processSample (float input) noexcept
    {
        float x = input;
        x = rear1k.processSample   (x);
        x = front350.processSample (x);
        x = front4k.processSample  (x);
        x = above8k.processSample  (x);
        x = below8k.processSample  (x);
        x = below700.processSample (x);
        return x;
    }

private:
    using IIR = juce::dsp::IIR::Filter<float>;

    double sampleRate = 44100.0;

    float y = 0.0f;
    float z = 0.0f;
    float amount = 1.0f;

    bool hasLastValues = false;
    float lastY = 0.0f;
    float lastZ = 0.0f;
    float lastAmount = 1.0f;

    IIR rear1k;
    IIR front350;
    IIR front4k;
    IIR above8k;
    IIR below8k;
    IIR below700;

    static bool approximatelyEqual (float a, float b, float epsilon = 1.0e-5f) noexcept
    {
        return std::abs (a - b) <= epsilon;
    }

    void update()
    {
        if (hasLastValues
            && approximatelyEqual (lastY, y)
            && approximatelyEqual (lastZ, z)
            && approximatelyEqual (lastAmount, amount))
            return;

        lastY = y;
        lastZ = z;
        lastAmount = amount;
        hasLastValues = true;

        const auto rearWeight  = juce::jmax (0.0f, -y);
        const auto frontWeight = juce::jmax (0.0f,  y);
        const auto belowWeight = juce::jmax (0.0f, -z);
        const auto aboveWeight = juce::jmax (0.0f,  z);

        const float rearGainDb     = amount * rearWeight  * 4.0f;
        const float front350GainDb = amount * frontWeight * 2.0f;
        const float front4kGainDb  = amount * frontWeight * 4.0f;
        const float above8kGainDb  = amount * aboveWeight * 5.0f;
        const float below8kCutDb   = amount * belowWeight * -4.0f;
        const float below700GainDb = amount * belowWeight * 2.5f;

        rear1k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 1000.0f, 1.0f, juce::Decibels::decibelsToGain (rearGainDb));

        front350.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 350.0f, 0.8f, juce::Decibels::decibelsToGain (front350GainDb));

        front4k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 4000.0f, 1.0f, juce::Decibels::decibelsToGain (front4kGainDb));

        above8k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 8000.0f, 1.0f, juce::Decibels::decibelsToGain (above8kGainDb));

        below8k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 8000.0f, 0.9f, juce::Decibels::decibelsToGain (below8kCutDb));

        below700.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 700.0f, 0.9f, juce::Decibels::decibelsToGain (below700GainDb));
    }
};
