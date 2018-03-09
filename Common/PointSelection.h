/*
  ==============================================================================

    PointSelection.h
    Created: 25 Jul 2017 8:39:25am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#define NO_POINT_SELECTED -1

class PointSelection : public ChangeBroadcaster
{
public:
	PointSelection();
	int getSelectedPointIndex() const;
	void unselectPoint();
	void selectPoint(int index);
	void notifyChange();

private:
	int selectedPointIndex;
};