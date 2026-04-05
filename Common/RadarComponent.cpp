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

#include "RadarComponent.h"

RadarComponent::RadarComponent (AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions)
{
    showPointInfo = true;

    radar.reset (new Radar3D (pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (radar.get());
    radar->setName ("radar");

    pointInfo.reset (new PointInfoControl (pEditablePoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (pointInfo.get());
    pointInfo->setName ("pointInfo");

    setSize (600, 400);
}

RadarComponent::~RadarComponent()
{
    radar = nullptr;
    pointInfo = nullptr;
}

void RadarComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void RadarComponent::resized()
{
	if(!showPointInfo)
	{
		radar->setBounds(0, 0, getWidth() - 0, getHeight());
		pointInfo->setBounds(0, 0, 0, 0);
	}
	else
	{
        int pointInfoHeight = pointInfo->getRequiredHeight(getWidth());
        radar->setBounds (0, 0, getWidth() - 0, getHeight() - pointInfoHeight);
        pointInfo->setBounds (0, getHeight() - pointInfoHeight, getWidth() - 0, pointInfoHeight);
    }
}

void RadarComponent::setPointInfoVisible(bool visible)
{
    this->showPointInfo = visible;
    startTimer(100); // pseudo async resize call to avoid OSX crash
}

void RadarComponent::timerCallback()
{
    stopTimer();
    resized();
}
