/*
  ==============================================================================

    PointSelection.cpp
    Created: 25 Jul 2017 8:39:25am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "PointSelection.h"

PointSelection::PointSelection() : selectedPointIndex(-1)
{
}

int PointSelection::getSelectedPointIndex() const
{
	return selectedPointIndex;
}

void PointSelection::unselectPoint()
{
	selectedPointIndex = NO_POINT_SELECTED;
	notifyChange();
}

void PointSelection::selectPoint(int index)
{
	selectedPointIndex = index;
	notifyChange();
}

void PointSelection::notifyChange()
{
	sendChangeMessage();
}
