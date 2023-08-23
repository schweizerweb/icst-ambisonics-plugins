/*
  ==============================================================================

    DistanceEncodingGraph.h
    Created: 22 Nov 2019 1:29:47pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "../../Common/SimpleGraph.h"
#include "../../Common/ZoomSettings.h"
#include "DistanceEncodingParams.h"

#define NB_STEPS 200

class DistanceEncodingGraph : public SimpleGraph
{
public:
	DistanceEncodingGraph(DistanceEncodingParams* params, ZoomSettings* pZoomSettings);
	~DistanceEncodingGraph() override;

	void paintData(Graphics&) override;

private:
	DistanceEncodingParams* pParams;
  ZoomSettings* pZoomSettings;
};
