/*
  ==============================================================================

    PointSelection.h
    Created: 25 Jul 2017 8:39:25am
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PointSelection : public ChangeBroadcaster
{
public:
	enum SelectionMode { None, Point, Group };

	PointSelection();

	void unselectPoint();
	void selectPoint(int index, bool add = false);
	void notifyChange();
	bool isPointSelected(int index) const;
	bool isGroupSelected(int index) const;
	SelectionMode getSelectionMode() const;
	int getMainSelectedPointIndex() const;
	Array<int> getSelectedIndices() const;
	void selectGroup(int index, bool add);

private:
	Array<int> selectedIndices;
	int mainSelectedIndex;
	SelectionMode selectionMode;
};