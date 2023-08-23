/*
  ==============================================================================

    AmbiOSCSenderExt.cpp
    Created: 2 Jul 2019 1:15:15pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiOSCSenderExt.h"
#include "OSCHandlerEncoder.h"

AmbiOSCSenderExt::AmbiOSCSenderExt(AmbiDataSet* ambiPoints, StatusMessageHandler* _pStatusMessageHandler, ScalingInfo* _pScalingInfo): pPoints(ambiPoints), pStatusMessageHandler(_pStatusMessageHandler), pScalingInfo(_pScalingInfo)
{
}

AmbiOSCSenderExt::~AmbiOSCSenderExt()
{
	stop();
	oscSender.clear();
}

OSCSenderInstance* AmbiOSCSenderExt::getOrCreateInstance(int index)
{
	while(index >= oscSender.size())
	{
		auto newInstance = new OSCSenderInstance(pScalingInfo);
		oscSender.add(newInstance);
	}

	return oscSender[index];
}

bool AmbiOSCSenderExt::start(EncoderSettings* pSettings, String* pMessage)
{
	stop();

    if(!pSettings->oscSendExtMasterFlag)
        return true;
    
    int successfulCount = 0;
	int index = 0;

    successfulCount += connectStandardSender(&index, pSettings->oscSendExtXyz.get(), String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ) + " {n} {x} {y} {z}", "XYZ", pMessage);
	
    successfulCount += connectStandardSender(&index, pSettings->oscSendExtAed.get(), String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED) + " {n} {a} {e} {d}", "AED", pMessage);
    
    successfulCount += connectStandardSender(&index, pSettings->oscSendExtXyzIndex.get(), String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_XYZ) + " {i} {x} {y} {z}", "XYZ (Index)", pMessage);
    
    successfulCount += connectStandardSender(&index, pSettings->oscSendExtAedIndex.get(), String(OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_INDEX_AED) + " {i} {a} {e} {d}", "AED (Index)", pMessage);
    
	for (auto target : pSettings->customOscTargets)
	{
		if (target->enabledFlag)
		{
			OSCSenderInstance* pInstance = getOrCreateInstance(index++);
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
	for (auto sender : oscSender)
	{
		sender->disconnect();
	}

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
		
	for (int i = 0; i < pPoints->size(); i++)
	{
		AmbiPoint* pt = pPoints->get(i);
        Vector3D<double> absPt = pPoints->getAbsSourcePoint(i);
		if (pt != nullptr && pt->getEnabled())
		{
			if (doContinuousUpdate || history[i]->update(absPt, pt))
			{
				for (auto sender : oscSender)
				{
                    try
                    {
						sender->sendMessage(absPt, pt, i);
                    }
                    catch (...)
                    {
						pStatusMessageHandler->showMessage("Error sending message", "Error creating message for sender " + sender->getOscPath(), StatusMessage::Error);
                    }
					
				}
			}
		}
	}
}

int AmbiOSCSenderExt::connectStandardSender(int *pIndex, StandardOscTarget *pTarget, String oscString, String description, String* pMessage)
{
    if (pTarget->enabledFlag)
    {
        OSCSenderInstance* pInstance = getOrCreateInstance((*pIndex)++);
        String localErrorMessage;
        bool ret = pInstance->setOscPath(oscString, &localErrorMessage);
        if(!ret)
        {
            pMessage->append("- Program error for standard sender (" + description + "): " + localErrorMessage + NewLine::getDefault(), 1000);
            return 0;
        }
        
        ret = pInstance->connect(pTarget->targetHost, pTarget->targetPort);
        if (!ret) {
            pMessage->append("- standard " + description + " sender @ " + pTarget->targetHost + ":" + String(pTarget->targetPort) + NewLine::getDefault(), 500);
            return 0;
        }
    }
    
    return 1;
}

