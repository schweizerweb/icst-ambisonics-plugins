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
#include "FilterBankInfo.h"
#include "JuceHeader.h"

class FilterControlCallback
{
public:
    virtual bool getBypass(int rowNumber) = 0;
    virtual void setBypass(int rowNumber, bool newValue) = 0;
    virtual FilterBankInfo* getFilterInfo(int rowNumber) = 0;
    virtual dsp::ProcessSpec* getFilterSpecification() = 0;
    virtual void showFilterEditor(int rowNumber, Rectangle<int> screenBounds) = 0;
};

