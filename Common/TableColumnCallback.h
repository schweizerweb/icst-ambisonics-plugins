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



#pragma once
#include "SliderRange.h"
#include "EnablementCallback.h"
#include "JuceHeader.h"

class TableColumnCallback : public EnablementCallback
{
public:
	virtual ~TableColumnCallback() = default;
	virtual double getValue(int columnId, int rowNumber) = 0;
	virtual void setValue(int columnId, int rowNumber, double newValue) = 0;
	virtual SliderRange getSliderRange(int columnId) = 0;
	virtual TableListBox* getTable() = 0;
	virtual String getTableText(const int columnId, const int rowNumber) = 0;
	virtual void setTableText(const int columnId, const int rowNumber, const String& newText) = 0;
};
