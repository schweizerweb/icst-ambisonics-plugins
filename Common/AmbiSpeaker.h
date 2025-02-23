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
#include "AmbiPoint.h"
#include "FilterBankInfo.h"

class AmbiSpeaker : public AmbiPoint
{
public:
	
	virtual ~AmbiSpeaker() = default;
	AmbiSpeaker(AmbiSpeaker* other, bool copyImage = false);
	AmbiSpeaker(String id, Point3D<double> _point, String _name, Colour _color = Colour(), double _gain = 1.0);
	AmbiSpeaker(XmlElement* element);

	FilterBankInfo* getFilterInfo();
    bool getFilterBypass() const;
    void setFilterBypass(bool byPass);
    
	float getDisplayScaler() override;
	XmlElement* getAsXmlElement(String tagName) override;

    bool allowMute() const override { return true; }
    bool allowSolo() const override { return true; }
    
private:
	FilterBankInfo filterInfo;
};
