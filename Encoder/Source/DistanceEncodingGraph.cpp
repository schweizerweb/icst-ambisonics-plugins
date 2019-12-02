/*
  ==============================================================================

    DistanceEncodingGraph.cpp
    Created: 22 Nov 2019 1:29:47pm
    Author:  chris

  ==============================================================================
*/

#include "DistanceEncodingGraph.h"

DistanceEncodingGraph::DistanceEncodingGraph(DistanceEncodingParams* params) : pParams(params)
{
	setDisplayRange(Linear, Range<double>(0.0, 1.0), Linear, Range<double>(-20, 0));
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

	for (int step = 0; step <= NB_STEPS; step++)
	{
		double wFactor, otherFactor;
		double distance = 0.00001 + 1.0 / NB_STEPS * step;
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
