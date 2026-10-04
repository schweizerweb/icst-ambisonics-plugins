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

#include "AmbiOSCSenderExt.h"
#include "OSCHandlerEncoder.h"

namespace
{
    bool scopeIncludesSources(OscPointScope scope, const OscPointSelection& selection)
    {
        return scope == OscPointScope::AllSources
            || scope == OscPointScope::AllSourcesAndGroups
            || (scope == OscPointScope::Select && !selection.sourceIndices.isEmpty());
    }

    bool scopeIncludesGroups(OscPointScope scope, const OscPointSelection& selection)
    {
        return scope == OscPointScope::AllGroups
            || scope == OscPointScope::AllSourcesAndGroups
            || (scope == OscPointScope::Select && !selection.groupIndices.isEmpty());
    }
}

AmbiOSCSenderExt::AmbiOSCSenderExt(AmbiDataSet* ambiPoints, StatusMessageHandler* _pStatusMessageHandler, ScalingInfo* _pScalingInfo): pPoints(ambiPoints), pStatusMessageHandler(_pStatusMessageHandler), pScalingInfo(_pScalingInfo)
{
}

AmbiOSCSenderExt::~AmbiOSCSenderExt()
{
	stop();
}

OSCSenderInstance* AmbiOSCSenderExt::addBoundInstance(bool sendsSources, bool sendsGroups, OscPointScope scope, const OscPointSelection& selection)
{
    auto* instance = new OSCSenderInstance(pScalingInfo);
    oscSenderInstances.add(instance);

    auto* binding = new OscSenderBinding();
    binding->instance = instance;
    binding->sendsSources = sendsSources;
    binding->sendsGroups = sendsGroups;
    binding->allowedSourceIndices = (scope == OscPointScope::Select) ? &selection.sourceIndices : nullptr;
    binding->allowedGroupIndices = (scope == OscPointScope::Select) ? &selection.groupIndices : nullptr;
    senderBindings.add(binding);

    return instance;
}

bool AmbiOSCSenderExt::start(EncoderSettings* pSettings, String* pMessage)
{
	stop();

    if(!pSettings->oscSendExtMasterFlag)
        return true;

    int successfulCount = 0;

    successfulCount += connectStandardSender(pSettings->oscSendExtXyz.get(),
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ) + " {n} {x} {y} {z}",
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_XYZ) + " {n} {x} {y} {z}",
        "XYZ", pMessage);

    successfulCount += connectStandardSender(pSettings->oscSendExtAed.get(),
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED) + " {n} {a} {e} {d}",
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUP_AED) + " {n} {a} {e} {d}",
        "AED", pMessage);

    successfulCount += connectStandardSender(pSettings->oscSendExtXyzIndex.get(),
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ) + " {i} {x} {y} {z}",
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUPINDEX_XYZ) + " {i} {x} {y} {z}",
        "XYZ (Index)", pMessage);

    successfulCount += connectStandardSender(pSettings->oscSendExtAedIndex.get(),
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED) + " {i} {a} {e} {d}",
        String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_GROUPINDEX_AED) + " {i} {a} {e} {d}",
        "AED (Index)", pMessage);

	for (auto target : pSettings->customOscTargets)
	{
		if (target->enabledFlag)
		{
            bool sendsSources = scopeIncludesSources(target->scope, target->selection);
            bool sendsGroups = scopeIncludesGroups(target->scope, target->selection);
            OSCSenderInstance* pInstance = addBoundInstance(sendsSources, sendsGroups, target->scope, target->selection);

            String errorStringBase = "- custom OSC sender @ " + target->targetHost + ":" + String(target->targetPort) + NewLine::getDefault()  + "  --> ";
            String localErrorString;
            if(!pInstance->setOscPath(target->oscString, &localErrorString))
            {
                pMessage->append(errorStringBase + localErrorString + NewLine::getDefault(), 1000);
                continue;
            }
            if(!pInstance->connect(target->targetHost, target->targetPort))
			{
            	pMessage->append(errorStringBase + "connection failed" + NewLine::getDefault(), 1000);
                continue;
			}

            successfulCount++;
		}
        else
        {
            successfulCount++;
        }
	}

	if (successfulCount > 0)
	{
        doContinuousUpdate = pSettings->oscSendExtContinuousFlag;
		startTimer(pSettings->oscSendExtIntervalMs);
	}

	return successfulCount == (4 + pSettings->customOscTargets.size());
}

