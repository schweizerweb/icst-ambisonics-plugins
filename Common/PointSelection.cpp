/*
  ==============================================================================

    PointSelection.cpp
    Created: 25 Jul 2017 8:39:25am
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "PointSelection.h"

PointSelection::PointSelection() : selectionMode(None)
{
}

void PointSelection::unselectPoint()
{
	selectionMode = None;
	selectedIndices.clear();
	notifyChange();
}

void PointSelection::selectPoint(int index, bool add)
{
	if(selectionMode == Group || selectionMode == Point && !add && !selectedIndices.contains(index))
	{
		unselectPoint();
	}
	
	mainSelectedIndex = index;
	selectedIndices.addIfNotAlreadyThere(index);
	selectionMode = Point;
	
	notifyChange();
}

void PointSelection::notifyChange()
{
	sendChangeMessage();
}

bool PointSelection::isPointSelected(int index) const
{
	if (selectionMode != Point)
		return false;

	return selectedIndices.contains(index);
}

bool PointSelection::isGroupSelected(int index) const
{
	if (selectionMode != Group)
		return false;

	return selectedIndices.contains(index);
}

PointSelection::SelectionMode PointSelection::getSelectionMode() const
{
	return selectionMode;
}

int PointSelection::getMainSelectedPointIndex() const
{
	return selectionMode == None ? -1 : mainSelectedIndex;
}

Array<int> PointSelection::getSelectedIndices() const
{
	return Array<int>(selectedIndices);
}

void PointSelection::selectGroup(int index, bool add)
{
	if(selectionMode == Point || selectionMode == Group && !add && !selectedIndices.contains(index))
	{
		unselectPoint();
	}

	selectionMode = Group;
	mainSelectedIndex = index;
	selectedIndices.addIfNotAlreadyThere(index);

	notifyChange();
}
