/*
  ==============================================================================

    AmbiOSCSenderExt.cpp
    Created: 2 Jul 2019 1:15:15pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "AmbiOSCSenderExt.h"
#include "OSCHandlerEncoder.h"

AmbiOSCSenderExt::AmbiOSCSenderExt(AmbiDataSet* ambiPoints): pPoints(ambiPoints)
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
		auto newInstance = new OSCSenderInstance();
		oscSender.add(newInstance);
	}

	return oscSender[index];
}

bool AmbiOSCSenderExt::start(EncoderSettings* pSettings, String* pMessage)
{
	stop();

	bool hasErrors = false;
	bool hasSuccessful = false;
	int index = 0;

	if (pSettings->oscSendExtXyzFlag)
	{
		OSCSenderInstance* pInstance = getOrCreateInstance(index++);
		StringArray a = { OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_XYZ,
			pInstance->escapeStringMap[OSCSenderInstance::Name],
			pInstance->escapeStringMap[OSCSenderInstance::X],
			pInstance->escapeStringMap[OSCSenderInstance::Y],
			pInstance->escapeStringMap[OSCSenderInstance::Z]};

		pInstance->setOscPath(a.joinIntoString(" "));
		bool ret = pInstance->connect(pSettings->oscSendExtXyzHost, pSettings->oscSendExtXyzPort);
		if (!ret) {
			pMessage->append("Error initializing standard XYZ sender @ " + pSettings->oscSendExtXyzHost + ":" + String(pSettings->oscSendExtXyzPort) + NewLine::getDefault(), 500);
			hasErrors = true;
		}
		else
		{
			hasSuccessful = true;
		}
	}

	if (pSettings->oscSendExtAedFlag)
	{
		OSCSenderInstance* pInstance = getOrCreateInstance(index++);
		StringArray a = { OSC_ADDRESS_AMBISONIC_PLUGINS_EXTERN_AED,
			pInstance->escapeStringMap[OSCSenderInstance::Name],
			pInstance->escapeStringMap[OSCSenderInstance::A],
			pInstance->escapeStringMap[OSCSenderInstance::E],
			pInstance->escapeStringMap[OSCSenderInstance::D]};

		pInstance->setOscPath(a.joinIntoString(" "));

		bool ret = pInstance->connect(pSettings->oscSendExtAedHost, pSettings->oscSendExtAedPort);
		if(!ret)
		{
			pMessage->append("Error initializing standard AED sender @ " + pSettings->oscSendExtAedHost + ":" + String(pSettings->oscSendExtAedPort) + NewLine::getDefault(), 500);
			hasErrors = true;
		}
		else
		{
			hasSuccessful = true;
		}
	}

	for (auto target : pSettings->customOscTargets)
	{
		if (target->enabledFlag)
		{
			OSCSenderInstance* pInstance = getOrCreateInstance(index++);
			pInstance->setOscPath(target->oscString);
			bool ret = pInstance->connect(target->targetHost, target->targetPort);
			if(!ret)
			{
				pMessage->append("Error initializing custom OSC sender @ " + target->targetHost + ":" + String(target->targetPort) + ": " + target->oscString + NewLine::getDefault(), 500);
				hasErrors = true;
			}
			else
			{
				hasSuccessful = true;
			}
		}
	}

	if (hasSuccessful)
	{
		startTimer(pSettings->oscSendExtIntervalMs);
	}

	return !hasErrors;
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
	// create history elements if required
	while (pPoints->size() > history.size())
		history.add(new PointHistoryEntry());
		
	for (int i = 0; i < pPoints->size(); i++)
	{
		AmbiPoint* pt = pPoints->get(i);
		if (pt != nullptr && pt->getEnabled())
		{
			if (history[i]->update(pt))
			{
				for (auto sender : oscSender)
				{
					sender->sendMessage(pt, i);
				}
			}
		}
	}
}
