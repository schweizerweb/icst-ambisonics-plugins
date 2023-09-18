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
	if(selectionMode == Group || (selectionMode == Point && !add && !selectedIndices.contains(index)))
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
	if(selectionMode == Point || (selectionMode == Group && !add && !selectedIndices.contains(index)))
	{
		unselectPoint();
	}

	selectionMode = Group;
	mainSelectedIndex = index;
	selectedIndices.addIfNotAlreadyThere(index);

	notifyChange();
}
