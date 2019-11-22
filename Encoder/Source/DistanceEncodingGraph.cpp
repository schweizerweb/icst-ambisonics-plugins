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

void DistanceEncodingGraph::paint(Graphics& g)
{
	SimpleGraph::paint(g);

	Path pathW;
	Path pathOther;

	for(int step = 1; step <= NB_STEPS; step++)
	{
		double wFactor, otherFactor;
		double distance = 1.0 / NB_STEPS * step;
		pParams->calculateAttenuation(distance, &wFactor, &otherFactor);

		Point<float> displayPointW = mapValues(distance, Decibels::gainToDecibels(wFactor)).toFloat();
		Point<float> displayPointOther = mapValues(distance, Decibels::gainToDecibels(otherFactor)).toFloat();

		if (step == 1)
		{
			pathW.startNewSubPath(displayPointW);
			pathOther.startNewSubPath(displayPointOther);
		}
		else
		{
			pathW.lineTo(displayPointW);
			pathOther.lineTo(displayPointOther);
		}
	}

	g.setColour(Colours::darkgreen);
	g.strokePath(pathW, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));
	
	g.setColour(Colours::darkred);
	g.strokePath(pathOther, PathStrokeType(3, PathStrokeType::curved, PathStrokeType::rounded));
}
