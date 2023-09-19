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



#include "DistanceEncodingGraph.h"

DistanceEncodingGraph::DistanceEncodingGraph(DistanceEncodingParams* params, ZoomSettings* _pZoomSettings) : pParams(params), pZoomSettings(_pZoomSettings)
{
	setDisplayRange(Linear, Range<double>(0.0, pZoomSettings->getScalingInfo()->CartesianMax()), Linear, Range<double>(-20, 0));
	fullGridFlag = true;
	labelAxisX = "Rel. Distance";
	labelAxisY = "Attenuation [dB]";
}

DistanceEncodingGraph::~DistanceEncodingGraph()
{
}

void DistanceEncodingGraph::paintData(Graphics& g)
{
	Path pathW;
	Path pathOther;
	bool firstW = true;
	bool firstOther = true;

    double w, o;
    auto maxValue = pZoomSettings->getScalingInfo()->CartesianMax();
    pParams->calculateAttenuation(maxValue, &w, &o);
    setDisplayRange(Linear, Range<double>(0.0, maxValue), Linear, Range<double>(jmin(-20.0, Decibels::gainToDecibels(w), Decibels::gainToDecibels(o)), 0));

	for (int step = 0; step <= NB_STEPS; step++)
	{
		double wFactor, otherFactor;
		double distance = 0.00001 + (double)maxValue / (double)NB_STEPS * (double)step;
		pParams->calculateAttenuation(distance, &wFactor, &otherFactor);

		Point<float> displayPointW = mapValues(distance, Decibels::gainToDecibels(wFactor)).toFloat();
		Point<float> displayPointOther = mapValues(distance, Decibels::gainToDecibels(otherFactor)).toFloat();

		if (firstW == true)
		{
			firstW = false;
			pathW.startNewSubPath(displayPointW);

		}
		else
		{
			pathW.lineTo(displayPointW);
		}

		if (firstOther)
		{
			firstOther = false;
			pathOther.startNewSubPath(displayPointOther);
		}
		else
		{
			pathOther.lineTo(displayPointOther);
		}
	}

	g.setColour(Colours::darkgreen);
	g.strokePath(pathW, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));
	g.drawSingleLineText("W-Signal", graphArea->getRight() - 100, graphArea->getY() - 5, Justification::right);

	g.setColour(Colours::darkred);
	g.strokePath(pathOther, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));
	g.drawSingleLineText("Other Signals", graphArea->getRight(), graphArea->getY() - 5, Justification::right);
}
