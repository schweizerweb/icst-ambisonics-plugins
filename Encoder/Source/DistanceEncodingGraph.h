/*
  ==============================================================================

    DistanceEncodingGraph.h
    Created: 22 Nov 2019 1:29:47pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../../Common/SimpleGraph.h"
#include "DistanceEncodingParams.h"
#define NB_STEPS 100

class DistanceEncodingGraph : public SimpleGraph
{
public:
	DistanceEncodingGraph(DistanceEncodingParams* params);
	~DistanceEncodingGraph();

	void paint(Graphics&) override;

private:
	DistanceEncodingParams* pParams;
};
