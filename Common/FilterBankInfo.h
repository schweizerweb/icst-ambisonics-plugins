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
#define MAX_FILTER_COUNT 4

#include "FilterInfo.h"
#include "JuceHeader.h"

class FilterBankInfo
{
public:
    FilterInfo* get(int index)
    {
        if (index >= 0 && index < MAX_FILTER_COUNT)
        {
            return &filters[index];
        }

        return nullptr;
    }

    void getCoefficients(double sampleRate, std::vector<dsp::IIR::Coefficients<float>::Ptr>* coefficientList) const
    {
        for(int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            if (filters[i].filterType != FilterInfo::None)
                coefficientList->push_back(filters[i].getCoefficients(sampleRate));
        }
    }

    bool isLowPass() const
    {
        for(int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            if (filters[i].isLowPass())
                return true;
        }

        return false;
    }

    bool anyActive() const
    {
        for(int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            if (filters[i].filterType != FilterInfo::None)
                return true;
        }

        return false;
    }

    bool loadFromXmlElement(XmlElement* xmlElement)
    {
        for (int i = 0; i < MAX_FILTER_COUNT; i++)
            filters[i].loadFromXmlElement(xmlElement, i == 0 ? "" : String(i));

        return true;
    }

    bool writeToXmlElement(XmlElement* xmlElement)
    {
        for (int i = 0; i < MAX_FILTER_COUNT; i++)
            filters[i].writeToXmlElement(xmlElement, i == 0 ? "" : String(i));

        return true;
    }

    void copyFrom(FilterBankInfo* info)
    {
        for (int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            if (info->get(i) != nullptr)
                filters[i].copyFrom(info->get(i));
        }
    }

    bool equals(FilterBankInfo* info)
    {
        if (info == nullptr)
            return false;

        for (int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            if (info->get(i) == nullptr || !filters[i].equals(info->get(i)))
                return false;
        }

        return true;
    }

private:
    FilterInfo filters[MAX_FILTER_COUNT];
};
