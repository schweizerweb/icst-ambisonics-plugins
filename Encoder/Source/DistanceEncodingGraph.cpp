/*
  ==============================================================================

    DistanceEncodingGraph.cpp
    Created: 22 Nov 2019 1:29:47pm
    Author:  chris

  ==============================================================================
*/

#include "DistanceEncodingGraph.h"
#include "../../Common/Globals.h"

DistanceEncodingGraph::DistanceEncodingGraph(DistanceEncodingParams* params) : pParams(params)
{
	setDisplayRange(Linear, Range<double>(0.0, Globals::CartesianMax()), Linear, Range<double>(-20, 0));
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
    pParams->calculateAttenuation(Globals::CartesianMax(), &w, &o);
    setDisplayRange(Linear, Range<double>(0.0, Globals::CartesianMax()), Linear, Range<double>(jmin(-20.0, Decibels::gainToDecibels(w), Decibels::gainToDecibels(o)), 0));

	for (int step = 0; step <= NB_STEPS; step++)
	{
		double wFactor, otherFactor;
		double distance = 0.00001 + Globals::CartesianMax() / NB_STEPS * step;
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
