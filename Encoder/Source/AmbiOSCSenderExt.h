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
#include "../../Common/AmbiDataSet.h"
#include "EncoderSettings.h"
#include "OSCSenderInstance.h"
#include "OscPointScope.h"
#include "../../Common/StatusMessageHandler.h"
#include "../../Common/ScalingInfo.h"

class PointHistoryEntry
{
public:
	bool update(Vector3D<double> absPos, AmbiPoint* pAmbiPoint)
	{
		bool changed = false;

		if(pAmbiPoint->getName() != name)
		{
			name = pAmbiPoint->getName();
			changed = true;
		}
		if(!(absPos - point).lengthIsBelowEpsilon())
		{
			point = absPos;
			changed = true;
		}
		if(!approximatelyEqual(pAmbiPoint->getGain(), gain))
		{
			gain = pAmbiPoint->getGain();
			changed = true;
		}
		if(pAmbiPoint->getColor() != color)
		{
			color = pAmbiPoint->getColor();
			changed = true;
		}
		return changed;
	}

	String name; 
	Vector3D<double> point;
	Colour color;
	double gain;
};

// One connected OSCSenderInstance, plus which point collection(s) it should receive and - for the
// "Select..." scope - which specific indices. Standard targets whose scope includes groups get a
// second binding with the source/sourceindex address swapped to group/groupindex (connectStandardSender()),
// since one OSCSenderInstance only ever holds a single address template; custom targets always get
// exactly one binding regardless of scope, since their template is free-form user text with no
// "source"/"group" substring we could reliably swap.
struct OscSenderBinding
{
    OSCSenderInstance* instance = nullptr;
    bool sendsSources = false;
    bool sendsGroups = false;
    const Array<int>* allowedSourceIndices = nullptr; // non-null => only these indices (Select scope)
    const Array<int>* allowedGroupIndices = nullptr;  // non-null => only these indices (Select scope)
};

class AmbiOSCSenderExt : public Timer
{
public:
	AmbiOSCSenderExt(AmbiDataSet* ambiPoints, StatusMessageHandler* pStatusMessageHandler, ScalingInfo* pScaling);
    virtual ~AmbiOSCSenderExt() override;

	bool start(EncoderSettings* pSettings, String* pMessage);
	void stop();

private:
	void timerCallback() override;
    int connectStandardSender(StandardOscTarget* pTarget, String sourceOscPath, String groupOscPath, String description, String* pMessage);
    OSCSenderInstance* addBoundInstance(bool sendsSources, bool sendsGroups, OscPointScope scope, const OscPointSelection& selection);

private:
	CriticalSection cs;
	AmbiDataSet* pPoints;
	StatusMessageHandler* pStatusMessageHandler;
	OwnedArray<PointHistoryEntry> history;
	OwnedArray<PointHistoryEntry> groupHistory;
	OwnedArray<OSCSenderInstance> oscSenderInstances;
	OwnedArray<OscSenderBinding> senderBindings;
    ScalingInfo* pScalingInfo;
    bool doContinuousUpdate;
};
