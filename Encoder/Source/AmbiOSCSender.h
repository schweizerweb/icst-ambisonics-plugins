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
#include "JuceHeader.h"
#include "../../Common/AmbiSourceSet.h"

class AmbiOSCSender : public Timer
{
public:
	AmbiOSCSender(AmbiSourceSet* ambiPoints);
	~AmbiOSCSender() override;

	bool start(String targetHost, int port, int intervalMs);
	void stop();

private:
	void timerCallback() override;

private:
	AmbiSourceSet* pPoints;
	OSCSender* oscSender;
	int sendIntervalMs;
};