void AmbiOSCSenderExt::stop()
{
	stopTimer();
	for (auto* binding : senderBindings)
		binding->instance->disconnect();

    senderBindings.clear();
    oscSenderInstances.clear();
}

int AmbiOSCSenderExt::connectStandardSender(StandardOscTarget* pTarget, String sourceOscPath, String groupOscPath, String description, String* pMessage)
{
    if (!pTarget->enabledFlag)
        return 1;

    bool wantsSources = scopeIncludesSources(pTarget->scope, pTarget->selection);
    bool wantsGroups = scopeIncludesGroups(pTarget->scope, pTarget->selection);
    String errorStringBase = "- standard " + description + " sender @ " + pTarget->targetHost + ":" + String(pTarget->targetPort) + NewLine::getDefault();
    bool ok = true;

    if (wantsSources)
    {
        OSCSenderInstance* pInstance = addBoundInstance(true, false, pTarget->scope, pTarget->selection);
        String localErrorMessage;
        if (!pInstance->setOscPath(sourceOscPath, &localErrorMessage))
        {
            pMessage->append("- Program error for standard sender (" + description + "): " + localErrorMessage + NewLine::getDefault(), 1000);
            ok = false;
        }
        else if (!pInstance->connect(pTarget->targetHost, pTarget->targetPort))
        {
            pMessage->append(errorStringBase, 500);
            ok = false;
        }
    }

    if (wantsGroups)
    {
        OSCSenderInstance* pInstance = addBoundInstance(false, true, pTarget->scope, pTarget->selection);
        String localErrorMessage;
        if (!pInstance->setOscPath(groupOscPath, &localErrorMessage))
        {
            pMessage->append("- Program error for standard sender (" + description + ", groups): " + localErrorMessage + NewLine::getDefault(), 1000);
            ok = false;
        }
        else if (!pInstance->connect(pTarget->targetHost, pTarget->targetPort))
        {
            pMessage->append(errorStringBase, 500);
            ok = false;
        }
    }

    return ok ? 1 : 0;
}

void AmbiOSCSenderExt::timerCallback()
{
	const ScopedTryLock lock(cs);

	// skip if still busy
	if (!lock.isLocked())
		return;

	// create history elements if required
	while (pPoints->size() > history.size())
		history.add(new PointHistoryEntry());
	while (pPoints->groupCount() > groupHistory.size())
		groupHistory.add(new PointHistoryEntry());

	// Sources: "changed" is computed once per point per tick, then fanned out to every binding
	// that wants sources - computing it per-binding instead would make PointHistoryEntry::update()'s
	// side effect (it stores the new value once it reports a change) hide the same change from
	// whichever binding is checked second.
	for (int i = 0; i < pPoints->size(); i++)
	{
		AmbiPoint* pt = pPoints->get(i);
        Vector3D<double> absPt = pPoints->getAbsSourcePoint(i);
		if (pt != nullptr && pt->getEnabled())
		{
            bool changed = doContinuousUpdate || history[i]->update(absPt, pt);
            if (!changed)
                continue;

            for (auto* binding : senderBindings)
            {
                if (!binding->sendsSources) continue;
                if (binding->allowedSourceIndices != nullptr && !binding->allowedSourceIndices->contains(i)) continue;

                try
                {
                    binding->instance->sendMessage(absPt, pt, i);
                }
                catch (...)
                {
                    pStatusMessageHandler->showMessage("Error sending message", "Error creating message for sender " + binding->instance->getOscPath(), StatusMessage::Error);
                }
            }
		}
	}

	// Groups: same pattern, separate history/index space (group indices and source indices are
	// unrelated, so they must not share one history array).
	for (int i = 0; i < pPoints->groupCount(); i++)
	{
		AmbiGroup* grp = pPoints->getGroup(i);
		if (grp != nullptr && grp->getEnabled())
		{
            Vector3D<double> absPt = grp->getVector3D();
            bool changed = doContinuousUpdate || groupHistory[i]->update(absPt, grp);
            if (!changed)
                continue;

            for (auto* binding : senderBindings)
            {
                if (!binding->sendsGroups) continue;
                if (binding->allowedGroupIndices != nullptr && !binding->allowedGroupIndices->contains(i)) continue;

                try
                {
                    binding->instance->sendMessage(absPt, grp, i);
                }
                catch (...)
                {
                    pStatusMessageHandler->showMessage("Error sending message", "Error creating message for sender " + binding->instance->getOscPath(), StatusMessage::Error);
                }
            }
		}
	}
}
