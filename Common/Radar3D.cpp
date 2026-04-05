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

#include "Radar3D.h"

Radar3D::Radar3D (AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions_) : pRadarOptions(pRadarOptions_)
{
    xzRadar.reset (new Radar2D (Radar2D::XZ_Half, pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (xzRadar.get());
    xzRadar->setName ("xzRadar");

    xzRadar->setBounds (0, 240, 336, 120);

    xyRadar.reset (new Radar2D (Radar2D::XY, pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (xyRadar.get());
    xyRadar->setName ("xyRadar");

    xyRadar->setBounds (0, 0, 336, 240);

    setSize (600, 400);


    xyRadar->addMouseListener(this, true);
    xzRadar->addMouseListener(this, true);
    pRadarOptions->zoomSettings->addChangeListener(this);
}

Radar3D::~Radar3D()
{
    pRadarOptions->zoomSettings->removeChangeListener(this);
    xyRadar->removeMouseListener(this);
    xzRadar->removeMouseListener(this);

    xzRadar = nullptr;
    xyRadar = nullptr;
}

void Radar3D::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void Radar3D::resized()
{
    if(getWidth() > getHeight())
    {
        xzRadar->setRadarMode(Radar2D::XZ_Full);
        xyRadar->setAnchor(Radar2D::X_Right, Radar2D::Y_Center);
        xzRadar->setAnchor(Radar2D::X_Left, Radar2D::Y_Center);
        xyRadar->setBounds(0, 0, getWidth()/2, getHeight());
        xzRadar->setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
    }
    else
    {
        bool fullRadarFlag = pRadarOptions->zoomSettings->fullSphere;
        xzRadar->setRadarMode(fullRadarFlag ? Radar2D::XZ_Full : Radar2D::XZ_Half);
        int secondRadarHeight = int((getHeight()) / (fullRadarFlag ? 2.0 : 3.0));
        int topRadarHeight = fullRadarFlag ? secondRadarHeight : secondRadarHeight * 2;
        xyRadar->setAnchor(Radar2D::X_Center, Radar2D::Y_Bottom);
        xzRadar->setAnchor(Radar2D::X_Center, Radar2D::Y_Top);
        xyRadar->setBounds(0, 0, getWidth(), topRadarHeight);
        xzRadar->setBounds(0, topRadarHeight, getWidth(), secondRadarHeight);
    }
}

void Radar3D::mouseEnter (const juce::MouseEvent& /*e*/)
{
    xyRadar->setRefreshRate(ACTIVE_REFRESH_RATE);
    xzRadar->setRefreshRate(ACTIVE_REFRESH_RATE);
}

void Radar3D::mouseExit (const juce::MouseEvent& /*e*/)
{
    xyRadar->setRefreshRate(INACTIVE_REFRESH_RATE);
    xzRadar->setRefreshRate(INACTIVE_REFRESH_RATE);
}

void Radar3D::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    if(xzRadar->isFullRadar() != pRadarOptions->zoomSettings->fullSphere)
    {
        xzRadar->setRadarMode(pRadarOptions->zoomSettings->fullSphere ? Radar2D::XZ_Full : Radar2D::XZ_Half);
        resized();
    }
}
