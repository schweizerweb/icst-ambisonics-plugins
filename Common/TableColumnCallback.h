/*
  ==============================================================================

    NumericColumnCallback.h
    Created: 2 Nov 2019 1:00:36pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "SliderRange.h"
#include "JuceHeader.h"

class TableColumnCallback
{
public:
	virtual ~TableColumnCallback() = default;
	virtual double getValue(int columnId, int rowNumber) = 0;
	virtual void setValue(int columnId, int rowNumber, double newValue) = 0;
	virtual SliderRange getSliderRange(int columnId) = 0;
	virtual TableListBox* getTable() = 0;
	virtual String getTableText(const int columnId, const int rowNumber) = 0;
	virtual void setTableText(const int columnId, const int rowNumber, const String& newText) = 0;
    virtual bool getEnabled(const int columnId, const int rowNumber) = 0;

};
